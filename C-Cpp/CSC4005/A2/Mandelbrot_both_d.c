/**
 *@Credit: Eric Yuan Changcheng 116010276 CUHK(SZ)
 *@Course: CSC4005: Distributed and PC
 * Note that this is the MPI + Pthread + static scheduling
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <mpi.h>
#include <pthread.h>

#define         NUM_THREAD 4
#define         X_RESN  800       /* x resolution */
#define         Y_RESN  800       /* y resolution */
#define         Vibra_Limit 1024   /* Set the vibration limit */

int global_col = 0;
int colors[X_RESN + 1];
pthread_mutex_t colLock;

typedef struct complextype
        {
        float real, imag;
        } Compl;

typedef enum { DATA_TAG, TERM_TAG, RESULT_TAG} Tags;


int Manda_cal(Compl c)
{
    int count;
    Compl z;
    float temp, lengthsq;
    z.real = 0; z.imag = 0;
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

void *p_minions(void* t) {
    int row = colors[0];
    int col;

    Compl c;

    do {
        pthread_mutex_lock(&colLock);
        col = global_col;
        global_col++;
        pthread_mutex_unlock(&colLock);

        if(col >= X_RESN - 1) {
            break;
        }

        c.imag = ((float)row - 400.0)/200.0;
        c.real = ((float)col - 400.0)/200.0;
        colors[col + 1] = Manda_cal(c);

        printf("%d\n", col);


    } while (col < X_RESN);

}

void minions() {

    int row,col,rank;
    MPI_Status status;
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    Compl c;

    pthread_mutex_init(&colLock, NULL);
    pthread_t threads[NUM_THREAD];

    do {
        MPI_Recv(&row , 1 , MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);


        colors[0] = row;


        // calculation of the value
        long i;
        for(i = 0; i < NUM_THREAD; i++) {
            pthread_create(&threads[i], NULL, p_minions, (void *)i);
        }

        for(i = 0; i < NUM_THREAD; i++){
    		pthread_join(threads[i], NULL);
    	}

        if (status.MPI_TAG != TERM_TAG) {

            MPI_Send(&colors[0], X_RESN, MPI_INT, 0, RESULT_TAG, MPI_COMM_WORLD);
        }

    } while (status.MPI_TAG != TERM_TAG);

    pthread_mutex_destroy(&colLock); // destroy mutex
    pthread_exit(NULL) ;
}

void draw(Display * display, Window win, GC gc, int row, int * colors)
{
    int j = 0;
    for(j=0; j < X_RESN; j++) {
        if (colors[j] == Vibra_Limit) {
            XDrawPoint(display, win, gc, j, row);
        }
    }
}


int main(int argc, char **argv) {
    int rank, size, row, i, working;
    // if(MPI_Init(&argc,&argv) != MPI_SUCCESS)
    // {
    //     perror("Unable to initialize MPI\n");
    //     exit(1);
    // }
    int provided;
	MPI_Init_thread(&argc,&argv,MPI_THREAD_MULTIPLE, &provided);
	if(provided != MPI_THREAD_MULTIPLE)
	{
		printf("MPI do not Support Multiple thread\n");
		exit(0);
	}

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank != 0) {
        minions();
    } else {
        /* Enter the Boss process */

        /* Set the window */
        Window          win;                            /* initialization for a window */
        unsigned
        int             width, height,                  /* window size */
                        x, y,                           /* window position */
                        border_width,                   /*border width in pixels */
                        display_width, display_height,  /* size of screen */
                        screen;                         /* which screen */

        char            *window_name = "Mandelbrot Set", *display_name = NULL;
        GC              gc;
        unsigned
        long            valuemask = 0;
        Display         *display;
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

        // XSetBackground (display, gc, 0x0000ff); //WhitePixel (display, screen));
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

        /* send the initial task to each worker process */
        row = 0;
        working = 0;
        for (int i = 1; i < size; i++) {
            MPI_Send(&row, 1, MPI_INT, i, DATA_TAG, MPI_COMM_WORLD);
            row++;
            working++;
        }
        /* send ends */

        int * colors = (int *) malloc(sizeof(int) * (X_RESN+1));
        MPI_Status status;
        do {
            MPI_Recv(&colors[0], X_RESN,
                MPI_INT, MPI_ANY_SOURCE, RESULT_TAG, MPI_COMM_WORLD, &status);
            draw(display, win, gc, colors[0], &colors[1]);
            working--;
            if (row < Y_RESN) {
                MPI_Send(&row, 1, MPI_INT, status.MPI_SOURCE, DATA_TAG, MPI_COMM_WORLD);
                row++;
                working++;
            } else {
                MPI_Send(&row, 1, MPI_INT, status.MPI_SOURCE, TERM_TAG, MPI_COMM_WORLD);
            }
        } while (working > 0 && row <= Y_RESN);

        //End counting time
        gettimeofday(&t_val_end, NULL);
        timersub(&t_val_end, &t_val, &t_result);
        totaltime = t_result.tv_sec + (1.0 * t_result.tv_usec)/1000000;
        printf("total time : %f\n", totaltime);

        // FILE *log_file_p;
        // char file_name[100];
        // sprintf(file_name, "MPI_%d_dynamic.txt", size);
        // log_file_p = fopen(file_name, "a");  // add a new log
        // fprintf(log_file_p, "%f\n", totaltime);
        // fclose(log_file_p);

        XFlush (display);
    	sleep (30);
        printf("end\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
