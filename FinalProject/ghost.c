#include "defs.h"
/*
  Function: initGhost(GhostType* ghost, RoomListType* roomList, BuildingType* b)
  Purpose:  Initializes fields of a new GhostType
           out: GhostType* ghost , pointer to already allocated ghost
       in/out: RoomListType* roomList, pointer to master room list, used to randomly place ghost in one of the rooms
       out: BuildingType* b , pointer to building to place ghost in
*/
void initGhost(GhostType* ghost, RoomListType* roomList, BuildingType* b) {
    int randomGen = randInt(0, 3);
    FILE* outfile;

    outfile = fopen("output.txt", "a");
    switch (randomGen) {
        case 0:
            ghost->ghostClass = POLTERGEIST;
            break;
        case 1:
            ghost->ghostClass = BANSHEE;
            break;
        case 2:
            ghost->ghostClass = BULLIES;
            break;
        case 3:
            ghost->ghostClass = PHANTOM;
            break;
    }

    randomGen = randInt(1, roomList->size);

    RoomNodeType* currNode;
    RoomNodeType* nextNode;

    currNode = roomList->head;

    for (int i = 0; i < randomGen; i++) {
        nextNode = currNode->next;
        currNode = nextNode;
    }
    ghost->currRoom = currNode->room;
    ghost->currRoom->ghostInRoom = ghost;

    ghost->boredomTimer = BOREDOM_MAX;
    printf("GHOST SPAWNED IN %s", currNode->room->name);
    fprintf(outfile, "GHOST SPAWNED IN %s", currNode->room->name);
    ghost->building = b;
    fclose(outfile);
}

/*
  Function: ghostClassToString(GhostClassType statePass, char* string)
  Purpose:  Takes in a GhostClassType and string as a parameter, the function string
  copies in String format the ghost class type into the string parameter
           In: GhostClassType statePass , GhostClass to format as string
       out: char* string , String variable to copy into
*/
void ghostClassToString(GhostClassType statePass, char* string) {
    switch (statePass) {
        case POLTERGEIST:
            strcpy(string, "POLTERGEIST");
            break;
        case BANSHEE:
            strcpy(string, "BANSHEE");
            break;
        case BULLIES:
            strcpy(string, "BULLIES");
            break;
        case PHANTOM:
            strcpy(string, "PHANTOM");
            break;
    }
}

