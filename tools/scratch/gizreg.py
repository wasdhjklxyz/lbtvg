# gizreg.py: find every XXX_RegisterGizmo (copies Default_ADDGIZMOTYPE
# 0x960118 with rep movsd, then stores callbacks into the static addtype) and
# print each callback slot with its annotation state; slot names from saga's
# ADDGIZMOTYPE.fns order. Callbacks are statics, so referenced directly.
import re,glob,struct,csv,pefile
pe=pefile.PE('orig/LEGOBatman.exe');img=pe.get_memory_mapped_image()
ann={}
for f in glob.glob('src/**/*.c*',recursive=True):
    for m in re.finditer(r'//\s*(FUNCTION|STUB):\s*LEGOBATMAN\s+0x([0-9a-fA-F]+)',open(f,errors='ignore').read()): ann[int(m.group(2),16)]=m.group(1)[0]
nm={int(r['pc_addr'],16):r['demangled'] for r in csv.DictReader(open('tools/symbols/pc-names.csv'))}
SLOT=['get_max_gizmos','add_gizmos','early_update','late_update','draw','panel_draw','get_gizmo_name','get_output','get_output_name','get_num_outputs','activate','activate_rev','set_visibility','get_visibility','get_pos','using_special','bolt_hit_plat','get_best_bolt_target','bolt_hit','allocate_progress_data','clear_progress','store_progress','reset','reserve_buffer_space','load','x25','x26','x27']
pat=bytes.fromhex('be18019600bf')
for m in re.finditer(re.escape(pat),img):
    o=m.start(); base=struct.unpack('<I',img[o+6:o+10])[0]
    fs=o
    while img[fs-1]!=0xcc: fs-=1
    fa=fs+0x400000
    end=o+0x200; slots={}
    i=o
    while i<end:
        if img[i:i+2]==b'\xc7\x05':
            dst,val=struct.unpack('<II',img[i+2:i+10])
            off=dst-base
            if 0xc<=off<0x7c and 0x401000<=val<0x740000: slots[(off-0xc)//4]=val
            i+=10; continue
        if img[i]==0xc3: break
        i+=1
    print(f'{fa:08x} {ann.get(fa,"-")} {nm.get(fa,"")[:50]}')
    for k in sorted(slots):
        v=slots[k]; print(f'   {SLOT[k]:22} {v:08x} {ann.get(v,"-")} {nm.get(v,"")[:40]}')
