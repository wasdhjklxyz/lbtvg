// Apply tools/symbols/pc-names.csv (PC <-> Mac 1.0.1 pairs) to the current
// program. Renames functions that still have a default FUN_ name or a name an
// earlier run gave them (source IMPORTED, so corrections propagate). Names you
// set yourself in the GUI (source USER_DEFINED) are never touched. Weak pairings (order, gapfill) get a _hint suffix; puts
// C++ methods in their class namespace; adds the full Mac signature and the
// pairing method as a plate comment. Arg 0 (headless) or a file prompt (GUI):
// path to the csv. Re-runnable.
//@category lbtvg
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.address.Address;
import java.io.*;
import java.util.*;

public class ApplyNames extends GhidraScript {
  @Override
  public void run() throws Exception {
    String[] args = getScriptArgs();
    File csv = args.length > 0 ? new File(args[0]) : askFile("pc-names.csv", "Apply");
    SymbolTable st = currentProgram.getSymbolTable();
    FunctionManager fm = currentProgram.getFunctionManager();
    int renamed = 0, skipped = 0, missing = 0;
    try (BufferedReader r = new BufferedReader(new FileReader(csv))) {
      r.readLine();
      String line;
      while ((line = r.readLine()) != null) {
        List<String> f = parse(line);
        if (f.size() < 6) continue;
        Address a = toAddr(Long.parseLong(f.get(0), 16));
        Function fn = fm.getFunctionAt(a);
        if (fn == null) { missing++; continue; }
        // ours to (re)name: still FUN_, or named by an earlier run of this script
        // (IMPORTED). Anything you renamed in the GUI is USER_DEFINED: left alone.
        SourceType src = fn.getSymbol().getSource();
        boolean ours = fn.getName().startsWith("FUN_") || src == SourceType.IMPORTED;
        if (!ours) { skipped++; continue; }
        String method = f.get(4);
        boolean hint = method.equals("order") || method.equals("gapfill");
        String dem = f.get(3);
        String name = dem.contains("(") ? dem.substring(0, dem.indexOf('(')) : dem;
        int sp = name.lastIndexOf(' ');           // drop return types if any
        if (sp >= 0 && !name.contains("operator")) name = name.substring(sp + 1);
        if (name.startsWith("_") && !name.contains("::")) name = name.substring(1);
        Namespace ns = currentProgram.getGlobalNamespace();
        String[] parts = name.split("::");
        for (int i = 0; i < parts.length - 1; i++) {
          Namespace n = st.getNamespace(parts[i], ns);
          ns = n != null ? n : st.createNameSpace(ns, parts[i], SourceType.IMPORTED);
        }
        try {
          // weak pairings carry _hint so the listing says "check me"
          fn.setName(parts[parts.length - 1] + (hint ? "_hint" : ""), SourceType.IMPORTED);
          fn.setParentNamespace(ns);
          fn.setComment("mac 1.0.1: " + dem + "  [" + method + (hint ? ": hint, verify against the code" : "") + "]");
          renamed++;
        } catch (Exception e) {
          println("skip " + f.get(0) + " " + name + ": " + e.getMessage());
          skipped++;
        }
      }
    }
    println("ApplyNames: renamed=" + renamed + " skipped=" + skipped + " missing=" + missing);
  }

  private static List<String> parse(String line) {
    List<String> out = new ArrayList<>();
    StringBuilder cur = new StringBuilder();
    boolean q = false;
    for (char c : line.toCharArray()) {
      if (c == '"') q = !q;
      else if (c == ',' && !q) { out.add(cur.toString()); cur.setLength(0); }
      else cur.append(c);
    }
    out.add(cur.toString());
    return out;
  }
}
