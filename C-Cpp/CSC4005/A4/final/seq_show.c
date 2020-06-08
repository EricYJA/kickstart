/**
 *@Credit: Eric Yuan Changcheng 116010276 CUHK(SZ)
 *@Course: CSC4005: Distributed and PC A4
 * Note that this is the sequential heat distribution simulation
 * This file is for demo, complie thisfile alone on the cluster
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
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

const char *window_name = "Heat Distribution";
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

static int roomSize, roomTmp, Flag, limit, sUP;
double ** tmpMap;
double ** instanMap;

typedef enum { DATA_TAG, TERM_TAG, RESULT_TAG} Tags;

static int Rarray[25] = {51, 70, 84, 67, 77, 87, 79, 132, 108, 115,
                    171, 203, 238, 249, 252, 253, 252, 250, 250, 255,
                    240, 247, 252, 231, 213};

static int Garray[25] = {13, 33, 64, 73, 102, 117, 151, 185, 198, 225,
                    247, 251, 253, 250, 242, 227, 200, 166, 146, 121,
                    93, 79, 38, 0, 6};

static int Barray[25] = {128, 146, 182, 201, 210, 213, 225, 251, 236, 231,
                    235, 218, 202, 213, 172, 125, 101, 68, 0, 26,
                    4, 20, 3, 0, 55};

void init_window(int roomSize) {
    if((display = XOpenDisplay(display_name)) == NULL) {fprintf(stderr, "drawon: cannot connect to X server %s\n", XDisplayName(display_name));}
	screen = DefaultScreen(display);
	display_width = DisplayWidth(display, screen);
	display_height = DisplayHeight(display, screen);
	width = roomSize;
	height = roomSize;
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
	XSetBackground(display, gc, BlackPixel (display, screen));
	XSetForeground(display, gc, 0x00ff00);
	XSetLineAttributes(display, gc, 1, LineSolid, CapRound, JoinRound);
	attr[0].backing_store = Always;
	attr[0].backing_planes = 1;
	attr[0].backing_pixel = BlackPixel(display, screen);
	XChangeWindowAttributes(display, win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);
	XMapWindow(display, win);
	XSync(display, 0);
}

int getcolor(double tmp) {
    int ratio = ((tmp - 1) / 5) + 5;
    // printf("%d - %f\n", ratio, tmp);
    int r = Rarray[ratio];
    int g = Garray[ratio];
    int b = Barray[ratio];
    int toReturn = (r << 16) + (g << 8) + b;
    if (toReturn > 0xffffff) {
        return 0xffffff;
    }
    return toReturn;
}

void updatePoints( double ** tmpMap, int roomSize)
{
    int i, j;
    // XClearWindow(display,win);
    for (i = 0; i < roomSize; i++) {
        for (j = 0; j < roomSize; j++) {
            XSetForeground(display, gc, getcolor(tmpMap[i][j]));
            XDrawPoint(display, win, gc, j, i);
        }
    }
    XFlush(display);
    usleep(1000);
}

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

int main(int argc, char **argv)
{
    int i, diff = 0, cnt = 0, displayFlag = 1;
    roomSize = atoi(argv[1]);
    roomTmp = atoi(argv[2]);
    limit = atoi(argv[3]);
    displayFlag = atoi(argv[4]);
    tmpMap = InitRoom(roomSize, roomTmp);
    instanMap = InitRoom(roomSize, roomTmp);
    sUP = 10;
    if (displayFlag == 1) init_window(roomSize);

    // Start counting time
    double totaltime;
    struct timeval t_val, t_val_end, t_result;
    gettimeofday(&t_val, NULL);

    do {
        if (sqrt(diff) < ERRORS && limit == -1 && cnt > 1) limit = cnt + 1;

        diff = 0;

        if (cnt % 2 == 0) {
            for (i = 1; i < roomSize - 1; i++) {
                diff = diff + ComputeTemp(tmpMap, instanMap, i, roomSize);
            }

            if (cnt % sUP == 0 && displayFlag == 1) {
                updatePoints(instanMap, roomSize);
            }
        } else {
            for (i = 1; i < roomSize - 1; i++) {
                diff = diff + ComputeTemp(instanMap, tmpMap, i, roomSize);
            }

            if (cnt % sUP == 0 && displayFlag == 1) {
                updatePoints(tmpMap, roomSize);
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
    sprintf(file_name, "SEQ_r%d_i%d_l%d.txt", roomSize, roomTmp, limit);
    log_file_p = fopen(file_name, "a");  // add a new log
    fprintf(log_file_p, "%f\n", totaltime);
    fclose(log_file_p);
    /* recording ends */

    printf("%s cnt: %d\n", "end", cnt);
    if (displayFlag == 1) XFlush(display);
    return 0;
}
