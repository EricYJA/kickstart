
/*
 * Note that this is the final version
 */

#include "mpi.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

int swap(int *num, int i, int j);

int main (int argc, char ** argv) {
    int data_in[200000];
	int size = sizeof(data_in) / sizeof(data_in[0]);
	int revFlag = 0, local_n, recv_right, recv_left;
	int *data_out, *mykeys; //  , *data_in;
	int numtasks, rank, len, max, addFlag = 0;
	char hostname[MPI_MAX_PROCESSOR_NAME];
	double start, finish;
	double totaltime;
	MPI_Request request;
	MPI_Status status;

	MPI_Init(&argc, &argv); // initialize MPI
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // get number of tasks
	MPI_Comm_rank(MPI_COMM_WORLD, &rank); // get my rank
	MPI_Get_processor_name(hostname, &len); // this one is obvious

	//data_in = (int *)malloc(size * sizeof(int)); /*�������������еĿռ�*/
	//int data_in[16];
	if (rank == 0) {
		//data_in = (int *)malloc(size * sizeof(int)); /*�������������еĿռ�*/
		srand(48365);
		printf("Random Numbers Generating: \n");
		for (int i = 0; i < size; i++) {
			data_in[i] = ((int)rand()) % 10000; /*������������������*/
			//printf("%d ", data_in[i]);
		}
		printf("\n");
	}

	 //printf ("Number of tasks= %d My rank= %d Running on %s\n",
	 //        numtasks,rank,hostname);

	/* setting up all the preset variables */
	start = MPI_Wtime();

    local_n = size / numtasks;
    if (size % numtasks != 0) {
        local_n++;
        addFlag = 1;

        if (rank == 0) {
            int j;
            max = data_in[0];
            printf("find %d, %d\n", j, (int) max);
            for (j = 0; j < size; j++) {
                if (max < data_in[j]) {
                    max = data_in[j];
                }
            }

            MPI_Send(&max, 1, MPI_INT, numtasks - 1,
                0, MPI_COMM_WORLD);
        } else if (rank == numtasks - 1) {
            MPI_Recv(&max, 1, MPI_INT, 0,
                0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }


        if (size / numtasks == 1) {
            MPI_Finalize();
            perror("Unproper number of procssors!!!");
            exit(1);
        }
    }

    if (local_n % 2 != 0) {
        revFlag = 1;
    }

    int leftRank = rank;
    int rightRank = rank;
    if (rank != 0) {
        leftRank--;
    }
    if (rank + 1 != numtasks) {
        rightRank++;
    }

    mykeys = (int *)malloc(sizeof(int) * local_n);

    /* setting up ends */

    //sending out the spilted message to each process
    MPI_Scatter(&data_in, local_n, MPI_INT, mykeys, local_n, MPI_INT,
        0, MPI_COMM_WORLD);

    if (addFlag == 1 && rank == numtasks - 1) {
        for (int k = 0; k < numtasks - (size % numtasks); k++) {
            printf("%d, %d\n", local_n - 1 - k, max);
            mykeys[local_n - 1 - k] = max;
        }
    }

    /* ranking starts */
    int flag = 0;
    int m;

    // this is for rank when each block is even
    if (revFlag == 0) {
        for (m = 0; m < size; m++) {
            flag = 0;

            if (m % 2 == 0) {
                for (int i = 0; i < local_n; i += 2) {
                    if (mykeys[i] > mykeys[i + 1]) {
                        swap(mykeys, i, i + 1);
                        flag = 1;
                    }
                }
            } else {
                if (rank != numtasks - 1) {
					MPI_Send(&mykeys[local_n - 1], 1, MPI_INT, rightRank,
						0, MPI_COMM_WORLD);
				}

				if (rank != 0) {
					MPI_Recv(&recv_left, 1, MPI_INT, leftRank,
						0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

				if (rank != 0) {
					if (recv_left > mykeys[0]) {
						int tmp = mykeys[0];
						mykeys[0] = recv_left;
						recv_left = tmp;
					}
				}

				/* local sort starts*/

				if (1 < local_n - 1) {
					for (int i = 1; i < local_n - 1; i += 2) {
						if (mykeys[i] > mykeys[i + 1]) {
							swap(mykeys, i, i + 1);
							flag = 1;
						}
					}
				}

				/* local sort ends*/

				if (leftRank != rank) {
					MPI_Send(&recv_left, 1, MPI_INT, leftRank,
						0, MPI_COMM_WORLD);
				}

				if (rank != rightRank) {
					MPI_Recv(&recv_right, 1, MPI_INT, rightRank,
						0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}

				if (rank != numtasks - 1) {
					if (recv_right < mykeys[local_n - 1]) {
						mykeys[local_n - 1] = recv_right;
						flag = 1;
					}
				}

            }
        }
    }

    // this is for rank when each block is odd
    else {
        int count = 0;

        while (count < size) {

            if (1 < local_n) {
                if ((count % 2) == ((int) (rank % 2))) {
                        for (int m = 0; m + 1 < local_n; m += 2) {
                            if (mykeys[m] > mykeys[m + 1]) {
                                swap(mykeys, m, m + 1);
                            }
                        }
                } else {
                        for (int n = 1; n < local_n; n += 2) {
                            if (mykeys[n] > mykeys[n + 1]) {
                                swap(mykeys, n, n + 1);
                            }
                        }
                }
            }

            /* transfering information*/
            if (rank != numtasks - 1) {
                MPI_Send(&mykeys[local_n - 1], 1, MPI_INT, rightRank,
                    0, MPI_COMM_WORLD);
            }

            if (rank != 0) {
                MPI_Recv(&recv_left, 1, MPI_INT, leftRank,
                    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //printf("%d --recleft-> %d : %d \n", leftRank, rank, recv_left);
            }

            if (count % 2 == 0) {
                if (rank % 2 == 1 && recv_left > mykeys[0]) {
                    int tmp = recv_left;
                    recv_left = mykeys[0];
                    mykeys[0] = tmp;
                }
            } else {
                if (rank % 2 == 0 && rank != 0 && recv_left > mykeys[0]) {
                    int tmp = recv_left;
                    recv_left = mykeys[0];
                    mykeys[0] = tmp;
                }
            }

            if (leftRank != rank) {
                MPI_Send(&recv_left, 1, MPI_INT, leftRank,
                    0, MPI_COMM_WORLD);
            }

            if (rank != rightRank) {
                MPI_Recv(&recv_right, 1, MPI_INT, rightRank,
                    0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                //printf("%d --recright-> %d : %d\n", rightRank, rank, recv_right);
            }

            /* transfering ends */

            if (count % 2 == 0) {
                if (rank % 2 == 0 && rank != numtasks - 1
                    && recv_right < mykeys[local_n - 1]) {
                        mykeys[local_n - 1] = recv_right;
                }
            } else {
                if (rank % 2 == 1 && rank != numtasks - 1
                    && recv_right < mykeys[local_n - 1]) {
                        mykeys[local_n - 1] = recv_right;
                    }
            }

            count++;
        }
    }

    /* ranking ends */

    // int i;
    // printf("Rank %d outputs ", rank);
    // for (i = 0; i < local_n; i++){
    //     printf("%d ", mykeys[i]);
    // }
    // printf("\n");

    data_out = (int *)malloc(sizeof(int) * size);
    MPI_Gather(mykeys, local_n, MPI_INT,
        data_out, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    finish = MPI_Wtime();

	if (rank == 0) {
		totaltime = (double) (finish - start);
		// int i;
		// printf("\n");
		// for (i = 0; i < size; i++) {
		// 	printf("%d ", data_out[i]);
		// }
		printf("total time : %f\n", totaltime);
	}


    MPI_Finalize();// done with MPI

    return 0;
}

int swap(int *num, int i, int j) {
    int tmp = num[i];
    num[i] = num[j];
    num[j] = tmp;

    return 0;
}
