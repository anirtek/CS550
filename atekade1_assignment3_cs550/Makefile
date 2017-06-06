obj-m := linepipe.o
KDIR := /lib/modules/`uname -r`/build
PWD := `pwd`
default:
	make -C $(KDIR) M=$(PWD) modules
	gcc -std=c99 producer.c -o producer
	gcc -std=c99 consumer.c -o consumer
