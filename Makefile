ORIG        ?= orig
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

# One-shot: import + auto-analyze orig/LEGOBatman.exe into $(GHIDRA_PROJ)/ (read-only on the exe).
ghidra-import: verify ghidra-check
	$(ANALYZE_HEADLESS) $(GHIDRA_PROJ) $(GHIDRA_NAME) \
		-import $(ORIG)/LEGOBatman.exe -overwrite \
		-scriptPath $(SCRIPTS) -preScript PreAnalysis.java \
		-postScript DumpStats.java $(GHIDRA_PROJ)/stats.tsv

ghidra: ghidra-check
	$(GHIDRA_RUN) $(abspath $(GHIDRA_PROJ)/$(GHIDRA_NAME).gpr)

ghidra-check:
	@command -v "$(ANALYZE_HEADLESS)" >/dev/null && command -v "$(GHIDRA_RUN)" >/dev/null \
		|| { echo "ghidra not found: set GHIDRA_INSTALL_DIR (see docs/setup.md)" >&2; exit 1; }

.PHONY: all verify ghidra-import ghidra ghidra-check