/*
  Function: ghostMove(void* ghostArg)
  Purpose:  Ghost thread function, Simulates the ghost's turns/ moves in while loop, based on hunter proximity or random choice
  Print's its actions to terminal and to file output, Thread exits when ghost reaches a boredom counter of 0
           In/Out: GhostType* ghost, Initialized ghost to use in the simulation
*/
void* ghostMove(void* ghostArg) {
    GhostType* ghost = (GhostType*)ghostArg;

    FILE* outfile;
    outfile = fopen("output.txt", "a");

    // Picks Evidence Types states that the ghost can produce, based on the ghost type
    EvidenceClassType state1;
    EvidenceClassType state2;
    EvidenceClassType state3;

    if (ghost->ghostClass == POLTERGEIST) {
        state1 = EMF;
        state2 = TEMPERATURE;
        state3 = FINGERPRINTS;
    } else if (ghost->ghostClass == BANSHEE) {
        state1 = EMF;
        state2 = TEMPERATURE;
        state3 = SOUND;
    } else if (ghost->ghostClass == BULLIES) {
        state1 = EMF;
        state2 = FINGERPRINTS;
        state3 = SOUND;
    } else if (ghost->ghostClass == PHANTOM) {
        state1 = TEMPERATURE;
        state2 = FINGERPRINTS;
        state3 = SOUND;
    }

    while (1) {
        int choice = -1;
        // Trys locking/ is queued to lock its current room
        if (sem_wait(&(ghost->currRoom->mutex)) >= 0) {
            // If hunters also in the room, reset Ghost's boredom counter, and make a random move of either doing nothing (0) or leaving evidence
            if (ghost->currRoom->numHunters > 0) {
                ghost->boredomTimer = BOREDOM_MAX;
                choice = randInt(0, 2);
            } else {
                // If no hunter present in current room, decrease boredom timer and make random choice of the previous options, plus moving rooms (2)
                ghost->boredomTimer--;
                choice = randInt(0, 3);
            }
            if (choice == 0) {
                // Do nothing
                // printf("\n THE GHOST DID NOTHING");
                sem_post(&(ghost->currRoom->mutex));

            } else if (choice == 1) {
                // Leave evidence, Updates the current room list and master list in the building
                EvidenceClassType statePass;
                int evidenceValue = randInt(1, 4);
                if (evidenceValue == 1) {
                    statePass = state1;
                } else if (evidenceValue == 2) {
                    statePass = state2;

                } else if (evidenceValue == 3) {
                    statePass = state3;
                }
                if (sem_wait(&(ghost->building->mutex)) >= 0) {  // Lock building to update master list
                    char classTypeStr[13];
                    evidenceClassToString(statePass, classTypeStr);
                    // If the ghost produced ghostly evidence
                    if (produceNewEvidence(ghost->currRoom->roomEvidence, &ghost->building->evidenceMaster, statePass, TRUE) == TRUE) {
                        printf("\nThe GHOST has left GHOSTLY %s evidence in the %s", classTypeStr, ghost->currRoom->name);
                        fprintf(outfile, "\nThe GHOST has left GHOSTLY %s evidence in the %s", classTypeStr, ghost->currRoom->name);

                    } else {  // If the ghost produced standard evidence
                        printf("\nThe GHOST has left STANDARD %s evidence in the %s", classTypeStr, ghost->currRoom->name);
                        fprintf(outfile, "\nThe GHOST has left STANDARD %s evidence in the %s", classTypeStr, ghost->currRoom->name);
                    }
                    // Unlock building and current room
                    sem_post(&(ghost->building->mutex));
                    sem_post(&(ghost->currRoom->mutex));
                } else {
                    printf("Semphore LOCKING ERROR- GHOST PRODUCE EVIDENCE");
                    sem_post(&(ghost->currRoom->mutex));
                }

            } else if (choice == 2) {
                // move rooms, update ghost's current room, and before and after room's Ghost pointer's
                int moveValue = randInt(0, ghost->currRoom->connectedRooms->size);  // Which room to move to in connectRooms list
                RoomNodeType* currNode = NULL;
                RoomNodeType* nextNode = NULL;
                RoomType* beforeRoom = ghost->currRoom;
                currNode = ghost->currRoom->connectedRooms->head;
                for (int i = 0; i < moveValue; i++) {
                    nextNode = currNode->next;
                    currNode = nextNode;
                }
                if (sem_trywait(&(currNode->room->mutex)) >= 0) {  // Try lock new room mutex (Cant deadlock)
                    // Update pointers
                    ghost->currRoom->ghostInRoom = NULL;
                    ghost->currRoom = currNode->room;
                    ghost->currRoom->ghostInRoom = ghost;
                    printf("\nMOVEMENT - The GHOST has moved to the %s", ghost->currRoom->name);
                    fprintf(outfile, "\nMOVEMENT - The GHOST has moved to the %s", ghost->currRoom->name);
                    // Unlock
                    sem_post(&(beforeRoom->mutex));
                    sem_post(&(currNode->room->mutex));
                } else {
                    sem_post(&(beforeRoom->mutex));
                }
            }
            // Exit thread if the ghost's boredom counter goes past 0
            if (ghost->boredomTimer <= 0) {
                printf("\nThe GHOST got bored and left");
                fprintf(outfile, "\nThe GHOST got bored and left");
                ghost->currRoom->ghostInRoom = NULL;
                sem_post(&(ghost->currRoom->mutex));
                fclose(outfile);

                return 0;
            }
        }
        usleep(USLEEP_TIME);
    }
    return 0;
}

