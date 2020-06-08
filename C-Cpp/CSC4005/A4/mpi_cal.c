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
#include <mpi.h>
#include <sys/time.h>

#define RESN 100
#define WALLTMP 20
#define FIREPLACETMP 100
#define ERRORS 2

typedef enum { DATA_TAG, TERM_TAG, RESULT_TAG} Tags;

double ComputeTemp(double ** tmpMap, double ** localMap, int row, int roomSize)
{
    int j;
    double diff = 0.0;
    for (j = 1; j < roomSize - 1; j++) {
        localMap[row][j] = 0.25 * (tmpMap[row - 1][j] + tmpMap[row][j - 1]
            + tmpMap[row + 1][j] + tmpMap[row][j + 1]);
        diff = diff + pow((localMap[row][j] - tmpMap[row][j]), 2);
    }
    return diff;
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

void minions(int rank, int size)
{
    int roomSize, start, work, i;
    double diff = 0.0;
    double ** localMap;
    double ** tmpMap;
    MPI_Status status;
    MPI_Recv(&roomSize, 1, MPI_INT, 0, DATA_TAG, MPI_COMM_WORLD, &status);
    work = (roomSize - 2) / (size - 1) + 1;
    start = (rank - 1) * work + 1;
    localMap = InitRoom(roomSize, 0);
    tmpMap = InitRoom(roomSize, 0);

    if (rank == size - 1) {
        work = (roomSize - 2) - (work)*(size - 2);
    }

    printf("%d------\n", roomSize);
    printf("%d - %d\n", start, work);

    do {
        MPI_Recv(&tmpMap[0][0], roomSize * roomSize, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        diff = 0.0;

        for (i = 0; i < work; i++) {
            diff = diff + ComputeTemp(tmpMap, localMap, i + start, roomSize);
        }

        MPI_Send(&localMap[0][0], roomSize * roomSize, MPI_DOUBLE, 0, RESULT_TAG, MPI_COMM_WORLD);
        MPI_Send(&diff, 1, MPI_DOUBLE, 0, DATA_TAG, MPI_COMM_WORLD);
    } while (status.MPI_TAG != TERM_TAG);
}

int main(int argc, char **argv)
{
    /* main MPI beigins */
    int rank, size, row, i, working;
    MPI_Status status;

    if(MPI_Init(&argc,&argv) != MPI_SUCCESS)
    {
        perror("Unable to initialize MPI\n");
        exit(1);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank != 0) {
        minions(rank, size);
    } else {
        double ** tmpMap;
        double ** instanMap;
        int cnt = 0, limit = 0, i, j, work, start, roomSize, roomTmp, sUP;
        double tmpdiff = 0.0, diff = 0.0;

        roomSize = atoi(argv[1]);
        roomTmp = atoi(argv[2]);
        limit = atoi(argv[3]);
        tmpMap = InitRoom(roomSize, roomTmp);
        instanMap = InitRoom(roomSize, roomTmp);
        sUP = 10;

        printf("%d\n", limit);
        // Start counting time
        double totaltime;
        struct timeval t_val, t_val_end, t_result;


        for (i = 1; i < size; i++) {
            MPI_Send(&roomSize, 1, MPI_INT, i, DATA_TAG, MPI_COMM_WORLD);
        }

        while (1) {
            if (sqrt(diff) < ERRORS && limit == -1 && cnt > 1) limit = cnt + 1;

            // send the termination signal
            for (i = 1; i < size; i++) {
                if (cnt == limit - 1) {
                    MPI_Send(&tmpMap[0][0], roomSize * roomSize, MPI_DOUBLE, i, TERM_TAG, MPI_COMM_WORLD);
                } else {
                    gettimeofday(&t_val, NULL);
                    MPI_Send(&tmpMap[0][0], roomSize * roomSize, MPI_DOUBLE, i, DATA_TAG, MPI_COMM_WORLD);
                    gettimeofday(&t_val_end, NULL);
                    timersub(&t_val_end, &t_val, &t_result);
                    totaltime = t_result.tv_sec + (1.0 * t_result.tv_usec)/1000000;
                    printf("%f\n", totaltime);

                }
            }

            diff = 0;
            for (i = 1; i < size; i++) {
                MPI_Recv(&instanMap[0][0], roomSize * roomSize, MPI_DOUBLE, i, RESULT_TAG, MPI_COMM_WORLD, &status);
                MPI_Recv(&tmpdiff, 1, MPI_DOUBLE, i, DATA_TAG, MPI_COMM_WORLD, &status);
                diff = tmpdiff + diff;

                work = (roomSize - 2) / (size - 1) + 1;
                start = (i - 1) * work + 1;

                if (i == size - 1) {
                    work = (roomSize - 2) - (work)*(size - 2);
                }

                for (int i = start; i < start + work; i++) {
                    for (j = 1; j < roomSize - 1; j++) {
                        tmpMap[i][j] = instanMap[i][j];
                    }
                }
            }

            if (limit != 0) {
                cnt++;
                if (cnt == limit) {
                    break;
                }
            }
        }

        //end counting time


        /* record the time log in a txt file */
        // FILE *log_file_p;
        // char file_name[100];
        // sprintf(file_name, "data/MPI_%d_r%d_i%d_l%d.txt", size, roomSize, roomTmp, limit);
        // log_file_p = fopen(file_name, "a");  // add a new log
        // fprintf(log_file_p, "%f\n", totaltime);
        // fclose(log_file_p);
        /* recording ends */

        printf("%s cnt: %d\n", "end", cnt);
    }

	MPI_Finalize();
    return 0;
}
