Author: Keshav Narula

# Systems-Programming-FinalProject
Multi-Threaded C program for a Ghost Hunt Simulation Game.

Program Description:

Program using multiple threads for each entity, that simulates a ghost hunt game between 4 hunters and 1 ghost. Building type stores dynamic list of rooms (each with respective connectedRoomsList) and evidence master list. Ghost and 4 hunters loop in separate threads. Ghost starts simulation with a random ghost class for which evidence it can leave and therefore be identified for. Ghost can do no action, create new evidence to leave in the room its currently in or change rooms, ghosts exits thread when its boredom counter hits zero. SMARTER BEHAVIOR: The hunters always prioritize sharing evidence first, If a hunter contains a evidence type or multiple that a hunter in the same room doesn't possess it will share it with said hunter, otherwise hunter can search the current room's evidence list for evidence, if their current room doesnt contain evidence of their searchable type they will create their own non Ghostly evidence, or change rooms. Hunter will exit thread for one of the three reasons, if it can identify the ghost after finding / being shared three different types of Ghostly evidence, if their fear counter hits 99 or if their boredom counter hits zero. The final results of the program will print after all the threads end, showing if either one of the hunters can identify the ghost, thus the hunters win, otherwise the Ghost wins.


Data Structures, Rules, Game Map, Basic Control Flow are specified in pdf spec.
  + State of Simulation, printed, formatted in terminal
  + Smarter behaviour for Hunters
  + Evidence is dynamically allocated, no duplicates. Stored using pointers in one or multiple EvidenceLinkedLists. Including master list of evidence stored in       	  Building strucutre
  + Simulation output printed to text file output.txt

List of Files:
	defs.h , ghost.c , building.c , room.c , hunter.c , evidence.c , main.c , Makefile

Instructions for Compilation terminal:
	
	make run

	make clean (removes object files)

Instructions for Launching in terminal:
	
	./run
