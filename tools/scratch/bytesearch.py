# bytesearch.py HEX...: find each byte pattern (hex, '??' wildcards allowed)
# in .text and print the hit with its containing function (int3 bounds).
import sys, re, pefile
pe = pefile.PE('orig/LEGOBatman.exe', fast_load=True)
base = pe.OPTIONAL_HEADER.ImageBase
s = [s for s in pe.sections if s.Name.startswith(b'.text')][0]
data, va = s.get_data(), base + s.VirtualAddress

def start(o):
    while o > 0 and not (o % 16 == 0 and data[o - 1] == 0xcc):
        o -= 1
    return o

for arg in sys.argv[1:]:
    h = arg.replace(' ', '')
    pat = b''.join(b'.' if h[i:i+2] == '??' else re.escape(bytes([int(h[i:i+2], 16)])) for i in range(0, len(h), 2))
    for m in re.finditer(pat, data, re.S):
        print('%08x  in %08x' % (va + m.start(), va + start(m.start())))
