Author: Keshav Narula

# Systems-Programming-FinalProject
Multi-Threaded C program for a Ghost Hunt Simulation Game.

# Program Description:

Program using multiple threads for each entity, that simulates a ghost hunt game between 4 hunters and 1 ghost. Building type stores dynamic list of rooms (each with respective connectedRoomsList) and evidence master list. Ghost and 4 hunters loop in separate threads. Ghost starts simulation with a random ghost class which species which 3 out of 4 evidence types it can generate and therefore be identified for. Ghost starts with a boredom counter of 100 Ghost can do no action, create new evidence to leave in the room its currently in or change rooms, ghosts exits thread when its boredom counter hits zero. SMARTER BEHAVIOR: The hunters always prioritize sharing evidence first, If a hunter contains a evidence type or multiple that a hunter in the same room doesn't possess it will share it with said hunter, otherwise hunter can search the current room's evidence list for evidence, if their current room doesnt contain evidence of their searchable type they will create their own non Ghostly evidence, or change rooms. Hunter will exit thread for one of the three reasons, if it can identify the ghost after finding / being shared three different types of Ghostly evidence, if their fear counter hits 99 or if their boredom counter hits zero. The final results of the program will print after all the threads end, showing if either one of the hunters can identify the ghost, thus the hunters win, otherwise the Ghost wins.

Simulation behaviour:


# Entities 

A ghost is an entity that moves between rooms and leaves evidence behind inside the range of GHOSTLY evidence (also has a chance to produce standard evidence, which the hunters cannot use to identify the ghost). A starts with a random GhostClass (POLTERGEIST, BANSHEE, BULLIES, or PHANTOM), which is an enumerated data type representing the type of ghost it is, this species what 3 out of 4 evidence types it is able to generate and therefore be identified for. It also contains a pointer to the room that it is in. It also contains a boredom timer - an integer initially set to BOREDOM_MAX. Each time the ghost is in a room with a hunter, it resets the counter to BOREDOM_MAX. Each time the ghost is in a room without a hunter, it decreases the timer variable by 1. If the boredom timer reaches <= 0, it exits the thread. A ghost takes one of three actions: It has a random chance to generate evidence to add to the room, or it will move, or it will take no action. If a ghost is in a room with a hunter, it will not choose to move, but it may choose to take no action. These actions are selected at random.

A hunter is an entity that moves between rooms, reads the room for evidence, and communicates evidence to other hunter in room. They contain a pointer to the room they are currently in, an enumerated type representing the type of evidence their equipment can read (note: there is no equipment type, only the type of evidence they collect), a collection of evidence that they have personally collected, a name, and a fear integer which starts at 0. Hunters choose their action at random. If a hunter is in a room with a ghost when they make their decision, they gain 1 point of fear, specified by a definition in defs.h. If they reach 100 fear, they are removed from the simulation by closing their thread. Hunters can take one of HUNTERS are programmed with smarter behaviour than just random choice, Hunters will always prioritize sharing evidence to other hunters meaning they will first check their current room for other hunters and if that hunter contains some GHOSTLY evidence type that another hunter does not contain it will always share the evidence, no matter what type it is. If they communicate, ghostly evidence data is append the other hunter’s evidence collection. Ignore standard data. A hunters also have a Boredom coutner. Each time they detect ghostly evidence, reset the timer to BORING_MAX. Each time they take an action other than detecting ghostly evidence, decrease the timer by 1. If the timer is <= 0, they exit the thread. If a hunter finds three diferent types of GHOSTLY evidence it can predict the ghost type and exits the thread.

<img src="Example Output Images/Screenshot 2023-01-16 104758.png" width ="600">

<img src="Example Output Images/Screenshot 2023-01-16 104820.png" width ="600">

#Structures
Evidence is a data type containing the Evidence Class (or Category) and a value for the data of that reading. Evidence can be one of the following categories: EMF, TEMPERATURE, FINGERPRINTS, SOUND. The value for each piece of evidence is selected from the above table. A piece of evidence can have either a Ghost Reading or a Standard Reading; note, these readings are not reflected as fields. Instead, when we
create evidence, we generate the value for the evidence randomly following the table. Note: If a ghost produces a value that is within the standard results, it can not be used as evidence for confirming that ghost type. Evidence is stored by each hunter, and in each room. Multiple hunters may point to the same data, but once it is found, its pointer is removed from the room's Evidence List it was generated in.

A room represents a room in the building that the hunters are hunting in. A room has a name, a linked list of other rooms that it is connected to, a linked list of evidence that the ghost has left in the room, a collection of hunters currently in the room, and a pointer to the ghost. If the ghost is not in the room, the ghost pointer will be NULL. Also contains a mutex semaphore so that only one entity is ever manipulating the rooms data at any time to prevent deadlock. (All rooms are dynamically allocated and RoomsLists contain pointers to them)

A building is used to hold all of the information about the current hunt. It contains the ghost , a collection of all hunters, and a linked list of all rooms. The first room in every building is always the ghost hunter’s vehicle, the Van. Building also contains a master list of all rooms and evidence that has been generated during the simulation either by the ghost or hunters, this allows for neater de allocation of memory during or after the program.

Data Structures, Game Map, Basic Control Flow for the Threads are specified in pdf spec.
Additional Functionality
  + State of Simulation, printed, formatted in terminal
  + Smarter behaviour for Hunters
  + Evidence is dynamically allocated, no duplicates. Stored using pointers in one or multiple EvidenceLinkedLists. Including master list of 	evidence stored in Building strucutre
  + Simulation output printed to text file output.txt
 
Game Map:

<img src="Example Output Images/Screenshot 2023-01-16 021526.png" width="600" >

List of Files:
	defs.h , ghost.c , building.c , room.c , hunter.c , evidence.c , main.c , Makefile

Instructions for Compilation terminal:
	
	make run

	make clean (removes object files)

Instructions for Launching in terminal:
	
	./run
	
Example Terminal Output (MAX_FEAR & MAX_BOREDOM set to 25):

<img src="Example Output Images/01.png" width="700" >
<img src="Example Output Images/02.png" width="700" >
<img src="Example Output Images/03.png" width="700" >


