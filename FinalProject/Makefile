OPT = -Wall
OBJ = main.o building.o hunter.o room.o ghost.o evidence.o


run: $(OBJ)
	gcc $(OPT) -o run $(OBJ) -lpthread

main.o: main.c defs.h
	gcc $(OPT) -c main.c

building.o: building.c defs.h
	gcc $(OPT) -c building.c
        
hunter.o: hunter.c defs.h
	gcc $(OPT) -c hunter.c
        
room.o: room.c defs.h
	gcc $(OPT) -c room.c
        
ghost.o: ghost.c defs.h
	gcc $(OPT) -c ghost.c
        
evidence.o: evidence.c defs.h
	gcc $(OPT) -c evidence.c

clean:
	rm -f $(OBJ)
