PROG_NAME := flocked

$(PROG_NAME): main.o attr.o cipher.o perm.o
	gcc -o flocked main.o attr.o cipher.o perm.o
	@echo -e "\n************"
	@echo "Run 'sudo setcap cap_sys_admin+ep ./$(PROG_NAME)' to give the program root access"
	@echo "which is required for setting extended attributes under the 'security' namespace"
	@echo -e "************"

main.o: main.c attr.h cipher.h perm.h
	gcc -c main.c

attr.o: attr.c attr.h
	gcc -c attr.c

cipher.o: cipher.c cipher.h
	gcc -c cipher.c

perm.o: perm.c perm.h
	gcc -c perm.c

clean:
	rm $(PROG_NAME) *.o

install:
	cp ./$(PROG_NAME) /usr/local/bin
	setcap cap_sys_admin+ep /usr/local/bin/$(PROG_NAME)
