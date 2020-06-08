/**
 *@Credit: Eric Yuan Changcheng 116010276 CUHK(SZ)
 *@Course: CSC4005: Distributed and PC A3
 * Note that this is the MPI N-body simulation
 * This file is for calculation of time, no display in this program
 * DO NOT RUN this file alone, run it with the run.py
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>
#include <sys/time.h>

#define X_RESN  800       /* x resolution */
#define Y_RESN  800       /* y resolution */
#define X_UPPER 700
#define Y_UPPER 700
#define X_LOWER 100
#define Y_LOWER 100
#define MODULUS 2147483647
#define MULTIPLIER 48271
#define DT 0.01
#define G 6.6742E-11
#define COLLISION_FACTOR 1

MPI_Datatype particletype;
MPI_Datatype oldtype[4]= {MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
int blockcount[4]= {2, 2, 2, 2};
MPI_Aint offset[4];
MPI_Status status;
static long seed = 0; //get the random seed


typedef struct {
    double lable, mass;
    double x, y;
    double fx, fy;
    double vx, vy;
} Particle;

typedef enum { DATA_TAG, TERM_TAG, RESULT_TAG} Tags;

double Random(int isX, int board)
{
    const long Q = MODULUS / MULTIPLIER;
    const long R = MODULUS % MULTIPLIER;
    int board_x, board_y;
    long t;

    t = MULTIPLIER * (seed % Q) - R * (seed / Q);
    if (t > 0)
    seed = t;
    else
    seed = t + MODULUS;

    srand(seed);

    if (isX == 1) {
        return (rand() % (X_UPPER - X_LOWER)) + board;
    } else if (isX == 0) {
        return (rand() % (Y_UPPER - Y_LOWER)) + board;
    } else {
        return (rand() / (double)(RAND_MAX / 95) + 5) * 1E15;
    }
}

double distance(Particle p1, Particle p2)
{
    return sqrt(pow(p1.x - p2.x,2)+pow(p1.y - p2.y,2));
}

void check_col_body(Particle* p1, Particle* p2)
{
    if (distance(*p1, *p2) <= 1){
        double vx1 = p1->vx;
        double vy1 = p1->vy;
        p1->vx = ((p1->mass - p2->mass)*p1->vx + 2*p2->mass*p2->vx)/(p1->mass+p2->mass)*COLLISION_FACTOR;
        p1->vy = ((p1->mass - p2->mass)*p1->vy + 2*p2->mass*p2->vy)/(p1->mass+p2->mass)*COLLISION_FACTOR;
        p2->vx = ((p2->mass - p1->mass)*p2->vx + 2*p1->mass*vx1)/(p1->mass+p2->mass)*COLLISION_FACTOR;
        p2->vy = ((p2->mass - p1->mass)*p2->vy + 2*p1->mass*vy1)/(p1->mass+p2->mass)*COLLISION_FACTOR;
    }
}

void boundParticals(int i, Particle particles[])
{
    while (particles[i].x >= Y_UPPER || particles[i].y >= X_UPPER
                || particles[i].x <= X_LOWER || particles[i].y <= Y_LOWER) {

        if (particles[i].x >= X_UPPER) {
            particles[i].x = 2*X_UPPER - particles[i].x;
            particles[i].vx = -particles[i].vx * COLLISION_FACTOR;
        }

        if (particles[i].y >= Y_UPPER) {
            particles[i].y = 2*Y_UPPER - particles[i].y;
            particles[i].vy = -particles[i].vy * COLLISION_FACTOR;
        }

        if (particles[i].x <= X_LOWER) {
            particles[i].x = 2*X_LOWER - particles[i].x;
            particles[i].vx = -particles[i].vx * COLLISION_FACTOR;
        }

        if (particles[i].y <= Y_LOWER) {
            particles[i].y = 2*Y_LOWER - particles[i].y;
            particles[i].vy = -particles[i].vy * COLLISION_FACTOR;
        }
    }
}

void ComputeForces(Particle *p, Particle others[], int npart)
{
    int j;
    double ds = pow(floor((X_UPPER-X_LOWER)/sqrt(npart)/2-0.1),2);
    double rs,dx,dy;
    p->fx =0;
    p->fy = 0;
    for (j = 0; j < npart; j++){
        check_col_body(p, &others[j]);
        dx = others[(int)p->lable].x - others[j].x;
        dy = others[(int)p->lable].y - others[j].y;
        rs = pow(dx, 2) + pow(dy, 2);
        p->fx += -(G * others[(int)p->lable].mass * others[j].mass) * dx / pow(rs+ds,1.5);
        p->fy += -(G * others[(int)p->lable].mass * others[j].mass) * dy / pow(rs+ds,1.5);
    }
}

Particle* InitParticles(int npart)
{
    Particle* particles = (Particle *) malloc(sizeof(Particle)*npart);
    int i;
    int board_x = (800 - (X_UPPER + X_LOWER)) / 2;
    int board_y = (800 - (Y_UPPER + Y_LOWER)) / 2;
    for (i = 0; i < npart; i++) {
        particles[i].lable = i;
    	particles[i].x     = Random(1, board_x);
        // X_LOWER + rand()/(double)(RAND_MAX/(X_UPPER-X_LOWER)) + board_x;
        // Random(1, board_x);
    	particles[i].y     = Random(0, board_y);
        // Y_LOWER + rand()/(double)(RAND_MAX/(Y_UPPER-Y_LOWER)) + board_y;
        // Random(1, board_y);
    	particles[i].mass  = Random(-1, 0);
    	particles[i].fx	   = 0;
    	particles[i].fy	   = 0;
        particles[i].vx	   = 0;
        particles[i].vy	   = 0;
    }
    return particles;
}

void minions(int rank, int size) {
    int npart;
    MPI_Status status;
    MPI_Recv(&npart, 1, MPI_INT, 0, DATA_TAG, MPI_COMM_WORLD, &status);

    int start, work;

    work = npart / (size - 1);
    start = (rank - 1) * work;

    printf("%d------\n", npart);
    printf("%d - %d\n", start, work);

    Particle* particles = InitParticles(npart);

    do {
        MPI_Recv(particles, npart, particletype, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        double vx,vy,x,y,ax,ay;
        int i;
        for (i = start; i < start + work; i++) {
            ComputeForces(&particles[i], particles, npart);
            ax = particles[i].fx * DT/ particles[i].mass;
            ay = particles[i].fy * DT/ particles[i].mass;

            particles[i].vx = particles[i].vx + ax*DT;
            particles[i].vy = particles[i].vy + ay*DT;

            particles[i].x = (vx + particles[i].vx)*DT / 2 + particles[i].x;
            particles[i].y = (vy + particles[i].vy)*DT / 2 + particles[i].y;

            boundParticals(i, particles);
        }

        MPI_Send(particles, npart, particletype, 0, RESULT_TAG, MPI_COMM_WORLD);
    } while (status.MPI_TAG != TERM_TAG);
}

int main(int argc, char **argv)
{
    /* main MPI beigins */
    int rank, size, row, i, working;
    if(MPI_Init(&argc,&argv) != MPI_SUCCESS)
    {
        perror("Unable to initialize MPI\n");
        exit(1);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Particle* particles;
    Particle* tmpPart;  /* Particles */

    MPI_Get_address(&(particles[0].lable), &offset[0]);
	MPI_Get_address(&(particles[0].x), &offset[1]);
	MPI_Get_address(&(particles[0].fx), &offset[2]);
    MPI_Get_address(&(particles[0].vx), &offset[3]);
	for(int i = 3; i >= 0; i--) offset[i] -= offset[0];
	MPI_Type_create_struct(4, blockcount, offset, oldtype, &particletype);
    MPI_Type_commit(&particletype);

    if (rank != 0) {
        minions(rank, size);
    } else {

        int cnt = 0, limit = 0, npart, i, j, work, start;

        if (argc != 3) {
    		printf("Wrong number of parameters.\nUsage: nbody num_bodies timesteps\n");
    		exit(1);
    	}


        limit = atoi(argv[2]);
        seed = time(0);
    	npart = atoi(argv[1]);

        // Start counting time
        double totaltime;
        struct timeval t_val, t_val_end, t_result;
        gettimeofday(&t_val, NULL);

        /* Generate the initial values */
        particles = InitParticles(npart);
        tmpPart = InitParticles(npart);
        for (i = 1; i < size; i++) {
            MPI_Send(&npart, 1, MPI_INT, i, DATA_TAG, MPI_COMM_WORLD);
        }

        while (cnt <= limit) {

            for (i = 1; i < size; i++) {
                if (cnt >= limit) {
                    MPI_Send(particles, npart, particletype, i, TERM_TAG, MPI_COMM_WORLD);
                } else {
                    MPI_Send(particles, npart, particletype, i, DATA_TAG, MPI_COMM_WORLD);
                }
            }

            for (i = 1; i < size; i++) {
                MPI_Recv(tmpPart, npart, particletype, i, RESULT_TAG, MPI_COMM_WORLD, &status);
                if (rank == size - 1) {
                    work = npart - (npart / size) * size;
                }

                work = npart / (size - 1);
                start = (i - 1) * work;

                for (int i = start; i < start + work; i++) {
                    particles[i].x = tmpPart[i].x;
                    particles[i].y = tmpPart[i].y;
                    particles[i].fx = tmpPart[i].fx;
                    particles[i].fy = tmpPart[i].fy;
                    particles[i].vx = tmpPart[i].vx;
                    particles[i].vy = tmpPart[i].vy;
                }
            }

            cnt++;
        }

        for (i=0; i<npart; i++) {
            fprintf(stdout,"%.5lf %.5lf\n", particles[i].x, particles[i].y);
        }

        //end counting time
        gettimeofday(&t_val_end, NULL);
        timersub(&t_val_end, &t_val, &t_result);
        totaltime = t_result.tv_sec + (1.0 * t_result.tv_usec)/1000000;
        printf("total time : %f\n", totaltime);

        /* record the time log in a txt file */
        FILE *log_file_p;
        char file_name[100];
        sprintf(file_name, "data/MPI_%d_sendRecv_b%d_i%d.txt", size, npart, limit);
        log_file_p = fopen(file_name, "a");  // add a new log
        fprintf(log_file_p, "%f\n", totaltime);
        fclose(log_file_p);
        /* recording ends */

        printf("%s\n", "end");
    }

    MPI_Type_free(&particletype);
	MPI_Finalize();
    return 0;
}
