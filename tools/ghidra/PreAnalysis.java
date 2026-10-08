// Headless pre-script: turn on analyzers that are off by default but matter
// for a relocation-stripped PE (Ghidra can't pointer-scan without relocs, so
// vtable-only-reachable code is missed without the aggressive finder).
import ghidra.app.script.GhidraScript;
public class PreAnalysis extends GhidraScript {
  public void run() throws Exception {
    setAnalysisOption(currentProgram, "Aggressive Instruction Finder", "true");
    setAnalysisOption(currentProgram, "Non-Returning Functions - Discovered", "true");
  }
}
