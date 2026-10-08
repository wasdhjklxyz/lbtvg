ORIG ?= orig
all: verify
verify:
	sha1sum -c $(ORIG)/checksum.sha1

GHIDRA_PROJ ?= ghidra
GHIDRA_NAME ?= lbtvg
SCRIPTS     := tools/ghidra

# One-shot: import + auto-analyze orig/LEGOBatman.exe into $(GHIDRA_PROJ)/ (read-only on the exe).
ghidra-import: verify
	ghidra-analyzeHeadless $(GHIDRA_PROJ) $(GHIDRA_NAME) \
		-import $(ORIG)/LEGOBatman.exe -overwrite \
		-scriptPath $(SCRIPTS) -preScript PreAnalysis.java \
		-postScript DumpStats.java $(GHIDRA_PROJ)/stats.tsv

ghidra:
	ghidra $(GHIDRA_PROJ)/$(GHIDRA_NAME).gpr

.PHONY: all verify ghidra-import ghidra
