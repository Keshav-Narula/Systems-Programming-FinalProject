
#include "defs.h"

/*
    Note: While this function is provided, you will likely want
    to modify it to suit your solution. This has been provided
    to give you a starting point to lay out the room connectivity,
    but it is entirely possible that you will want to use different
    data structures to represent the rooms and their connectivity,
    or different function names.

    Not following this function's structure exactly will not result
    in a deduction of points.

    The map that this is recreating is visible in the assignment
    specification, and also available by the original creator here:
    https://steamcommunity.com/sharedfiles/filedetails/?id=2251267947

*/
/*
  Function: populateRooms(BuildingType* building)
  Purpose:  fills up the master room list of the building than connects the rooms, so that they are in each other's respective
  connectRooms list
           in: BuildingType *b
*/
void populateRooms(BuildingType* building) {
    // First, create each room. Perhaps you want to include more data
    // in the init parameters?
    RoomType* van = calloc(1, sizeof(RoomType));
    initRoom(van, "Van");
    RoomType* hallway = calloc(1, sizeof(RoomType));
    initRoom(hallway, "Hallway");
    RoomType* master_bedroom = calloc(1, sizeof(RoomType));
    initRoom(master_bedroom, "Master Bedroom");
    RoomType* boys_bedroom = calloc(1, sizeof(RoomType));
    initRoom(boys_bedroom, "Boy's Bedroom");
    RoomType* bathroom = calloc(1, sizeof(RoomType));
    initRoom(bathroom, "Bathroom");
    RoomType* basement = calloc(1, sizeof(RoomType));
    initRoom(basement, "Basement");
    RoomType* basement_hallway = calloc(1, sizeof(RoomType));
    initRoom(basement_hallway, "Basement Hallway");
    RoomType* right_storage_room = calloc(1, sizeof(RoomType));
    initRoom(right_storage_room, "Right Storage Room");
    RoomType* left_storage_room = calloc(1, sizeof(RoomType));
    initRoom(left_storage_room, "Left Storage Room");
    RoomType* kitchen = calloc(1, sizeof(RoomType));
    initRoom(kitchen, "Kitchen");
    RoomType* living_room = calloc(1, sizeof(RoomType));
    initRoom(living_room, "Living Room");
    RoomType* garage = calloc(1, sizeof(RoomType));
    initRoom(garage, "Garage");
    RoomType* utility_room = calloc(1, sizeof(RoomType));
    initRoom(utility_room, "Utility Room");

    /*
    // Now create a linked list of rooms using RoomNodeType in the Building
    RoomNodeType* van_node = calloc(1, sizeof(RoomNodeType));
    van_node->room = van;
    RoomNodeType* hallway_node = calloc(1, sizeof(RoomNodeType));
    hallway_node->room = hallway;
    RoomNodeType* master_bedroom_node = calloc(1, sizeof(RoomNodeType));
    master_bedroom_node->room = master_bedroom;
    RoomNodeType* boys_bedroom_node = calloc(1, sizeof(RoomNodeType));
    boys_bedroom_node->room = boys_bedroom;
    RoomNodeType* bathroom_node = calloc(1, sizeof(RoomNodeType));
    bathroom_node->room = bathroom;
    RoomNodeType* basement_node = calloc(1, sizeof(RoomNodeType));
    basement_node->room = basement;
    RoomNodeType* basement_hallway_node = calloc(1, sizeof(RoomNodeType));
    basement_hallway_node->room = basement_hallway;
    RoomNodeType* right_storage_room_node = calloc(1, sizeof(RoomNodeType));
    right_storage_room_node->room = right_storage_room;
    RoomNodeType* left_storage_room_node = calloc(1, sizeof(RoomNodeType));
    left_storage_room_node->room = left_storage_room;
    RoomNodeType* kitchen_node = calloc(1, sizeof(RoomNodeType));
    kitchen_node->room = kitchen;
    RoomNodeType* living_room_node = calloc(1, sizeof(RoomNodeType));
    living_room_node->room = living_room;
    RoomNodeType* garage_node = calloc(1, sizeof(RoomNodeType));
    garage_node->room = garage;
    RoomNodeType* utility_room_node = calloc(1, sizeof(RoomNodeType));
    utility_room_node->room = utility_room;

    */

    // Building->rooms might be a linked list structre, or maybe just a node.
    // initRoomList(&building->rooms);

    /*
    appendRoom(building->rooms, van_node);
    appendRoom(building->rooms, hallway_node);
    appendRoom(building->rooms, master_bedroom_node);
    appendRoom(building->rooms, boys_bedroom_node);
    appendRoom(building->rooms, bathroom_node);
    appendRoom(building->rooms, basement_node);
    appendRoom(building->rooms, basement_hallway_node);
    appendRoom(building->rooms, right_storage_room_node);
    appendRoom(building->rooms, left_storage_room_node);
    appendRoom(building->rooms, kitchen_node);
    appendRoom(building->rooms, living_room_node);
    appendRoom(building->rooms, garage_node);
    appendRoom(building->rooms, utility_room_node);
    */

    // Building
    addRoom(&(building->rooms), van);
    addRoom(&(building->rooms), hallway);
    addRoom(&(building->rooms), master_bedroom);
    addRoom(&(building->rooms), boys_bedroom);
    addRoom(&(building->rooms), bathroom);
    addRoom(&(building->rooms), basement);
    addRoom(&(building->rooms), basement_hallway);
    addRoom(&(building->rooms), right_storage_room);
    addRoom(&(building->rooms), left_storage_room);
    addRoom(&(building->rooms), kitchen);
    addRoom(&(building->rooms), living_room);
    addRoom(&(building->rooms), garage);
    addRoom(&(building->rooms), utility_room);

    // Now connect the rooms. It is possible you do not need a separate
    // function for this, but it is provided to give you a starting point.
    connectRooms(van, hallway);
    connectRooms(hallway, master_bedroom);
    connectRooms(hallway, boys_bedroom);
    connectRooms(hallway, bathroom);
    connectRooms(hallway, kitchen);
    connectRooms(hallway, basement);
    connectRooms(basement, basement_hallway);
    connectRooms(basement_hallway, right_storage_room);
    connectRooms(basement_hallway, left_storage_room);
    connectRooms(kitchen, living_room);
    connectRooms(kitchen, garage);
    connectRooms(garage, utility_room);
}

