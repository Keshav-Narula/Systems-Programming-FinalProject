#include "defs.h"

/*
  Function: initEvidenceList(EvidenceListType* list)
  Purpose:  Initializes fields of a new EvidenceListType to its default values (i.e set head to NULL and size to 0)
           out: EvidenceListType* list
*/
void initRoomList(RoomListType* list) {
    list->head = NULL;
    list->size = 0;
}
/*
  Function: initRoom(RoomType* room, char* name)
  Purpose: Initializes new roomType's varaibles, allocates new intialized Room List for connectedRooms and new EvidenceList to contain its room
  evidence, and sets the other fields to starting values
	   in: char* name , name to intialize as
*/
void initRoom(RoomType* room, char* name) {
    strcpy(room->name, name);
    room->numHunters = 0;
    room->connectedRooms = calloc(1, sizeof(RoomListType));

    initRoomList(room->connectedRooms);

    for (int i = 0; i < MAX_HUNTERS; ++i) {
        room->huntersInRoom[i] = NULL;
    }
    room->ghostInRoom = NULL;
    room->roomEvidence = calloc(1, sizeof(EvidenceListType));
    initEvidenceList(room->roomEvidence);
    if (sem_init(&(room->mutex), 0, 1) < 0) {
        printf("semaphore room initialization error\n");
    }
}

/*
  Function: addRoom(RoomListType* list, RoomType* room)
  Purpose: Adds passed in allocated and initialized RoomType data, into new allocated RoomTypenode, to the end of the passed in RoomListType linked list
           in/out: RoomListType* list, room linked list structure to traverse through and track nodes and update pointers
       in: RoomType* room, RoomType variable whose data will be pointed to in new Node
*/
void addRoom(RoomListType* list, RoomType* room) {
    RoomNodeType* currNode;
    RoomNodeType* prevNode;
    RoomNodeType* newNode;

    newNode = calloc(1, sizeof(RoomNodeType));
    newNode->room = room;
    newNode->next = NULL;

    currNode = list->head;
    prevNode = NULL;
    list->size++;

    if (currNode == NULL) {
        list->head = newNode;
        return;
    }

    while (currNode != NULL) {
        prevNode = currNode;
        currNode = currNode->next;
    }

    prevNode->next = newNode;
}

/*
  Function: connectRooms(RoomType* roomA, RoomType* roomB)
  Purpose: Adds RoomA to roomB's connectedRoom's linked list and vice versa
       in/out: RoomType* roomA , roomB, RoomType variables to use when creating new Room node and adding to their connected room linked list
*/
void connectRooms(RoomType* roomA, RoomType* roomB) {
    addRoom(roomA->connectedRooms, roomB);
    addRoom(roomB->connectedRooms, roomA);
}

/*
  Function: cleanupRoomList(RoomListType* roomlist)
  Purpose: De allocates data for nodes of a passed in Room list, by traversing through linked list
	   in/out: RoomListType* roomlist , room List to free nodes from
*/
void cleanupRoomList(RoomListType* roomlist) {
    RoomNodeType* currNode;
    RoomNodeType* nextNode;
    currNode = roomlist->head;

    while (currNode != NULL) {
        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
    }
    free(roomlist);
}

/*
  Function: cleanupRoomData(RoomListType* roomlist)
  Purpose:  De allocates the data fields of passed in room list, by traversing nodes of linked list
   (called if nothing else is pointing to the data)
	   in/out:RoomListType* roomlist , room list to de allocate room data fields being pointed to 
*/
void cleanupRoomData(RoomListType* roomlist) {
    RoomNodeType* currNode;
    RoomNodeType* nextNode;
    currNode = roomlist->head;

    while (currNode != NULL) {
        nextNode = currNode->next;
        free(currNode->room);
        currNode = nextNode;
    }
}


// TESTING FUNCTIONS (NOT USED / Commented out)

/*
  Function: printRoomList(RoomListType* list, char* roomName)
  Purpose: Prints formatted name of the room passed and loops and prints the room's connectedRooms linked list
    IN: RoomListType* list , linked list of connected rooms
    IN: char* roomName
*/
void printRoomList(RoomListType* list, char* roomName) {
    RoomNodeType* currNode;
    RoomNodeType* nextNode;
    currNode = list->head;
    printf("\n\n**Room Name: %s -- (%d)\n\n", roomName, list->size);

    while (currNode != NULL) {
        nextNode = currNode->next;
        printf("ROOM: %s\n", currNode->room->name);
        currNode = nextNode;
    }
}

/*
  Function: printMap(BuildingType* b)
  Purpose: Prints all the rooms in the passed in building and their respective connectedRoom's linked list
    IN: BuildingType* b , building varaible to read room list from
*/
void printMap(BuildingType* b) {
    printRoomList(&b->rooms, "BUILDING");
    RoomNodeType* currNode;
    RoomNodeType* nextNode;
    currNode = b->rooms.head;
    while (currNode != NULL) {
        nextNode = currNode->next;
        printRoomList(currNode->room->connectedRooms, currNode->room->name);
        currNode = nextNode;
    }
}



/*
void appendRoom(RoomListType* list, RoomNodeType* room) {
    RoomNodeType* currNode;
    RoomNodeType* prevNode;


    currNode = list->head;
    prevNode = NULL;
    if (currNode == NULL) {
        list->head = newNode;
        return;
    }

    while (currNode != NULL) {
        prevNode = currNode;
        currNode = currNode->next;
    }

    prevNode->next = newNode;
    list->size++;
}
*/

