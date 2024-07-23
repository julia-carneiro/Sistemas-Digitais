obj-m += kekel.o

all: 
# 	gcc criar_driver.c -o driver
# 	gcc biblioteca.c -o biblio
# 	gcc main.c -o main 

# 	sudo ./driver
# 	sudo ./biblio
# 	sudo ./main
	
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules 

clean: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean