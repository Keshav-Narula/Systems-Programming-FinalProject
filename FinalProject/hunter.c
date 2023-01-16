#include "defs.h"

/*
  Function: initHunter(HunterType* hunter, int id, char* name, BuildingType* building)
  Purpose: Initializes new Hunter's variables, allocates and initializes new EvidenceList, and sets the other fields to starting values / passed in fields
           in: int id, char* name BuildingType* building - fields
       out: HunterType* hunter, hunter to initialize fields of
*/
void initHunter(HunterType* hunter, int id, char* name, BuildingType* building) {
    hunter->hunterId = id;
    strcpy(hunter->name, name);

    switch (id) {
        case 0:
            hunter->hunterType = EMF;
            break;
        case 1:
            hunter->hunterType = TEMPERATURE;
            break;
        case 2:
            hunter->hunterType = FINGERPRINTS;
            break;
        case 3:
            hunter->hunterType = SOUND;
            break;
    }
    hunter->currRoom = building->rooms.head->room;
    hunter->boredomCounter = BOREDOM_MAX;
    hunter->fearCounter = 0;
    building->rooms.head->room->numHunters++;

    hunter->evidence = calloc(1, sizeof(EvidenceListType));
    initEvidenceList(hunter->evidence);
    hunter->building = building;
    building->rooms.head->room->huntersInRoom[id] = hunter;
    for (int i = 0; i < MAX_HUNTERS; ++i) {
        hunter->possesGhostlyEvidence[i] = FALSE;
    }
}
/*
  Function: shareEvidence(HunterType* hunterGiving, HunterType* hunterReceiving, FILE* outfile)
  Purpose: The parameter hunter giving shares any ghostly evidence types it has obtained to the other hunter, any GHOSTLY evidence types it
  doesnt already contain
       In: HunterType* hunterGiving, Hunter to look evidence in
       In/Out: HunterType* hunterReceiving , hunter to cross check, if it contains that Ghostly evidence it not append to its list
       Out: FILE* outfile , file to write to
*/
void shareEvidence(HunterType* hunterGiving, HunterType* hunterReceiving, FILE* outfile) {
    EvidenceNodeType* currNode;
    currNode = hunterGiving->evidence->head;

    while (currNode != NULL) {
        if (currNode->evidence->isGhostly == TRUE) {
            int type = evidenceClassToInt(currNode->evidence->evidenceClass);
            if (hunterReceiving->possesGhostlyEvidence[type] == FALSE) {
                hunterReceiving->possesGhostlyEvidence[type] = TRUE;
                addEvidence(currNode->evidence, hunterReceiving->evidence);
                char classTypeStr[13];
                evidenceClassToString(currNode->evidence->evidenceClass, classTypeStr);
                printf("\n%s has shared GHOSTLY %s to %s", hunterGiving->name, classTypeStr, hunterReceiving->name);
                fprintf(outfile, "\n%s has shared GHOSTLY %s to %s", hunterGiving->name, classTypeStr, hunterReceiving->name);
            }
        }
        currNode = currNode->next;
    }
}

/*
  Function: hunterCollectEvidence(HunterType* hunter, EvidenceListType* roomEvidence, FILE* outfile)
  Purpose: Function for the hunter collecting evidence option, if the room List contains evidence of the hunter's type it will collect it and append to its Evidence list
  Otherwise the hunter will create a new piece of Non ghostly evidence and append to its evidence list
       In/out: HunterType* hunter, to cross check evidence type and append evidence to its list
       In/out: EvidenceListType* roomEvidence, room evidence list to serach through and remove evidence from
       Out: FILE* outfile , file to write to

*/
void hunterCollectEvidence(HunterType* hunter, EvidenceListType* roomEvidence, FILE* outfile) {
    EvidenceNodeType* currNode;
    EvidenceNodeType* prevNode;

    currNode = roomEvidence->head;
    prevNode = NULL;

    while (currNode != NULL) {
        if (currNode->evidence->evidenceClass == hunter->hunterType) {
            break;
        }
        prevNode = currNode;
        currNode = currNode->next;
    }
    char classTypeStr[13];

    if (currNode == NULL) {
        // Produce your own evidence

        produceNewEvidence(hunter->evidence, &hunter->building->evidenceMaster, hunter->hunterType, FALSE);
        evidenceClassToString(hunter->hunterType, classTypeStr);
        printf("\n%s has collected some RANDOM %s evidence in the %s", hunter->name, classTypeStr, hunter->currRoom->name);
        fprintf(outfile, "\n%s has collected some RANDOM %s evidence in the %s", hunter->name, classTypeStr, hunter->currRoom->name);
        return;
    } else {
        evidenceClassToString(currNode->evidence->evidenceClass, classTypeStr);
        int type = evidenceClassToInt(currNode->evidence->evidenceClass);
        addEvidence(currNode->evidence, hunter->evidence);
        if (currNode->evidence->isGhostly == TRUE) {
            printf("\n%s has collected some GHOSTLY %s evidence in the %s | Value: %f", hunter->name, classTypeStr, hunter->currRoom->name, currNode->evidence->value);
            fprintf(outfile, "\n%s has collected some GHOSTLY %s evidence in the %s | Value: %f", hunter->name, classTypeStr, hunter->currRoom->name, currNode->evidence->value);

            if (hunter->possesGhostlyEvidence[type] == FALSE) {
                hunter->possesGhostlyEvidence[type] = TRUE;
            }
            hunter->boredomCounter = BOREDOM_MAX;
        } else {
            printf("\n%s has collected some STANDARD %s evidence in the %s", hunter->name, classTypeStr, hunter->currRoom->name);
            fprintf(outfile, "\n%s has collected some STANDARD %s evidence in the %s", hunter->name, classTypeStr, hunter->currRoom->name);
        }

        if (prevNode == NULL) {
            roomEvidence->head = currNode->next;
        } else {
            prevNode->next = currNode->next;
        }
        free(currNode);
        roomEvidence->size--;
    }
}
/*
  Function: identifyGhost(HunterType* hunter, FILE* outfile)
  Purpose: Takes in hunter, to search through its evidence list to see if it has collected three different types of evidence to confirm the
  ghost's type
       In: HunterType* hunter, hunter whos evidence to serach through
       Out: FILE* outfile , file to write to
    RETURN: FALSE is the hunter can identify the ghost, TRUE if hte hunter cannot

*/
int identifyGhost(HunterType* hunter, FILE* outfile) {
    int count = 0;
    int emfCheck = FALSE;
    int temperatureCheck = FALSE;
    int fingerprintsCheck = FALSE;
    int soundCheck = FALSE;
    /*
    for (int i = 0; i < MAX_HUNTERS; ++i) {
        if (hunter->possesGhostlyEvidence[i] == TRUE) {
            count++;
            if (i == 0) {
                emfCheck = TRUE;
            } else if (i == 1) {
                temperatureCheck = TRUE;
            } else if (i == 2) {
                fingerprintsCheck = TRUE;
            } else if (i == 3) {
                soundCheck = TRUE;
            }
        }
    }

   */
    EvidenceNodeType* currNode = NULL;

    currNode = hunter->evidence->head;
    if (currNode == NULL) {
        return TRUE;
    }

    while (currNode != NULL) {
        if (currNode->evidence->isGhostly == TRUE) {
            if (currNode->evidence->evidenceClass == EMF && emfCheck == FALSE) {
                emfCheck = TRUE;
                count++;
            }
            if (currNode->evidence->evidenceClass == TEMPERATURE && temperatureCheck == FALSE) {
                temperatureCheck = TRUE;
                count++;
            }
            if (currNode->evidence->evidenceClass == FINGERPRINTS && fingerprintsCheck == FALSE) {
                fingerprintsCheck = TRUE;
                count++;
            }
            if (currNode->evidence->evidenceClass == SOUND && soundCheck == FALSE) {
                soundCheck = TRUE;
                count++;
            }
        }
        if (count >= 3) {
            break;
        }
        currNode = currNode->next;
    }

    if (count == 3) {
        if (emfCheck == TRUE && temperatureCheck == TRUE && fingerprintsCheck == TRUE) {
            printf("\n%s SPECULATES THE GHOST IS OF TYPE : POLTERGEIST", hunter->name);
            fprintf(outfile, "\n%s SPECULATES THE GHOST IS OF TYPE : POLTERGEIST", hunter->name);

        } else if (emfCheck == TRUE && temperatureCheck == TRUE && soundCheck == TRUE) {
            printf("\n%s SPECULATES THE GHOST IS OF TYPE : BANSHEE", hunter->name);
            fprintf(outfile, "\n%s SPECULATES THE GHOST IS OF TYPE : BANSHEE", hunter->name);

        } else if (emfCheck == TRUE && fingerprintsCheck == TRUE && soundCheck == TRUE) {
            printf("\n%s SPECULATES THE GHOST IS OF TYPE : BULLIES", hunter->name);
            fprintf(outfile, "\n%s SPECULATES THE GHOST IS OF TYPE : BULLIES", hunter->name);

        } else if (temperatureCheck == TRUE && fingerprintsCheck == TRUE && soundCheck == TRUE) {
            printf("\n%s SPECULATES THE GHOST IS OF TYPE : PHANTOM", hunter->name);
            fprintf(outfile, "\n%s SPECULATES THE GHOST IS OF TYPE : PHANTOM", hunter->name);
        }
        return FALSE;

    } else {
        return TRUE;
    }
}

