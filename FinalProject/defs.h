#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef enum { EMF,
               TEMPERATURE,
               FINGERPRINTS,
               SOUND } EvidenceClassType;

typedef enum { POLTERGEIST,
               BANSHEE,
               BULLIES,
               PHANTOM } GhostClassType;
               
#define MAX_STR 64
#define FEAR_RATE 1
#define MAX_FEAR 20
#define MAX_HUNTERS 4
#define USLEEP_TIME 50000
#define BOREDOM_MAX 20
#define TRUE 1
#define FALSE 0

typedef struct Node {
    EvidenceClassType evidenceClass;
    float value;
    int isGhostly;

} EvidenceType;

typedef struct EvidenceNode {
    EvidenceType* evidence;
    struct EvidenceNode* next;
} EvidenceNodeType;

typedef struct EvidenceList {  // REMOVE
    int size;
    EvidenceNodeType* head;
} EvidenceListType;

typedef struct {
    EvidenceListType* evidence;
    int hunterId;
    char name[MAX_STR];
    EvidenceClassType hunterType;
    struct Room* currRoom;
    int boredomCounter;
    int fearCounter;
    struct Building* building;
    int possesGhostlyEvidence[MAX_HUNTERS];
} HunterType;

typedef struct {
    GhostClassType ghostClass;
    struct Room* currRoom;
    int boredomTimer;
    struct Building* building;
} GhostType;

typedef struct Room {
    char name[MAX_STR];
    struct RoomList* connectedRooms;
    int numConnectedRooms;
    int numHunters;
    HunterType* huntersInRoom[MAX_HUNTERS];
    GhostType* ghostInRoom;
    EvidenceListType* roomEvidence;
    sem_t mutex;
} RoomType;

typedef struct RoomNode {
    RoomType* room;
    struct RoomNode* next;
} RoomNodeType;

typedef struct RoomList {
    int size;
    RoomNodeType* head;
} RoomListType;

typedef struct Building {
    sem_t mutex;
    RoomListType rooms;
    GhostType ghost;
    HunterType hunters[MAX_HUNTERS];
    EvidenceListType evidenceMaster;
} BuildingType;

// main.c
int randInt(int, int);          // Generates a pseudorandom integer between the parameters
float randFloat(float, float);  // Generates a pseudorandom float between the parameters
// Building.c
void populateRooms(BuildingType*);  // Populates the building with sampl
void initBuilding(BuildingType* b);
void printFinalResults(BuildingType* b, FILE* outfile);
void cleanupBuilding(BuildingType* b);

// room.c
void initRoomList(RoomListType* list);
void initRoom(RoomType* room, char* name);
// void appendRoom(RoomListType* list, RoomNodeType* room);
void addRoom(RoomListType* list, RoomType* room);
void connectRooms(RoomType* roomA, RoomType* roomB);
void printRoomList(RoomListType* list, char* roomName);
void printMap(BuildingType* b);
void cleanupRoomList(RoomListType* roomlist);
void cleanupRoomData(RoomListType* roomlist);

// ghost.c
void initGhost(GhostType* ghost, RoomListType* roomList, BuildingType* b);
void* ghostMove(void* ghost);
void ghostClassToString(GhostClassType statePass, char* string);

// evidence.c
void initEvidenceList(EvidenceListType* list);
int produceNewEvidence(EvidenceListType* list, EvidenceListType* buildingList, EvidenceClassType classType, int ghostly);
void initEvidence(EvidenceType* evidence, EvidenceClassType classType, float initValue, int ghostly);
void addEvidence(EvidenceType* initEvidence, EvidenceListType* list);
void evidenceClassToString(EvidenceClassType statePass, char* string);
void printRoomEvidence(RoomType* room);
void printEvidence(EvidenceType* evidence);
int evidenceClassToInt(EvidenceClassType statePass);
void cleanupEvidenceList(EvidenceListType* evidenceList);
void cleanupEvidenceData(EvidenceListType* evidenceList);

// hunter.c
void initHunter(HunterType* hunter, int id, char* name, BuildingType* building);
void hunterCollectEvidence(HunterType* hunter, EvidenceListType* roomEvidence, FILE* outfile);
int identifyGhost(HunterType* hunter, FILE* outfile);
void shareEvidence(HunterType* hunterGiving, HunterType* hunterReceiving, FILE* outfile);
void* hunterMove(void* hunterArg);

