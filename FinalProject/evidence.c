#include "defs.h"
/*
  Function: initEvidenceList(EvidenceListType* list)
  Purpose:  Initializes fields of a new EvidenceListType to its default values (i.e set head to NULL and size to 0)
           out: EvidenceListType* list
*/
void initEvidenceList(EvidenceListType* list) {
    list->head = NULL;
    list->size = 0;
}

/*
  Function: produceNewEvidence(EvidenceListType* list, EvidenceListType* buildingList, EvidenceClassType classType, int ghostly)
  Purpose: Allocates, intializes the values of a new evidence variable and adds it to room/hunter passed in and master building evidence list
           Out: EvidenceListType* list, room / hunter list passed in to add new evidence node to
       Out: EvidenceListType* buildingList , master evidence list to add new evidence nod to
       In: EvidenceClassType classType, class of evidence to produce
       In: int ghostly, if its True, can possible produce ghostly evidence, is FALSE it will not produce ghostly evidence
       RETURN: TRUE, if Ghostly evidence was produced, FALSE otherwise
*/
int produceNewEvidence(EvidenceListType* list, EvidenceListType* buildingList, EvidenceClassType classType, int ghostly) {
    EvidenceType* newEvidence = calloc(1, sizeof(EvidenceType));
    float evidenceValue;
    int isGhostly;
    // Generates fields for evidence, based on EvidenceClassType and ghostly passed in
    switch (classType) {
        case EMF:
            if (ghostly == TRUE) {
                evidenceValue = randFloat(4.70, 6.0);
            } else {
                evidenceValue = randFloat(0.0, 5.90);
            }
            if (evidenceValue > 4.9 && ghostly == TRUE) {
                isGhostly = TRUE;
            } else {
                isGhostly = FALSE;
            }
            break;
        case TEMPERATURE:
            if (ghostly == TRUE) {
                evidenceValue = randFloat(-10.0, 2);
            } else {
                evidenceValue = randFloat(0.0, 28.0);
            }
            if (evidenceValue < 0.0 && ghostly == TRUE) {
                isGhostly = TRUE;
            } else {
                isGhostly = FALSE;
            }
            break;
        case FINGERPRINTS:
            if (ghostly == TRUE) {
                evidenceValue = 1.00;
                isGhostly = TRUE;
            } else {
                evidenceValue = 0.0;
                isGhostly = FALSE;
            }
            break;
        case SOUND:
            if (ghostly == TRUE) {
                evidenceValue = randFloat(65.0, 76.0);
            } else {
                evidenceValue = randFloat(40.0, 71.0);
            }

            if (evidenceValue > 70.0 && ghostly == TRUE) {
                isGhostly = TRUE;
            } else {
                isGhostly = FALSE;
            }
            break;
    }
    // Initialize new evidence with generated fields and add them to the respective lists
    initEvidence(newEvidence, classType, evidenceValue, isGhostly);
    addEvidence(newEvidence, buildingList);
    addEvidence(newEvidence, list);
    // Return
    return isGhostly;
}