/*
  Function: hunterMove(void* hunterArg)
  Purpose:  Hunter thread function, Simulates the hunter's turns/ moves in while loop, based on ghost proximity or random choice
  Print's its actions to terminal and to file output, Thread exits when hunter reaches a boredom counter of 0,  a fear of 99 or has collected
  enough evidence to idenitfy the ghost
           In/Out: HunterType* hunter entity, Initialized ghost to use in the simulation
*/
void* hunterMove(void* hunterArg) {
    HunterType* hunter = (HunterType*)hunterArg;
    FILE* outfile;
    outfile = fopen("output.txt", "a");

    int choice;
    int shareFlag = FALSE;

    while (1) {
        choice = -1;
        shareFlag = FALSE;

        // Trys locking/ is queued to lock its current room
        if (sem_trywait(&(hunter->currRoom->mutex)) >= 0) {
            // If in room with ghost, increases fear counter and resets its boredom counter
            if (hunter->currRoom->ghostInRoom != NULL) {
                hunter->fearCounter++;
                hunter->boredomCounter = BOREDOM_MAX;
            } else {
                // If no ghost present decrements the boredom counter
                hunter->boredomCounter--;
            }
            // Alwats Check first to see if there is another hunter in the room that is possible to share evidence not already obtained with the other hunter
            if (hunter->currRoom->numHunters > 1) {
                for (int i = 0; i < MAX_HUNTERS; i++) {
                    if (hunter->currRoom->huntersInRoom[i] != NULL && i != hunter->hunterId && shareFlag == FALSE) {
                        HunterType* otherHunter = hunter->currRoom->huntersInRoom[i];

                        for (int k = 0; k < MAX_HUNTERS; k++) {
                            if (hunter->possesGhostlyEvidence[k] == TRUE && otherHunter->possesGhostlyEvidence[k] == FALSE) {
                                // SHARE EVIDENCE TO OTHER hunter
                                shareEvidence(hunter, otherHunter, outfile);
                                shareFlag = TRUE;
                                if (identifyGhost(hunter, outfile) == FALSE) {
                                    hunter->currRoom->huntersInRoom[hunter->hunterId] = NULL;
                                    hunter->currRoom->numHunters--;
                                    sem_post(&(hunter->currRoom->mutex));
                                    fclose(outfile);
                                    return 0;
                                }
                                break;
                            }
                        }
                    }
                    if (shareFlag == TRUE) {
                        break;
                    }
                }
            }
            // If no share evidence occured proceeds to randomly pick action of collecting evidence (0) or move rooms (1)
            if (shareFlag == FALSE) {
                choice = randInt(0, 2);
            } else {
                // If share evidence occured skips to next loop
                choice = -1;
                sem_post(&(hunter->currRoom->mutex));
            }
            if (choice == 0) {
                // Do collect evidence
                sem_wait(&(hunter->building->mutex));                                    // Lock building to update master list
                hunterCollectEvidence(hunter, hunter->currRoom->roomEvidence, outfile);  // Collect room evidence or produce new evidence
                sem_post(&(hunter->building->mutex));
                // If hunter can identify ghost, than exits thread
                if (identifyGhost(hunter, outfile) == FALSE) {
                    hunter->currRoom->huntersInRoom[hunter->hunterId] = NULL;
                    hunter->currRoom->numHunters--;
                    sem_post(&(hunter->currRoom->mutex));
                    fclose(outfile);
                    return 0;
                }
                sem_post(&(hunter->currRoom->mutex));

            } else if (choice == 1) {
                // move rooms, update current and next rooms pointers
                int moveValue = randInt(0, hunter->currRoom->connectedRooms->size);  // Which room to move to in connectRooms list
                RoomNodeType* currNode = NULL;
                RoomNodeType* nextNode = NULL;
                RoomType* beforeRoom = hunter->currRoom;

                currNode = hunter->currRoom->connectedRooms->head;
                // Loop to new room
                for (int i = 0; i < moveValue; i++) {
                    nextNode = currNode->next;
                    currNode = nextNode;
                }
                if (sem_trywait(&(currNode->room->mutex)) >= 0) {  // Try lock new room mutex (Cant deadlock)
                    // Update pointers
                    hunter->currRoom->huntersInRoom[hunter->hunterId] = NULL;
                    hunter->currRoom->numHunters--;
                    hunter->currRoom = currNode->room;
                    currNode->room->numHunters++;
                    currNode->room->huntersInRoom[hunter->hunterId] = hunter;
                    printf("\nMOVEMENT - %s has moved to the %s", hunter->name, hunter->currRoom->name);
                    fprintf(outfile, "\nMOVEMENT - %s has moved to the %s", hunter->name, hunter->currRoom->name);
                    // Unlock
                    sem_post(&(beforeRoom->mutex));
                    sem_post(&(currNode->room->mutex));
                } else {
                    sem_post(&(beforeRoom->mutex));
                }
            }
            // Exit thread if the fearCounter exceeds or equals 100
            if (hunter->fearCounter >= MAX_FEAR) {
                printf("\nThe hunter %s has been scared away ", hunter->name);
                fprintf(outfile, "\nThe hunter %s has been scared away ", hunter->name);
                hunter->currRoom->huntersInRoom[hunter->hunterId] = NULL;
                hunter->currRoom->numHunters--;
                sem_post(&(hunter->currRoom->mutex));
                fclose(outfile);
                return 0;
            }
            // Exit thread if the ghost's boredom counter goes past 0
            if (hunter->boredomCounter <= 0) {
                printf("\nThe hunter %s got bored and left ", hunter->name);
                fprintf(outfile, "\nThe hunter %s got bored and left ", hunter->name);
                hunter->currRoom->huntersInRoom[hunter->hunterId] = NULL;
                hunter->currRoom->numHunters--;
                sem_post(&(hunter->currRoom->mutex));
                fclose(outfile);
                return 0;
            }
        }
        usleep(USLEEP_TIME);
    }
    return 0;
}

