/* 
 * Header of Addons Manager
 * @author ECHOES Technologies (FPO)
 * @date 13/03/2012
 */


#ifndef ADDON_H
#define	ADDON_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "plugin.h"

// Portable sleep
#if defined (WIN32) || defined (WIN64)

#include <windows.h>
#define SLEEP(time) Sleep(time*1000)

#else

#include <unistd.h>
#define SLEEP(time) sleep(time)

#endif

// Element of collected data queue
typedef struct CollectQueueElement CollectQueueElement;
struct CollectQueueElement
{
    unsigned int idPlg, idAsset, idSrc, idSearch, numSubSearch;
    char value[1000];
    time_t time;
    CollectQueueElement *next;
};
// Collected data queue
typedef struct CollectQueue CollectQueue;
struct CollectQueue
{
    pthread_mutex_t mutex;
    CollectQueueElement *first;
};

#define COLLECT_QUEUE_INITIALIZER {PTHREAD_MUTEX_INITIALIZER, NULL}

typedef struct LoopParams LoopParams;
struct LoopParams
{
    pthread_cond_t condition;
    pthread_mutex_t mutex;
    unsigned int idAddon, idType;
    void *params;
};

typedef struct AddonLocationLogParams AddonLocationLogParams;
struct AddonLocationLogParams
{
    unsigned int idPlg, idAsset, idSrc, idSearch, period, staticValues, nbLine, firstChar, length;
    char path[255];
    CollectQueue *collectQueue;
};

typedef struct AddonLocationFileParams AddonLocationFileParams;
struct AddonLocationFileParams
{
    unsigned int idPlg, idAsset, idSrc, idSearch, period, staticValues, line, firstChar, length;
    char path[255];
    CollectQueue *collectQueue;
};

typedef struct AddonsMgrParams AddonsMgrParams;
struct AddonsMgrParams
{
    PlgList plgList;
    pthread_t* addonsThreads;
    LoopParams* loopsParams;
    CollectQueue collectQueue;
};

#define ADDON_PARAMS_INITIALIZER {NULL, NULL, NULL, COLLECT_QUEUE_INITIALIZER}

/**
 * Push a new Element to CollectQueue
 * @param *collectQueue Pointer of Collected Data Queue
 * @param idPlg         Plugin Identifier
 * @param idAsset       Asset Identifier
 * @param idSrc         Source Identifier
 * @param idSearch      Search Identifier
 * @param numSubSearch  Sub-Search Number
 * @param value         Message Value
 * @param time          Time of collect
 * @return Exit status
 */
int pushCollectQueue(CollectQueue *collectQueue, unsigned int idPlg, unsigned int idAsset, unsigned int idSrc, unsigned int idSearch, unsigned int numSubSearch, const char *value, time_t time);

/**
 * Thread - Loop for addon.
 * @param arg Pointer of Loop Parameters 
 */
void *addonLoop(void *arg);

/**
 * Thread - Addon Location File.
 * @param *alfp Pointer of addon location file parameters
 */
void *addonLocationFile(void *arg);

/**
 * Thread - Addon Location Log.
 * @param *allp Pointer of addon location log parameters
 */
void *addonLocationLog(void *arg);

/**
 * Thread - Main function of Addons Manager.
 * @param *nbThreads      Pointer of Number of threads
 * @param *plgList        Pointer of Plugins list with informations
 * @param *addonThreads[] Table of addons threads
 */
//int addon(unsigned int *nbThreads, PlgList *plgList, pthread_t addonsThreads[]);
void *addon(void *arg);

#endif	/* ADDON_H */

