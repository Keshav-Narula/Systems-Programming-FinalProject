Author: Keshav Narula

# Systems-Programming-FinalProject
Multi-Threaded C program for a Ghost Hunt Simulation Game.

# DEMO LINK
To demo project in an online C complier  go to link [HERE](https://replit.com/join/gavzaqukso-narulakeshav1)
- Open the Shell tab, type 'make run; to compile the program and './run' to start the program.

# Program Description:

Multi-threaded Ghost hunt game simulation between 4 hunters and 1 ghost, in a map of connected rooms. Hunters are attempting to collect collect different types of evidence which the ghost generates a specific in rooms as it roams, each hunter can only identifies one unique kind of evidence, and share missing evidence they collected as they cross paths in rooms. Ghost is attempting to increment each hunter's scare level counter, by being in the same room as the hunter, if level is maxed out that hunter is eliminated and their thread is returned. Game ends when either the with the ghost thread winning by successfully returning causing all the hunter threads to terminate, or the threads are able to collect enough evidence types to identify the type of ghost.
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

Additional Functionality
  + State of Simulation, printed, formatted in terminal
  + Smarter behaviour for Hunters
  + Evidence is dynamically allocated, no duplicates. Stored using pointers in one or multiple EvidenceLinkedLists. Including master list of 	evidence stored in Building strucutre
  + Simulation output printed to text file output.txt
 
# Structures
Evidence is a data type containing the Evidence Class (or Category) and a value for the data of that reading. Evidence can be one of the following categories: EMF, TEMPERATURE, FINGERPRINTS, SOUND. The value for each piece of evidence is selected from the above table. A piece of evidence can have either a Ghost Reading or a Standard Reading; note, these readings are not reflected as fields. Instead, when we
create evidence, we generate the value for the evidence randomly following the table. Note: If a ghost produces a value that is within the standard results, it can not be used as evidence for confirming that ghost type. Evidence is stored by each hunter, and in each room. Multiple hunters may point to the same data, but once it is found, its pointer is removed from the room's Evidence List it was generated in.

A room represents a room in the building that the hunters are hunting in. A room has a name, a linked list of other rooms that it is connected to, a linked list of evidence that the ghost has left in the room, a collection of hunters currently in the room, and a pointer to the ghost. If the ghost is not in the room, the ghost pointer will be NULL. Also contains a mutex semaphore so that only one entity is ever manipulating the rooms data at any time to prevent deadlock. (All rooms are dynamically allocated and RoomsLists contain pointers to them)

A building is used to hold all of the information about the current hunt. It contains the ghost , a collection of all hunters, and a linked list of all rooms. The first room in every building is always the ghost hunter’s vehicle, the Van. Building also contains a master list of all rooms and evidence that has been generated during the simulation either by the ghost or hunters, this allows for neater de allocation of memory during or after the program.


# Entities 

A ghost is an entity that moves between rooms and leaves evidence behind inside the range of GHOSTLY evidence (also has a chance to produce standard evidence, which the hunters cannot use to identify the ghost). A starts with a random GhostClass (POLTERGEIST, BANSHEE, BULLIES, or PHANTOM), which is an enumerated data type representing the type of ghost it is, this species what 3 out of 4 evidence types it is able to generate and therefore be identified for. It also contains a pointer to the room that it is in. It also contains a boredom timer - an integer initially set to BOREDOM_MAX. Each time the ghost is in a room with a hunter, it resets the counter to BOREDOM_MAX. Each time the ghost is in a room without a hunter, it decreases the timer variable by 1. If the boredom timer reaches <= 0, it exits the thread. A ghost takes one of three actions: It has a random chance to generate evidence to add to the room, or it will move, or it will take no action. If a ghost is in a room with a hunter, it will not choose to move, but it may choose to take no action. These actions are selected at random.

A hunter is an entity that moves between rooms, reads the room for evidence, and communicates evidence to other hunter in room. They contain a pointer to the room they are currently in, an enumerated type representing the type of evidence their equipment can read (note: there is no equipment type, only the type of evidence they collect), a collection of evidence that they have personally collected, a name, and a fear integer which starts at 0. Hunters choose their action at random. If a hunter is in a room with a ghost when they make their decision, they gain 1 point of fear, specified by a definition in defs.h. If they reach 100 fear, they are removed from the simulation by closing their thread. Hunters can take one of HUNTERS are programmed with smarter behaviour than just random choice, Hunters will always prioritize sharing evidence to other hunters meaning they will first check their current room for other hunters and if that hunter contains some GHOSTLY evidence type that another hunter does not contain it will always share the evidence, no matter what type it is. If they communicate, ghostly evidence data is append the other hunter’s evidence collection. Ignore standard data. A hunters also have a Boredom coutner. Each time they detect ghostly evidence, reset the timer to BORING_MAX. Each time they take an action other than detecting ghostly evidence, decrease the timer by 1. If the timer is <= 0, they exit the thread. If a hunter finds three diferent types of GHOSTLY evidence it can predict the ghost type and exits the thread.


<img src="Example Output Images/Screenshot 2023-01-16 104758.png" width ="600">

<img src="Example Output Images/Screenshot 2023-01-16 104820.png" width ="600">
