/**
 *@Credit: Eric Yuan Changcheng 116010276 CUHK(SZ)
 *@Course: CSC4005: Distributed and PC A3
 * Note that this is the sequential N-body simulation
 * This file is for demo, complie thisfile alone on the cluster
 */

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

static int roomSize, roomTmp, Flag, limit, sUP;
double ** tmpMap;
double ** instanMap;

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

int main(int argc, char **argv)
{
    int i, stableFlag = 1, cnt = 0;
    roomSize = atoi(argv[1]);
    roomTmp = atoi(argv[2]);
    limit = atoi(argv[3]);
    tmpMap = InitRoom(roomSize, roomTmp);
    instanMap = InitRoom(roomSize, roomTmp);
    sUP = 50;

    // Start counting time
    double totaltime;
    struct timeval t_val, t_val_end, t_result;
    gettimeofday(&t_val, NULL);

    do {
        if (cnt % 2 == 0) {
            for (i = 1; i < roomSize - 1; i++) {
                stableFlag = stableFlag & ComputeTemp(tmpMap, instanMap, i, roomSize);
            }
        } else {
            for (i = 1; i < roomSize - 1; i++) {
                stableFlag = stableFlag & ComputeTemp(instanMap, tmpMap, i, roomSize);
            }
        }
        cnt++;
    } while (cnt < limit);

    //end counting time
    gettimeofday(&t_val_end, NULL);
    timersub(&t_val_end, &t_val, &t_result);
    totaltime = t_result.tv_sec + (1.0 * t_result.tv_usec)/1000000;
    printf("total time : %f\n", totaltime);

    /* record the time log in a txt file */
    FILE *log_file_p;
    char file_name[100];
    sprintf(file_name, "data/SEQ_r%d_i%d_l%d.txt", roomSize, roomTmp, limit);
    log_file_p = fopen(file_name, "a");  // add a new log
    fprintf(log_file_p, "%f\n", totaltime);
    fclose(log_file_p);
    /* recording ends */

    printf("%s - cnt : %d\n", "end", cnt);
    return 0;
}
