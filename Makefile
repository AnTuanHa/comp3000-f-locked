flock: main.o attr.o cipher.o
	gcc -o flock main.o attr.o cipher.o
	@echo -e "\n************"
	@echo "Run 'sudo setcap cap_sys_admin+ep ./flock' to give the program root access"
	@echo "which is required for setting extended attributes under the 'security' namespace"
	@echo -e "************"

main.o: main.c attr.h cipher.h
	gcc -c main.c

attr.o: attr.c attr.h
	gcc -c attr.c

cipher.o: cipher.c cipher.h
	gcc -c cipher.c

clean:
	rm flock *.o
