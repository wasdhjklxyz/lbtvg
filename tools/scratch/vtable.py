# vtable.py ADDR: find the vtable(s) holding ADDR (directly or through an
# incremental-link jmp thunk) and print every slot, thunks resolved.
import sys,pefile,struct
pe=pefile.PE('orig/LEGOBatman.exe');img=pe.get_memory_mapped_image();B=0x400000
t=int(sys.argv[1],16)
# build thunk map: e9 jumps in the thunk table region (scan .text start)
def res(v):
    o=v-B
    if 0<=o<len(img)-5 and img[o]==0xe9:
        return v+5+struct.unpack_from('<i',img,o+1)[0]
    return v
thunks=[]
for o in range(0x1000,0x60000):
    if img[o]==0xe9 and o+5<len(img):
        d=B+o+5+struct.unpack_from('<i',img,o+1)[0]
        if d==t: thunks.append(B+o)
print('thunks',[hex(x) for x in thunks])
for th in thunks+[t]:
    pat=struct.pack('<I',th);i=img.find(pat)
    while i!=-1:
        s=i
        while 0x401000<=struct.unpack_from('<I',img,s-4)[0]<0x800000: s-=4
        out=[];k=s
        while 0x401000<=struct.unpack_from('<I',img,k)[0]<0x800000:
            out.append(res(struct.unpack_from('<I',img,k)[0]));k+=4
        print('vtable %x: %d slots'%(B+s,len(out)))
        print(' '.join('%d:%x'%(n,v) for n,v in enumerate(out)))
        i=img.find(pat,i+1)
