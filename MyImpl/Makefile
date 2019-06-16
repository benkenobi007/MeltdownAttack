KVERSION = $(shell uname -r)
obj-m = KernelModule.o

CFLAGS_KernelModule.o := -march=native

all : 
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean	
