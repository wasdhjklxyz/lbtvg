# rtti.py NAME...: print vtable(s) for RTTI class names, slots resolved through jmp thunks
import sys,pefile,struct
pe=pefile.PE('orig/LEGOBatman.exe');img=pe.get_memory_mapped_image();B=0x400000
def res(v):
    o=v-B
    if 0<=o<len(img)-5 and img[o]==0xe9: return v+5+struct.unpack_from('<i',img,o+1)[0]
    return v
for name in sys.argv[1:]:
    s=('.?AV%s@@'%name).encode()+b'\0'
    i=img.find(s)
    if i<0: print(name,'no td'); continue
    td=B+i-8
    p=struct.pack('<I',td); j=img.find(p)
    while j!=-1:
        col=B+j-12
        if struct.unpack_from('<I',img,j-12)[0]==0:
            q=struct.pack('<I',col); k=img.find(q)
            while k!=-1:
                vt=B+k+4; out=[]; m=k+4
                while 0x401000<=struct.unpack_from('<I',img,m)[0]<0x800000:
                    out.append(res(struct.unpack_from('<I',img,m)[0])); m+=4
                print('%s vtable %x: %s'%(name,vt,' '.join('%d:%x'%(n,v) for n,v in enumerate(out))))
                k=img.find(q,k+1)
        j=img.find(p,j+1)
