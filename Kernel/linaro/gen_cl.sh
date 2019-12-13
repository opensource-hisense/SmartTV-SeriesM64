filename=p4_changelist.h
Kern=KERN-3.10.23.00
Kerm=KERM-3.10.23.00
version=`grep -o [0-9]* $filename`
Kern+=$version\"
Kerm+=$version\"
sed -i "s/KERN-3.10.23.*/$Kern/g" include/linux/release_version.h
sed -i "s/KERM-3.10.23.*/$Kern/g" include/linux/release_version.h
