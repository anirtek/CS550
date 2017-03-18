obj-m := process_list.o
KDIR := /lib/modules/`uname -r`/build
PWD := `pwd`
default:
	make -C $(KDIR) M=$(PWD) modules
	gcc -std=c99 user_space.c -o user_space
