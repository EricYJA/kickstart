/**
 *@Credit: Eric Yuan Changcheng 116010276 CUHK(SZ)
 *@Course: CSC4005: Distributed and PC A3
 * Note that this is the sequential N-body simulation
 * This file is for demo, complie thisfile alone on the cluster
 */

#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define RESN 100
#define WALLTMP 20
#define FIREPLACETMP 100
#define ERRORS 1

static int NUM_THREAD;
static int roomSize, roomTmp, Flag, limit, sUP;
double ** tmpMap;
double ** instanMap;
pthread_mutex_t Lock;

int ComputeTemp(double ** tmpMap, double ** localMap, int row, int roomSize)
{
    int j, stableFlag = 1;
    for (j = 1; j < roomSize - 1; j++) {
        localMap[row][j] = 0.25 * (tmpMap[row - 1][j] + tmpMap[row][j - 1]
            + tmpMap[row + 1][j] + tmpMap[row][j + 1]);
        // printf("%f - %d - %d\n", tmpMap[row][j], row, j);
    }
    return stableFlag;
}

double** InitRoom(int roomSize, int initTmp)
{
    double **tmpMap;
    double * storage;
    int i, j;

    tmpMap = (double**)malloc(sizeof(double*)*roomSize); //Allocate the first level
    storage = (double*)malloc(sizeof(double)*roomSize * roomSize);
    for (i = 0; i < roomSize; i++) {
        tmpMap[i] = &storage[i * roomSize]; // Allocate the seconde level
    }

    for (i = 0; i < roomSize; i++) {
        for (j = 0; j < roomSize; j++) {
            if (i == 0 || j == 0 || i == roomSize - 1 || j == roomSize - 1) {
                if (i == 0 && j < 7 * (roomSize / 10) && j > 3 * (roomSize / 10)) {
                    tmpMap[i][j] = FIREPLACETMP;
                } else {
                    tmpMap[i][j] = WALLTMP;
                }
            } else {
                tmpMap[i][j] = initTmp;
            }
        }
    }

    return tmpMap;
}

void * minions(void * t)
{
    int rank = *((int*)&t);
    int size = NUM_THREAD;
    int start, work, i, j, stableFlag = 1, cnt = 0;
    double** localMap = InitRoom(roomSize, roomTmp);
    work = (roomSize) / size;
    start = (rank) * work + 1;

    if (rank == size - 1) {
        work = roomSize - work * (size - 1) - 2;
    }

    printf("%d ------ \n", roomSize);
    printf("%d - %d\n", start, work);

    do {
        for (i = 0; i < work; i++) {
            stableFlag = stableFlag & ComputeTemp(tmpMap, localMap, i + start, roomSize);
        }

        for (int i = start; i < start + work; i++) {
            for (j = 1; j < roomSize - 1; j++) {
                tmpMap[i][j] = localMap[i][j];
            }
        }

        cnt++;
    } while (cnt < limit);
    return 0;
}

int main(int argc, char **argv)
{
    NUM_THREAD = atoi(argv[1]);
    roomSize = atoi(argv[2]);
    roomTmp = atoi(argv[3]);
    limit = atoi(argv[4]);
    tmpMap = InitRoom(roomSize, roomTmp);
    instanMap = InitRoom(roomSize, roomTmp);
    sUP = 1;

    pthread_t threads[NUM_THREAD];
    pthread_mutex_init(&Lock, NULL);

    // Start counting time
    double totaltime;
    struct timeval t_val, t_val_end, t_result;
    gettimeofday(&t_val, NULL);

    long i;
    for(i = 0; i < NUM_THREAD; i++) {
        pthread_create(&threads[i], NULL, minions, (void *)i);
    }

    for(i = 0; i < NUM_THREAD; i++){
        pthread_join(threads[i], NULL);
    }

    //end counting time
    gettimeofday(&t_val_end, NULL);
    timersub(&t_val_end, &t_val, &t_result);
    totaltime = t_result.tv_sec + (1.0 * t_result.tv_usec)/1000000;
    printf("total time : %f\n", totaltime);

    /* record the time log in a txt file */
    FILE *log_file_p;
    char file_name[100];
    sprintf(file_name, "data/Pthread_%d_r%d_i%d_l%d.txt", NUM_THREAD, roomSize, roomTmp, limit);
    log_file_p = fopen(file_name, "a");  // add a new log
    fprintf(log_file_p, "%f\n", totaltime);
    fclose(log_file_p);
    /* recording ends */

    printf("%s\n", "end");
    pthread_mutex_destroy(&Lock); // destroy mutex
    pthread_exit(NULL);
    return 0;
}
