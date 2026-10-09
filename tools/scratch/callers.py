# callers.py ADDR...: list every e8/e9 rel32 to ADDR in .text, with the
# containing function's bounds found from int3 padding (ghidra misses many
# starts). For register-convention statics: all callers share its TU.
import sys, struct, pefile
pe = pefile.PE('orig/LEGOBatman.exe', fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
s = [s for s in pe.sections if s.Name.startswith(b'.text')][0]
data, va = s.get_data(), base + s.VirtualAddress

def start(o):
    while o > 0 and not (o % 16 == 0 and data[o - 1] == 0xcc):
        o -= 1
    return o

def end(o):
    o += 1
    while not (o % 16 == 0 and data[o - 1] == 0xcc):
        o += 1
    while data[o - 1] == 0xcc:
        o -= 1
    return o

for arg in sys.argv[1:]:
    t = int(arg, 16)
    for i in range(len(data) - 5):
        if data[i] in (0xe8, 0xe9) and va + i + 5 + struct.unpack_from('<i', data, i + 1)[0] == t:
            f = start(i)
            print('%08x -> %08x %s  in %08x..%08x' % (va + i, t, 'call' if data[i] == 0xe8 else 'jmp ', va + f, va + end(f)))
