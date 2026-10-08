// Headless: run only the Function ID analyzer over the current program, with a
// given FID database attached. Seconds instead of a full re-analysis.
//
//   analyzeHeadless <proj> <name> -process LEGOBatman.exe -noanalysis \
//     -scriptPath tools/ghidra -preScript ApplyFid.java ghidra/vc8.fidb
import ghidra.app.script.GhidraScript;
import ghidra.app.util.importer.MessageLog;
import ghidra.feature.fid.analyzer.FidAnalyzer;
import ghidra.feature.fid.db.FidFileManager;
import ghidra.program.model.listing.Program;
import java.io.File;

public class ApplyFid extends GhidraScript {
  @Override
  public void run() throws Exception {
    for (String p : getScriptArgs()) {
      File f = new File(p).getAbsoluteFile();
      if (!f.exists()) throw new IllegalArgumentException("no such fidb: " + f);
      FidFileManager.getInstance().addUserFidFile(f);
    }
    FidAnalyzer fid = new FidAnalyzer();
    fid.registerOptions(currentProgram.getOptions(Program.ANALYSIS_PROPERTIES).getOptions("Function ID"), currentProgram);
    MessageLog log = new MessageLog();
    long before = currentProgram.getFunctionManager().getFunctionCount();
    fid.added(currentProgram, currentProgram.getMemory(), monitor, log);
    println("ApplyFid: ran Function ID over " + before + " functions");
    if (log.hasMessages()) println(log.toString());
  }
}
