/**
 *@Credit: Eric Yuan Changcheng 116010276 CUHK(SZ)
 *@Course: CSC4005: Distributed and PC
 * Note that this is the Pthread + static scheduling
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#define         NUM_THREAD 10 // default value 10
#define         X_RESN  800       /* x resolution */
#define         Y_RESN  800       /* y resolution */
#define         Vibra_Limit 100   /* Set the vibration limit */

int map[X_RESN][Y_RESN];
// int NUM_THREAD;

typedef struct complextype
        {
        float real, imag;
        } Compl;

int Manda_cal(Compl c)
{
    int count;
    Compl z;
    float temp, lengthsq;
    z.real = 0.0; z.imag = 0.0;
    count = 0;

    do {
        temp = z.real * z.real - z.imag * z.imag + c.real;
        z.imag = 2 * z.real * z.imag + c.imag;
        z.real = temp;
        lengthsq = z.real * z.real + z.imag * z.imag;
        count++;
    } while((lengthsq < 4.0) && (count < Vibra_Limit));

    return count;
}

void *minions(void* t) {
    int rank = *((int*)&t);
    int row, col;

    Compl c;
    row = rank * (Y_RESN / (NUM_THREAD));
    do {
        c.imag = ((float)row - 400.0)/200.0;

        // calculation of the value
        for (col = 0; col < X_RESN; col++) {
            c.real = ((float) col - 400.0)/200.0;
            map[col][row] = Manda_cal(c);

        }

        row++;
    } while (row < 1 + (rank + 1) * (Y_RESN / (NUM_THREAD))
                && row < Y_RESN);
}

int main(int argc, char **argv) {

    //this is for reading from terminal input
    // sscanf(argv[1], "%d", &NUM_THREAD);
    // pthread_t *threads = malloc(sizeof(pthread_t) * NUM_THREAD);

    /* Initialize all the variables */
    pthread_t threads[NUM_THREAD];

    /* Set the window */
    Window          win;                            /* initialization for a window */
    GC              gc;
    Display         *display;
    unsigned
    int             width, height,                  /* window size */
                    x, y,                           /* window position */
                    border_width,                   /*border width in pixels */
                    display_width, display_height,  /* size of screen */
                    screen;                         /* which screen */

    char            *window_name = "Mandelbrot Set", *display_name = NULL;
    unsigned
    long            valuemask = 0;

    XSizeHints      size_hints;
    XGCValues       values;

    XSetWindowAttributes attr[1];

    /* connect to Xserver */

    if (  (display = XOpenDisplay (display_name)) == NULL ) {
       fprintf (stderr, "drawon: cannot connect to X server %s\n",
                            XDisplayName (display_name) );
    exit (-1);
    }

    /* get screen size */

    screen = DefaultScreen (display);
    display_width = DisplayWidth (display, screen);
    display_height = DisplayHeight (display, screen);

    /* set window size */

    width = X_RESN;
    height = Y_RESN;

    /* set window position */

    x = 0;
    y = 0;

    /* create opaque window */

    border_width = 4;
    win = XCreateSimpleWindow (display, RootWindow (display, screen),
                            x, y, width, height, border_width,
                            BlackPixel (display, screen), WhitePixel (display, screen));

    size_hints.flags = USPosition|USSize;
    size_hints.x = x;
    size_hints.y = y;
    size_hints.width = width;
    size_hints.height = height;
    size_hints.min_width = 300;
    size_hints.min_height = 300;

    XSetNormalHints (display, win, &size_hints);
    XStoreName(display, win, window_name);

    /* create graphics context */

    gc = XCreateGC (display, win, valuemask, &values);

    XSetBackground (display, gc, 0x0000ff); //WhitePixel (display, screen));
    XSetForeground (display, gc, 0x00ff00); // BlackPixel (display, screen));
    XSetLineAttributes (display, gc, 1, LineSolid, CapRound, JoinRound);

    attr[0].backing_store = Always;
    attr[0].backing_planes = 10;
    attr[0].backing_pixel = BlackPixel(display, screen);

    XChangeWindowAttributes(display, win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);

    XMapWindow (display, win);
    XSync(display, 0);

    /* Window setup ends */

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

    /* print the map */
    int j;
    for(i = 0; i < X_RESN; i++)
    for(j = 0; j < Y_RESN; j++) {
        // draw colorful
        XSetForeground (display, gc, map[j][i] * 10000);
        XDrawPoint(display, win, gc, j, i);

        // draw two colors
        // if (colors[j] == Vibra_Limit) {
        //     XDrawPoint(display, win, gc, j, row);
        // }
    }
    /* print ends */


    //End counting time
    gettimeofday(&t_val_end, NULL);
    timersub(&t_val_end, &t_val, &t_result);
    totaltime = t_result.tv_sec + (1.0 * t_result.tv_usec)/1000000;
    printf("total time : %f\n", totaltime);

    /* record the time log in a txt file */
    // FILE *log_file_p;
    // char file_name[100];
    // sprintf(file_name, "MPI_%d_dynamic.txt", size);
    // log_file_p = fopen(file_name, "a");  // add a new log
    // fprintf(log_file_p, "%f\n", totaltime);
    // fclose(log_file_p);
    /* recording ends */

    printf("\nend\n");
    XFlush (display);
	sleep (30);
    pthread_exit(NULL) ;
    return 0;
}
