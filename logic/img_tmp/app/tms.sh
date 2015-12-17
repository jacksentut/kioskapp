HTTP_URL=$1
wget -P /tmp/update ${HTTP_URL}
if [ "$?" = "0" ]; then
	echo 1 > /tmp/update/result
else
	echo 0 > /tmp/update/result
fi

