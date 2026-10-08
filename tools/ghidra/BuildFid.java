// Headless: create (or open) a Function ID database and populate one library
// in it from every program under a project folder (recursively).
//
// Usage (via analyzeHeadless -preScript; the -process target is only a
// vehicle, use -noanalysis -readOnly so it is not touched):
//   BuildFid.java <fidb file> <project folder> <library> <version> <variant> <languageID>
// e.g.
//   BuildFid.java ghidra/vc8.fidb /fid/vc8/8.0.50727.762/mt vc8 8.0.50727.762 mt x86:LE:32:default
//
// The database is also attached as a user FID file so the Function ID
// analyzer picks it up in later analyses (GUI and headless).
import ghidra.app.script.GhidraScript;
import ghidra.feature.fid.db.*;
import ghidra.feature.fid.service.*;
import ghidra.framework.model.*;
import ghidra.program.database.ProgramContentHandler;
import ghidra.program.model.lang.LanguageID;
import java.io.File;
import java.util.*;

public class BuildFid extends GhidraScript {
  private void collect(DomainFolder folder, List<DomainFile> out) {
    for (DomainFile f : folder.getFiles()) {
      if (ProgramContentHandler.PROGRAM_CONTENT_TYPE.equals(f.getContentType())) out.add(f);
    }
    for (DomainFolder sub : folder.getFolders()) collect(sub, out);
  }

  @Override
  public void run() throws Exception {
    String[] a = getScriptArgs();
    if (a.length != 6) throw new IllegalArgumentException("args: <fidb> <folder> <library> <version> <variant> <languageID>");
    File dbFile = new File(a[0]).getAbsoluteFile();
    FidFileManager mgr = FidFileManager.getInstance();
    if (!dbFile.exists()) mgr.createNewFidDatabase(dbFile);
    FidFile fidFile = mgr.addUserFidFile(dbFile);

    DomainFolder root = state.getProject().getProjectData().getFolder(a[1]);
    if (root == null) throw new IllegalArgumentException("no project folder " + a[1]);
    List<DomainFile> programs = new ArrayList<>();
    collect(root, programs);
    println("BuildFid: " + programs.size() + " programs under " + a[1]);

    FidDB db = fidFile.getFidDB(true);
    try {
      FidPopulateResult r = new FidService().createNewLibraryFromPrograms(
          db, a[2], a[3], a[4], programs, null, new LanguageID(a[5]), null, null, monitor);
      println("BuildFid: attempted=" + r.getTotalAttempted() + " added=" + r.getTotalAdded()
          + " excluded=" + r.getTotalExcluded() + " failures=" + r.getFailures());
      db.saveDatabase("BuildFid", monitor);
    } finally {
      db.close();
    }
    println("BuildFid: wrote " + dbFile);
  }
}
