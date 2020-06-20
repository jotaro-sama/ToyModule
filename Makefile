obj-m := guestbook.o
# If built from other files as well: 
# <module_name>-y := <src1>.o <src2>.o ...


all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