/*
  Function: initBuilding(BuildingType *b)
  Purpose:  Initializes fields of a new BuildingType, intializing its RoomArray and master ghost list
           out: BuildingType *b
*/
void initBuilding(BuildingType* b) {
    initRoomList(&(b->rooms));
    if (sem_init(&(b->mutex), 0, 1) < 0) {
        printf("semaphore initialization error\n");
    }
    initEvidenceList(&b->evidenceMaster);
}

/*
  Function: printFinalResults(BuildingType* b)
  Purpose: Prints the final results of the program
        in: BuildingType whose data to read from
        Out: FILE* outfile , file to write to

*/
void printFinalResults(BuildingType* b, FILE* outfile) {
    printf("\n\n     SIMULATION FINAL RESULTS\n");
    fprintf(outfile, "\n\n     SIMULATION FINAL RESULTS\n");

    char ghostClassStr[13];
    ghostClassToString(b->ghost.ghostClass, ghostClassStr);
    int hunterWinFlag = FALSE;
    printf("\nGHOST IS OF TYPE: %s\n", ghostClassStr);
    for (int i = 0; i < MAX_HUNTERS; ++i) {
        if (b->hunters[i].boredomCounter <= 0) {
            printf("\nThe hunter %s got bored and left ", b->hunters[i].name);
            fprintf(outfile, "\nThe hunter %s got bored and left ", b->hunters[i].name);
        }
        if (b->hunters[i].fearCounter >= 100) {
            printf("\nThe hunter %s has been scared away ", b->hunters[i].name);
            fprintf(outfile, "\nThe hunter %s has been scared away ", b->hunters[i].name);
        }
        if (identifyGhost(&b->hunters[i], outfile) == FALSE) {
            hunterWinFlag = TRUE;
        }
        printf("\n%s - HUNTER STATS: BOREDOMCOUNTER - %d , FEAR - %d\n", b->hunters[i].name, b->hunters[i].boredomCounter, b->hunters[i].fearCounter);
        fprintf(outfile, "\n%s - HUNTER STATS: BOREDOMCOUNTER - %d , FEAR - %d\n", b->hunters[i].name, b->hunters[i].boredomCounter, b->hunters[i].fearCounter);
    }
    if (hunterWinFlag == TRUE) {
        printf("\n\nTHE HUNTERS WIN!");
        fprintf(outfile,"\n\nTHE HUNTERS WIN!");

    } else {
        printf("\n\nTHE GHOST WON!");
        fprintf(outfile,"\n\nTHE GHOST WON!");
    }
}

/*
  Function: cleanupBuilding(BuildingType* b)
  Purpose: frees memory dynamically allocated for the BuildingType paseed
           in/out: BuildingType whose data to free
*/
void cleanupBuilding(BuildingType* b) {
    RoomNodeType* currNode;
    RoomNodeType* nextNode;
    currNode = b->rooms.head;

    while (currNode != NULL) {
        nextNode = currNode->next;
        cleanupRoomList(currNode->room->connectedRooms);
        cleanupEvidenceList(currNode->room->roomEvidence);
        free(currNode->room->roomEvidence);
        free(currNode->room);
        free(currNode);
        currNode = nextNode;
    }
    for (int i = 0; i < MAX_HUNTERS; ++i) {
        cleanupEvidenceList(b->hunters[i].evidence);
        free(b->hunters[i].evidence);
    }
    cleanupEvidenceData(&b->evidenceMaster);
    cleanupEvidenceList(&b->evidenceMaster);
}
