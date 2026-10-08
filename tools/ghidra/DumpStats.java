import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.mem.*;
import java.io.*;
public class DumpStats extends GhidraScript {
  public void run() throws Exception {
    PrintWriter w = new PrintWriter(new FileWriter(getScriptArgs()[0]));
    FunctionManager fm = currentProgram.getFunctionManager();
    long n=0, bytes=0, thunks=0;
    for (Function f : fm.getFunctions(true)) {
      n++; if (f.isThunk()) { thunks++; continue; }
      long sz = f.getBody().getNumAddresses(); bytes += sz;
      w.println(f.getEntryPoint()+"\t"+sz+"\t"+f.getCallingConventionName()+"\t"+f.getName());
    }
    w.println("# functions="+n+" thunks="+thunks+" bytes="+bytes);
    long vt=0, named=0;
    for (Symbol s : currentProgram.getSymbolTable().getAllSymbols(false)) {
      String nm = s.getName();
      if (nm.contains("vftable")) { vt++; w.println("# VT\t"+s.getAddress()+"\t"+s.getName(true)); }
    }
    w.println("# vftables="+vt);
    MemoryBlock t = currentProgram.getMemory().getBlock(".text");
    w.println("# .text size="+t.getSize());
    w.close();
  }
}
