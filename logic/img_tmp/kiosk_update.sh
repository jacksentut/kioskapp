#!/bin/sh
cp -rf /tmp/update/app/* /app
sync
if [ -f /media/mmcblk0p1/u-boot.imx ]; then  
	echo "File is exist."
	sleep 1	
else
	if [ -f /app/system/u-boot.imx ]; then 
		/usr/local/bin/update-uboot.sh /app/system/u-boot.imx
		cp /app/system/u-boot.imx /media/mmcblk0p1/
		sync
	fi
fi

