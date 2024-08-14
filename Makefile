KDIR= /lib/modules/$(shell uname -r)/build
PWD= $(shell pwd)

obj-m := uio_kbd.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

uio_app:	uio_app.o

install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
	depmod -a

clean:
	rm -f *~ Module.markers Modules.symvers
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f uio_app

