# lbtvg — see docs/setup.md for what each tool is and where it comes from.

# --- inputs ------------------------------------------------------------------
ORIG        ?= orig
EXE         ?= $(ORIG)/LEGOBatman.exe
SHA1SUM     ?= sha1sum

# --- ghidra ------------------------------------------------------------------
# Manual installs: set GHIDRA_INSTALL_DIR to the dir containing ghidraRun and
# support/. nix develop exports ANALYZE_HEADLESS and GHIDRA_RUN directly.
GHIDRA_PROJ      ?= ghidra
GHIDRA_NAME      ?= lbtvg
ANALYZE_HEADLESS ?= $(GHIDRA_INSTALL_DIR)/support/analyzeHeadless
GHIDRA_RUN       ?= $(GHIDRA_INSTALL_DIR)/ghidraRun
SCRIPTS          := tools/ghidra
STATS             = $(GHIDRA_PROJ)/$(notdir $(EXE)).stats.tsv

# --- compiler (tools/vc8.sh) -------------------------------------------------
TOOLCHAIN ?= toolchain
VC8       ?= $(TOOLCHAIN)/vc8
WINSDK6   ?= $(TOOLCHAIN)/winsdk6
VC8_VER   := 8.0.50727.762

# --- Function ID -------------------------------------------------------------
FID_DIR := /fid/vc8/$(VC8_VER)/mt
FIDB    := $(GHIDRA_PROJ)/vc8.fidb
LANG_ID := x86:LE:32:default

# =============================================================================
all: verify

# sha1 every reference binary in orig/
verify:
	$(SHA1SUM) -c $(ORIG)/checksum.sha1

# compile every annotated function in src/ and diff it against orig/ (FUNC=0x... for one)
match:
	tools/match.py $(FUNC)

# same, verbose: side-by-side disassembly even for matches
match-v:
	tools/match.py -v $(FUNC)

# clang-format every file under src/ (.clang-format pins the style)
fmt:
	clang-format -i $(shell find src -name "*.c" -o -name "*.cpp" -o -name "*.h")

# fail if anything under src/ is not formatted (what CI will run)
fmt-check:
	clang-format --dry-run -Werror $(shell find src -name "*.c" -o -name "*.cpp" -o -name "*.h")

# start a function: pick its file, insert saga's body (or a TODO), try to match
new:
	tools/new.py $(FUNC)

# regenerate docs/linkmap.md (which source file owns which address range)
linkmap:
	tools/linkmap.py > docs/linkmap.md

# fetch + verify the VC8 SP1 compiler into $(TOOLCHAIN)/ (override: LBTVG_TOOLCHAIN)
vc8:
	LBTVG_TOOLCHAIN=$(abspath $(TOOLCHAIN)) tools/vc8.sh

# --- ghidra ------------------------------------------------------------------
# import + auto-analyze $(EXE) into the project (read-only on the exe).
# EXE=orig/LEGOBatmanDemo.exe imports the demo as a second program.
ghidra-import: verify ghidra-check
	$(ANALYZE_HEADLESS) $(GHIDRA_PROJ) $(GHIDRA_NAME) \
		-import $(EXE) -overwrite \
		-scriptPath $(SCRIPTS) -preScript PreAnalysis.java \
		-postScript DumpStats.java $(STATS)

# open the project in the GUI
ghidra: ghidra-check
	$(GHIDRA_RUN) $(abspath $(GHIDRA_PROJ)/$(GHIDRA_NAME).gpr)

ghidra-check:
	@command -v "$(ANALYZE_HEADLESS)" >/dev/null && command -v "$(GHIDRA_RUN)" >/dev/null \
		|| { echo "ghidra not found: set GHIDRA_INSTALL_DIR (see docs/setup.md)" >&2; exit 1; }

# --- Function ID: name the statically linked CRT in the game so it is skipped
# apply tools/symbols/pc-names.csv (Mac names) to $(EXE); GUI must be closed
names: ghidra-check
	$(ANALYZE_HEADLESS) $(GHIDRA_PROJ) $(GHIDRA_NAME) \
		-process $(notdir $(EXE)) -noanalysis \
		-scriptPath $(SCRIPTS) \
		-preScript ApplyNames.java $(abspath tools/symbols/pc-names.csv)

# regenerate the badge, docs/todo.md and site/data.json (the hook does this on commit)
progress:
	tools/progress.py --write

# once per clone: pre-commit hook (match check + progress) and submodules that
# follow pull/checkout (ref/saga stays on the pinned commit)
hooks:
	git config core.hooksPath tools/hooks
	git config submodule.recurse true

# regenerate tools/symbols/pc-names.csv from the Mac 1.0.1 symbols
macnames:
	tools/macnames.py

fid: fid-import fid-build fid-apply

# 1. every .obj of the static CRT libs becomes a program under $(FID_DIR)
fid-import: ghidra-check
	$(ANALYZE_HEADLESS) $(GHIDRA_PROJ) $(GHIDRA_NAME)$(FID_DIR) \
		-import $(VC8)/LIB/libcmt.lib $(VC8)/LIB/libcpmt.lib -recursive \
		-preScript FunctionIDHeadlessPrescript.java \
		-postScript FunctionIDHeadlessPostscript.java

# 2. hash them into $(FIDB) and attach it for the analyzer
fid-build: ghidra-check
	$(ANALYZE_HEADLESS) $(GHIDRA_PROJ) $(GHIDRA_NAME) \
		-process $(notdir $(EXE)) -noanalysis -readOnly \
		-scriptPath $(SCRIPTS) \
		-preScript BuildFid.java $(FIDB) $(FID_DIR) vc8 $(VC8_VER) mt $(LANG_ID)

# 3. run only the Function ID analyzer on $(EXE), dump stats again
fid-apply: ghidra-check
	$(ANALYZE_HEADLESS) $(GHIDRA_PROJ) $(GHIDRA_NAME) \
		-process $(notdir $(EXE)) -noanalysis \
		-scriptPath $(SCRIPTS) \
		-preScript ApplyFid.java $(FIDB) \
		-postScript DumpStats.java $(STATS)

.PHONY: all verify progress hooks new names macnames match match-v fmt fmt-check linkmap vc8 ghidra-import ghidra ghidra-check fid fid-import fid-build fid-apply
