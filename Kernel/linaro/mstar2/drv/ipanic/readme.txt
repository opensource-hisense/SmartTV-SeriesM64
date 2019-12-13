Default ipanic not enabled in kernel config

if you need enable ipanic, do as following
a. rebuild kernel and enable ipanic in <MStar Platform Configuration>
b. create one partition named "ipanic", size 2M bytes at least
   in scripts\set_partition (mmc create ipanic 0x200000)
c. when panic, reboot board, then enter /proc/ipanic
   cat upload 
   or dump the info into a file, for example:
   cat upload > /data/app/log.txt
   
   
