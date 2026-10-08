ORIG        ?= orig
EXE         ?= $(ORIG)/LEGOBatman.exe
SHA1SUM     ?= sha1sum
GHIDRA_PROJ ?= ghidra
GHIDRA_NAME ?= lbtvg
SCRIPTS     := tools/ghidra

# Ghidra 12.1.2. Manual installs: set GHIDRA_INSTALL_DIR to the dir containing
# ghidraRun and support/. nix develop exports ANALYZE_HEADLESS and GHIDRA_RUN
# directly (its wrappers are not a stock layout). See docs/setup.md.
ANALYZE_HEADLESS ?= $(GHIDRA_INSTALL_DIR)/support/analyzeHeadless
GHIDRA_RUN       ?= $(GHIDRA_INSTALL_DIR)/ghidraRun

all: verify

verify:
	$(SHA1SUM) -c $(ORIG)/checksum.sha1

# One-shot: import + auto-analyze $(EXE) into $(GHIDRA_PROJ)/ (read-only on the exe).
# make ghidra-import EXE=orig/LEGOBatmanDemo.exe for the demo build.
ghidra-import: verify ghidra-check
	$(ANALYZE_HEADLESS) $(GHIDRA_PROJ) $(GHIDRA_NAME) \
		-import $(EXE) -overwrite \
		-scriptPath $(SCRIPTS) -preScript PreAnalysis.java \
		-postScript DumpStats.java $(GHIDRA_PROJ)/$(notdir $(EXE)).stats.tsv

ghidra: ghidra-check
	$(GHIDRA_RUN) $(abspath $(GHIDRA_PROJ)/$(GHIDRA_NAME).gpr)

ghidra-check:
	@command -v "$(ANALYZE_HEADLESS)" >/dev/null && command -v "$(GHIDRA_RUN)" >/dev/null \
		|| { echo "ghidra not found: set GHIDRA_INSTALL_DIR (see docs/setup.md)" >&2; exit 1; }

.PHONY: all verify ghidra-import ghidra ghidra-check

# --- Function ID: name the statically linked CRT in the game so it is skipped.
VC8      ?= $(HOME)/.local/share/lbtvg/vc8
VC8_VER  := 8.0.50727.762
FID_DIR  := /fid/vc8/$(VC8_VER)/mt
FIDB     := $(GHIDRA_PROJ)/vc8.fidb
LANG_ID  := x86:LE:32:default

# 1. import every .obj of the static CRT libs as programs under $(FID_DIR)
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

# 3. run just the Function ID analyzer on $(EXE) with $(FIDB), dump stats again
fid-apply: ghidra-check
	$(ANALYZE_HEADLESS) $(GHIDRA_PROJ) $(GHIDRA_NAME) \
		-process $(notdir $(EXE)) -noanalysis \
		-scriptPath $(SCRIPTS) \
		-preScript ApplyFid.java $(FIDB) \
		-postScript DumpStats.java $(GHIDRA_PROJ)/$(notdir $(EXE)).stats.tsv

fid: fid-import fid-build fid-apply

.PHONY: fid fid-import fid-build fid-apply
