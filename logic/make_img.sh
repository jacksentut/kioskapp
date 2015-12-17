make clean;make
cp ./logic ./img_tmp/app
cp ./kioskapp ./img_tmp/app
#	cp -rf ./kioskimage ./img_tmp/app
cd ./img_tmp
tar cf ../image/kioskmini_20`date +%y%m%d`.bin *
cp ../image/kioskmini_20`date +%y%m%d`.bin ../image/kioskmini.bin
cd ..

#tar zcf ./image/kioskmini_20`date +%y%m%d`.bin ./app/*