/*
  Function: initEvidence(EvidenceType* evidence, EvidenceClassType classType, float initValue, int ghostly)
  Purpose:  Intializes fields for allocated EvidenceType, with the fields passed in
           Out: EvidenceType* evidence , pointer to allocated evidence to initialize fields
            In: EvidenceClassType classType, float initValue, int ghostly - FIELDS
*/
void initEvidence(EvidenceType* evidence, EvidenceClassType classType, float initValue, int ghostly) {
    evidence->evidenceClass = classType;
    evidence->value = initValue;
    evidence->isGhostly = ghostly;
}
/*
  Function: addEvidence(EvidenceType* initEvidence, EvidenceListType* list)
  Purpose: Adds passed in allocated and initialized evidence, into new allocated node, to the end of the passed in evidence linked list
           in/out: EvidenceListType* list, evidence linked list structure to traverse through and track nodes and update pointers
       in: EvidenceType* initEvidence, evidenceType varaible whose data will be pointed to in new Node
*/
void addEvidence(EvidenceType* initEvidence, EvidenceListType* list) {
    EvidenceNodeType* currNode = NULL;
    EvidenceNodeType* prevNode = NULL;
    EvidenceNodeType* newNode = NULL;

    newNode = calloc(1, sizeof(EvidenceNodeType));
    newNode->evidence = initEvidence;
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
  Function: evidenceClassToString(EvidenceClassType statePass, char* string)
  Purpose:  Takes in a EvidenceClassType and string as a parameter, the function string
  copies in String format the EvidenceClassType into the string parameter
           In: EvidenceClassType statePass , EvidenceClassType to format as string
       out: char* string , String variable to copy into
*/
void evidenceClassToString(EvidenceClassType statePass, char* string) {
    switch (statePass) {
        case EMF:
            strcpy(string, "EMF");
            break;
        case TEMPERATURE:
            strcpy(string, "TEMPERATURE");
            break;
        case FINGERPRINTS:
            strcpy(string, "FINGERPRINTS");
            break;
        case SOUND:
            strcpy(string, "SOUND");
            break;
    }
}
/*
  Function: int evidenceClassToInt(EvidenceClassType statePass)
  Purpose:  Takes in a EvidenceClassType and returns the integer value / postion it represents in the typedef enum EvidenceClassType
           In: EvidenceClassType statePass , EvidenceClassType to format as int
       RETURN: Int - 0,1,2, or 3
*/
int evidenceClassToInt(EvidenceClassType statePass) {
    switch (statePass) {
        case EMF:
            return 0;
        case TEMPERATURE:
            return 1;
        case FINGERPRINTS:
            return 2;
        case SOUND:
            return 3;
    }
    return 0;
}

/*
  Function: cleanupGhostList(GhostListType* list)
  Purpose: De allocates data for nodes of a passed in Evidence list, by traversing through linked list
	   in/out: EvidenceListType* evidenceList , evidence List to free nodes from
*/
void cleanupEvidenceList(EvidenceListType* evidenceList) {
    EvidenceNodeType* currNode;
    EvidenceNodeType* nextNode;
    currNode = evidenceList->head;

    while (currNode != NULL) {
        nextNode = currNode->next;
        free(currNode);
        currNode = nextNode;
    }
    // free(evidenceList);
}

/*
  Function: cleanupEvidenceData(EvidenceListType* evidenceList)
  Purpose:  De allocates the data fields of passed in evidence list, by traversing nodes of linked list
   (called if nothing else is pointing to the data)
	   in/out:EvidenceListType* evidenceList , evidence list to de allocate data fields being pointed to 
*/
void cleanupEvidenceData(EvidenceListType* evidenceList) {
    EvidenceNodeType* currNode;
    EvidenceNodeType* nextNode;
    currNode = evidenceList->head;

    while (currNode != NULL) {
        nextNode = currNode->next;
        free(currNode->evidence);
        currNode = nextNode;
    }
}


// TESTING FUNCTIONS (NOT USED / Commented out)

/*
  Function: printRoomEvidence(RoomType* room)
  Purpose: Loops through and prints the evidence in the evidence list of the roomType passed in
    IN: RoomType* room , pointer to the room to print evidence list
*/
void printRoomEvidence(RoomType* room) {
    printf("\n\n THE ROOM IS %s, evidence list size is %d :\n", room->name, room->roomEvidence->size);

    EvidenceNodeType* currNode;
    EvidenceNodeType* nextNode;
    currNode = room->roomEvidence->head;
    if (currNode == NULL) {
        printf("\n-->%7s", "EMPTY");
        return;
    }

    while (currNode != NULL) {
        nextNode = currNode->next;
        printEvidence(currNode->evidence);
        currNode = nextNode;
    }
}

/*
  Function: printEvidence(EvidenceType* evidence) , 
  Purpose: Prints passed in evidence, with formatting
    In: EvidenceType* evidence , Pointer to evidence to print
*/
void printEvidence(EvidenceType* evidence) {
    char isGhostlyStr[4];
    if (evidence->isGhostly == TRUE) {
        strcpy(isGhostlyStr, "YES");
    } else {
        strcpy(isGhostlyStr, "NO");
    }
    char evidenceClassStr[13];
    evidenceClassToString(evidence->evidenceClass, evidenceClassStr);
    printf("\nValue: %f  | Class: %s | Ghostly: %s", evidence->value, evidenceClassStr, isGhostlyStr);
}

