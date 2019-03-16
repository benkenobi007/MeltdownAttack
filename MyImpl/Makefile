KVERSION = $(shell uname -r)
obj-m = KernelModule.o

all : 
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean	
