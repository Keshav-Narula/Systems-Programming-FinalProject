#include "defs.h"

int main(int argc, char* argv[]) {
    // Initialize a random seed for the random number generators
    srand(time(NULL));
    pthread_t hunterThreads[MAX_HUNTERS];
    pthread_t ghostThread;
    FILE* outfile;

    // Open the file for writing only
    outfile = fopen("output.txt", "w");
    fclose(outfile);

    // You may change this code; this is for demonstration purposes
    BuildingType building;

    initBuilding(&building);
    populateRooms(&building);
    // printMap(&building);

    initGhost(&(building.ghost), &(building.rooms), &building);

    for (int i = 0; i < MAX_HUNTERS; i++) {
        char name[MAX_STR];
        printf("\nEnter Hunter %d Name : ", i);
        scanf("%s", name);
        initHunter(building.hunters + i, i, name, &building);
    }

    // initHunter(building.hunters + 0, 0, "Abby1", &building);
    // initHunter(building.hunters + 1, 1, "Bobby2", &building);
    // initHunter(building.hunters + 2, 2, "Sandy3", &building);
    // initHunter(building.hunters + 3, 3, "Patrick4", &building);

    pthread_create(&ghostThread, NULL, ghostMove, &building.ghost);

    pthread_create(&hunterThreads[0], NULL, hunterMove, building.hunters + 0);
    pthread_create(&hunterThreads[1], NULL, hunterMove, building.hunters + 1);
    pthread_create(&hunterThreads[2], NULL, hunterMove, building.hunters + 2);
    pthread_create(&hunterThreads[3], NULL, hunterMove, building.hunters + 3);

    pthread_join(ghostThread, NULL);

    pthread_join(hunterThreads[0], NULL);
    pthread_join(hunterThreads[1], NULL);
    pthread_join(hunterThreads[2], NULL);
    pthread_join(hunterThreads[3], NULL);

    // Print rooms
    /*
    RoomNodeType* currNode;
    RoomNodeType* nextNode;
    currNode = building.rooms.head;
    while (currNode != NULL) {
        nextNode = currNode->next;
        printRoomEvidence(currNode->room);
        currNode = nextNode;
    }
    */
    outfile = fopen("output.txt", "a");
    printFinalResults(&building, outfile);
    fclose(outfile);

    cleanupBuilding(&building);
    return 0;
}

/*
  Function:  randInt
  Purpose:   returns a pseudo randomly generated number,
             in the range min to (max - 1), inclusively
       in:   upper end of the range of the generated number
   return:   randomly generated integer in the range [min, max-1)
*/
int randInt(int min, int max) {
    return rand() % (max - min) + min;
}

/*
  Function:  randFloat
  Purpose:   returns a pseudo randomly generated number,
             in the range min to max, inclusively
       in:   upper end of the range of thFe generated number
   return:   randomly generated integer in the range [0, max-1)
*/
float randFloat(float a, float b) {
    // Get a percentage between rand() and the maximum
    float random = ((float)rand()) / (float)RAND_MAX;
    // Scale it to the range we want, and shift it
    return random * (b - a) + a;
}

