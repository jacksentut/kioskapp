TARGET=library.a

TOPDIR := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)

OBJS    += libautoprinter.o
OBJS    += libtimer.o
OBJS    += libserial.o

INCLUDEDIR +=-I./
INCLUDEDIR +=-I$(TOPDIR)/include
CFLAGS  += $(INCLUDEDIR)

#CROSS=arm-linux-
#CROSS=/opt/poky/1.6/sysroots/i686-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
CROSS=/home/yocto/fsl-community-bsp/build-fb/tmp/sysroots/x86_64-linux/usr/bin/cortexa9hf-vfp-neon-poky-linux-gnueabi/arm-poky-linux-gnueabi-

CC=$(CROSS)gcc
#AR=$(CC)
AR=$(CROSS)ar rc
RANLIB=$(CROSS)ranlib
STRIP=$(CROSS)strip

CPPFLAGS +=

SRCS=$(OBJS:.o=.c)

%.o:%.c
	@echo ""
	@echo " ****  Compiling $< ..."
	$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<
#$(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<
	
#all: $(TARGET)

$(TARGET): $(OBJS)
#	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $@ 
#$(STRIP) $(TARGET)
    

clean:
	rm -rf *.o
