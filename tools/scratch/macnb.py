# macnb.py MACADDR|NAME [N=12]: Mac text symbols around an address or name
# (demangled), in address order, with the PC address when pc-names.csv pairs it.
import sys,csv
nm=[]
for l in open('tools/symbols/mac-1.0.1.demangled.nm'):
    p=l.rstrip('\n').split(' ',2)
    if len(p)==3 and p[1] in 'Tt': nm.append((int(p[0],16),p[2]))
nm.sort()
pc={int(r['mac_addr'],16):r['pc_addr'] for r in csv.DictReader(open('tools/symbols/pc-names.csv')) if r['mac_addr']}
q=sys.argv[1];n=int(sys.argv[2]) if len(sys.argv)>2 else 12
try: a=int(q,16); i=min(range(len(nm)),key=lambda k:abs(nm[k][0]-a))
except ValueError: i=[k for k,(x,s) in enumerate(nm) if q in s][0]
for k in range(max(0,i-n),min(len(nm),i+n+1)):
    x,s=nm[k]; print(('>' if k==i else ' ')+f'{x:08x} {pc.get(x,""):8} {s[:100]}')
