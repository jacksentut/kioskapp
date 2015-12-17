kill -9 `pidof udhcpc`

if [ -z $1 ] || [ -z $2 ] || [ -z $3 ]
then
	exit 1
fi
ifconfig eth0 $1 netmask $2
if [ $? -ne 0 ]
then
	echo "ip set fail"
	exit 1
fi
route del default
route add default gw $3 dev eth0
if [ $? -ne 0 ]
then
	echo "ip set fail"
	exit 1
else
	echo "ip set success"
	exit 0
fi

