#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "mpi.h"

#define MASTER 0
#define X_RESN 800
#define Y_RESN 800

const double G = 1;
const double rid = 1e2;
const char *window_name = "N-body";
const int N = 12000;
const int num_steps = 100;
const double delta_t = 0.5;
typedef struct{
	int px, py;
	double vx, vy;
	double ax, ay;
	double m;
}vertex;

MPI_Datatype vertexTYPE;
MPI_Datatype oldtype[4] = {MPI_INT, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE};
int blockcount[4] = {2, 2, 2, 1};
MPI_Aint offset[4];
MPI_Status stat;
Window win;
unsigned int width, height, x, y, border_width, display_width, display_height, screen;
char *display_name = NULL;
GC gc;
unsigned long valuemask = 0;
XGCValues values;
Display *display;
XSizeHints size_hints;
Pixmap bitmap;
XSetWindowAttributes attr[1];

int randint(int lowerbound, int upperbound) {return rand()%(upperbound-lowerbound) + lowerbound;}
void init_bodies(vertex body[]) {
	for(int i=0; i<N; i++) {
		body[i].m = randint(5, 10);
		body[i].vx = 0; body[i].vy = 0;
		body[i].px = randint(0, 800); body[i].py = randint(0, 800);
		body[i].ax = 0; body[i].ay = 0;
	}
	MPI_Bcast(body, N, vertexTYPE, MASTER, MPI_COMM_WORLD);
}

void update(vertex *bodyM, vertex *bodyS) {
	int dx = bodyS->px - bodyM->px;
	int dy = bodyS->py - bodyM->py;
	double r = dx*dx + dy*dy;
	double r_3 = pow(sqrt(r), 3);
	double F_half = G/(r_3+rid);
	bodyM->ax += F_half * bodyS->m * dx;
	bodyM->ay += F_half * bodyS->m * dy;
}

void update_info(vertex *body_tmp) {
	body_tmp->vx += body_tmp->ax * delta_t;
	body_tmp->vy += body_tmp->ay * delta_t;
	body_tmp->px += (int)(body_tmp->vx * delta_t);
	body_tmp->py += (int)(body_tmp->vy * delta_t);
	if(body_tmp->px <= 0) {body_tmp->vx = -body_tmp->vx; body_tmp->px = 1;}
	if(body_tmp->px >= X_RESN) {body_tmp->vx = -body_tmp->vx; body_tmp->px = X_RESN - 1;}
	if(body_tmp->py <= 0) {body_tmp->vy = -body_tmp->vy; body_tmp->py = 1;}
	if(body_tmp->py >= Y_RESN) {body_tmp->vy = -body_tmp->vy; body_tmp->py = Y_RESN - 1;}
	body_tmp->ax = 0; body_tmp->ay = 0;
}

void init_window() {
	if((display = XOpenDisplay(display_name)) == NULL) {fprintf(stderr, "drawon: cannot connect to X server %s\n", XDisplayName(display_name));}
	screen = DefaultScreen(display);
	display_width = DisplayWidth(display, screen);
	display_height = DisplayHeight(display, screen);
	width = X_RESN;
	height = Y_RESN;
	x = 0;
	y = 0;
	border_width = 4;
	win = XCreateSimpleWindow(display, RootWindow(display, screen), x, y, width, height, border_width, BlackPixel(display, screen), WhitePixel(display, screen));
	size_hints.flags = USPosition | USSize;
	size_hints.x = x;
	size_hints.y = y;
	size_hints.width = width;
	size_hints.height = height;
	size_hints.min_width = 300;
	size_hints.min_height = 300;
	XSetNormalHints(display, win, &size_hints);
	XStoreName(display, win, window_name);
	gc = XCreateGC(display, win, valuemask, &values);
	XSetBackground(display, gc, WhitePixel(display, screen));
	XSetForeground(display, gc, BlackPixel(display, screen));
	XSetLineAttributes(display, gc, 1, LineSolid, CapRound, JoinRound);
	attr[0].backing_store = Always;
	attr[0].backing_planes = 1;
	attr[0].backing_pixel = BlackPixel(display, screen);
	XChangeWindowAttributes(display, win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);
	XMapWindow(display, win);
	XSync(display, 0);
}

int main(int argc, char* argv[]) {
	int size;
	double start_time;
	double end_time;
	int rank, total_processors;
	vertex body[N];
	vertex subbody[N];
	time_t start, end;
	clock_t startClock, endClock;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &total_processors);

	size = N / total_processors;
	MPI_Get_address(&(body[0].px), &offset[0]);
	MPI_Get_address(&(body[0].vx), &offset[1]);
	MPI_Get_address(&(body[0].ax), &offset[2]);
	MPI_Get_address(&(body[0].m), &offset[3]);
	for(int i = 3; i >= 0; i--) offset[i] -= offset[0];
	MPI_Type_create_struct(4, blockcount, offset, oldtype, &vertexTYPE);
    MPI_Type_commit(&vertexTYPE);

	if(rank==MASTER) {
		init_window();
		init_bodies(body);
		time(&start);
		startClock = clock();
	}
	else
		MPI_Bcast(body, N, vertexTYPE, MASTER, MPI_COMM_WORLD);

	start_time = MPI_Wtime();

	for(int step = 1; step <= num_steps; step++) {
		MPI_Scatter(body, size, vertexTYPE, subbody, size, vertexTYPE, MASTER, MPI_COMM_WORLD);

		for(int i = 0; i < size; i++)
			for(int j = 0; j < N; j++) {
				int part = rank * size + i;
				if(j != part) update(&subbody[i], &body[j]);
			}

		MPI_Gather(subbody, size, vertexTYPE, body, size, vertexTYPE, MASTER, MPI_COMM_WORLD);
		MPI_Scatter(body, size, vertexTYPE, subbody, size, vertexTYPE, MASTER, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
		for(int i = 0; i < size; i++) update_info(&subbody[i]);
		MPI_Gather(subbody, size, vertexTYPE, body, size, vertexTYPE, MASTER, MPI_COMM_WORLD);
		MPI_Barrier(MPI_COMM_WORLD);
	    if(rank == MASTER) {
			XClearArea(display,win,0,0,800,800,0);
			for(int i = 0; i < N ; i++) {
				XSetForeground(display, gc, BlackPixel(display, screen));
				XDrawArc(display, win, gc, body[i].px-1, body[i].py-1, 2, 2, 0, 360*64);
	          	XFlush(display);
	        }
	    }
	}

	end_time = MPI_Wtime();
	if(rank == MASTER) {
		time(&end);
		endClock = clock();
		double duration = difftime(end, start);
		double durationClock = (double)(endClock - startClock) / CLOCKS_PER_SEC;
		printf("Time: %lf\n", duration);
		printf("Clock Time: %lf\n", durationClock);
	}
	MPI_Type_free(&vertexTYPE);
	MPI_Finalize();
	return 0;
}
