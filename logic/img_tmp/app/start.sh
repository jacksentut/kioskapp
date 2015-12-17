#!/bin/sh

# SSH daemon service stop 
/etc/init.d/dropbear stop


TARGET=logic
TARGET1=kioskapp

kill -9 `pidof udhcpc`
#kill -9 `pidof $TARGET1`
#kill -9 `pidof $TARGET`

/app/static.sh 192.168.10.89 255.255.255.0 192.168.10.1

chmod 755 $TARGET
chmod 755 $TARGET1

./$TARGET&
./$TARGET1 -platform linuxfb -plugin Tslib

