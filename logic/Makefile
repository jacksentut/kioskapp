TARGET=logic

OBJS    += lib/libautoprinter.o
OBJS    += lib/libserial.o
OBJS    += lib/libtimer.o
OBJS    += appmain.o
OBJS    += logic.o
OBJS    += msganal.o
OBJS    += msghndlr.o
OBJS    += msgq.o
OBJS    += msgthread.o
OBJS    += api_lib.o
OBJS    += appinit.o
OBJS    += applib.o
OBJS    += thprt.o
OBJS    += bill_acceptor.o
OBJS    += datacap.o
OBJS    += apptms.o

INCLUDEDIR=-I./
INCLUDEDIR+=-I./inp_enc
CFLAGS  += $(INCLUDEDIR)

#CROSS=arm-linux-
#CROSS=/opt/poky/1.6/sysroots/i686-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
CROSS=/home/yocto/fsl-community-bsp/build-fb/tmp/sysroots/x86_64-linux/usr/bin/cortexa9hf-vfp-neon-poky-linux-gnueabi/arm-poky-linux-gnueabi-
CC=$(CROSS)gcc
CCP=$(CROSS)g++
AR=$(CC)
RANLIB=$(CROSS)ranlib
STRIP=$(CROSS)strip

SRCS=$(OBJS:.o=.c)

%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $< 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CCP) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $@ -L./inp_enc -lINPCipherService -lpthread
#	cp logic /home/yocto/work/nfs/KIOSK/
	cp ./logic /home/yocto/ftp/
	cp ./kioskapp /home/yocto/ftp/
#	./make_img.sh
	$(STRIP) $(TARGET)
    

clean:
	rm -rf $(TARGET) *.o
