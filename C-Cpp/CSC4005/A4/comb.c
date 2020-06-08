#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define NXPROB      20                 /* x dimension of problem grid */
#define NYPROB      20                 /* y dimension of problem grid */
#define STEPS       200                /* number of time steps */
#define MAXWORKER   8                  /* maximum number of worker tasks */
#define MINWORKER   3                  /* minimum number of worker tasks */
#define BEGIN       1                  /* message tag */
#define LTAG        2                  /* message tag */
#define RTAG        3                  /* message tag */
#define NONE        0                  /* indicates no neighbor */
#define DONE        4                  /* message tag */
#define MASTER      0                  /* taskid of first process */

#define HEIGHT         750
#define WIDTH          750
#define NCOLORS        255

typedef struct {
   Window window;
   XSizeHints hints;
   XColor backcolor;
   XColor bordcolor;
   int    bordwidth;
} MYWINDOW;

struct Parms {
  float cx;
  float cy;
} parms = {0.1, 0.1};


char title[] = {"draw_heat"},
     moreword[] = {"More"},
     exitword[] = {"Exit"},
     text[10];

/* To call from C, use the routine below - without the extra underscore */
void draw_heat(int nx, int ny) {

    float 	scale, point, high, *a1, *a2, coloratio = 65535.0 / 255.0;
    int 	i,j,k,x,y,done, myscreen, rectheight, rectwidth,toggle;
    FILE	*initial, *final;

    MYWINDOW base,more,quit;
    Font 	font,font2;
    GC 		itemgc,textgc,rectgc[NCOLORS],linegc;
    XColor 	rectcolor,red,yellow,blue,green,black,white;
    XEvent 	myevent;
    Colormap cmap;
    KeySym 	mykey;
    Display *mydisp;

    yellow.red= (int) (255 * coloratio);
    yellow.green= (int) (255 * coloratio);
    yellow.blue= (int) (0 * coloratio);


    green.red= (int) (0 * coloratio);
    green.green= (int) (255 * coloratio);
    green.blue= (int) (0 * coloratio);

    black.red= (int) (0 * coloratio);
    black.green= (int) (0 * coloratio);
    black.blue= (int) (0 * coloratio);

    white.red= (int) (255 * coloratio);
    white.green= (int) (255 * coloratio);
    white.blue= (int) (255 * coloratio);

    mydisp = XOpenDisplay("");
    if (!mydisp) {
        fprintf (stderr, "Hey! Either you don't have X or something's not right.\n");
        fprintf (stderr, "Guess I won't be showing the graph.  No big deal.\n");
        exit(1);
    }
    myscreen = DefaultScreen(mydisp);
    cmap = DefaultColormap (mydisp, myscreen);
    XAllocColor (mydisp, cmap, &yellow);
    XAllocColor (mydisp, cmap, &black);
    XAllocColor (mydisp, cmap, &green);
    XAllocColor (mydisp, cmap, &white);

    base.hints.x = 50;
    base.hints.y = 50;
    base.hints.width = WIDTH;
    base.hints.height = HEIGHT;
    base.hints.flags = PPosition | PSize;
    base.bordwidth = 5;

    base.window = XCreateSimpleWindow (mydisp, DefaultRootWindow (mydisp),
                base.hints.x, base.hints.y, base.hints.width, base.hints.height,
                base.bordwidth, black.pixel, black.pixel);
    XSetStandardProperties (mydisp, base.window, title, title, None,
                            NULL, 0, &base.hints);

    /* quit window position and size (subwindow of base) */
    quit.hints.x = 5;
    quit.hints.y = HEIGHT-75;
    quit.hints.width = 70;
    quit.hints.height = 30;
    quit.hints.flags = PPosition | PSize;
    quit.bordwidth = 5;

    quit.window = XCreateSimpleWindow (mydisp, base.window, quit.hints.x,
                quit.hints.y, quit.hints.width, quit.hints.height,
                quit.bordwidth, green.pixel, yellow.pixel);
    XSetStandardProperties (mydisp, quit.window, exitword, exitword, None,
                NULL, 0, &quit.hints);
    /* more window position and size (subwindow of base) */
    more.hints.x = WIDTH-85;
    more.hints.y = HEIGHT-75;
    more.hints.width = 70;
    more.hints.height = 30;
    more.hints.flags = PPosition | PSize;
    more.bordwidth = 5;

    more.window = XCreateSimpleWindow (mydisp, base.window, more.hints.x,
                more.hints.y, more.hints.width, more.hints.height,
                more.bordwidth, green.pixel, yellow.pixel);
    XSetStandardProperties (mydisp, more.window, moreword, moreword, None,
                NULL, 0, &more.hints);

    /* Load fonts */
    /*
    font = XLoadFont (mydisp, "Rom28");
    font2 = XLoadFont (mydisp, "Rom17.500");
    */
    font = XLoadFont (mydisp, "fixed");
    font2 = XLoadFont (mydisp, "fixed");

    /* GC creation and initialization */
    textgc = XCreateGC (mydisp, base.window, 0,0);
    XSetFont (mydisp, textgc, font);
    XSetForeground (mydisp, textgc, green.pixel);

    linegc = XCreateGC (mydisp, base.window, 0,0);
    XSetForeground (mydisp, linegc, white.pixel);

    itemgc = XCreateGC (mydisp, quit.window, 0,0);
    XSetFont (mydisp, itemgc, font2);
    XSetForeground (mydisp, itemgc, black.pixel);

    /* create shades of red color scale */
    for (k=0;k<NCOLORS;k++) {
        rectgc[k] = XCreateGC (mydisp, base.window, 0,0);
        rectcolor.red= (int) (k * coloratio);
        rectcolor.green= (int) (0 * coloratio);
        rectcolor.blue= (int) (0 * coloratio);
        XAllocColor (mydisp, cmap, &rectcolor);
        XSetForeground (mydisp, rectgc[k], rectcolor.pixel);
    }

    /* The program is event driven; the XSelectInput call sets which kinds of */
    /* interrupts are desired for each window. These aren't all used. */
    XSelectInput (mydisp, base.window,
                ButtonPressMask | KeyPressMask | ExposureMask);
    XSelectInput (mydisp, quit.window,
                ButtonPressMask | KeyPressMask | ExposureMask);
    XSelectInput (mydisp, more.window,
                ButtonPressMask | KeyPressMask | ExposureMask);

    /* window mapping -- this lets windows be displayed */
    XMapRaised (mydisp, base.window);
    XMapSubwindows (mydisp, base.window);

    /* create space for data to be read */
    a1 = (float *) malloc(nx*ny*sizeof(float));
    a2 = (float *) malloc(nx*ny*sizeof(float));

    /* Read the two data files created by heat2D program execution */
    /* Also determine the highest value read */
    high = 0.0;
    initial = fopen("initial.dat","r");
    for (k=0;k<nx*ny;k++) {
    fscanf(initial,"%f",a1+k);
    if ( *(a1+k) > high) high = *(a1+k);
    }
    fclose(initial);

    final = fopen("final.dat","r");
    for (k=0;k<nx*ny;k++)
    fscanf(final,"%f",a2+k);
    fclose(final);

    /* Scale rectangle size and color */
    rectwidth = WIDTH/ny;
    rectheight = HEIGHT/nx;
    scale = ((float)NCOLORS) / high;

    /* Main event loop  --  exits when user clicks on "exit" */
    done = 0;
    toggle = 0;
    while (! done) {
    XNextEvent (mydisp, &myevent);   /* Read next event */
    switch (myevent.type) {
    case Expose:
        if (myevent.xexpose.count == 0) {
            if (myevent.xexpose.window == base.window) {
                for (x=0;x<nx;x++) {
                for (y=0;y<ny;y++) {
                    if (toggle == 0)
                    point = scale * *(a1+x*ny+y);
                    else
                    point = scale * *(a2+x*ny+y);
                    k = (int) point;
                    if (k >= NCOLORS) k = NCOLORS-1;
                    XFillRectangle (mydisp, base.window, rectgc[k], y*rectwidth,
                                    x*rectheight, rectwidth, rectheight);
                    }
                }
                for (x=0;x<nx;x++)
                XDrawLine (mydisp, base.window, linegc, 1,x*rectheight,WIDTH,
                            x*rectheight);
                for (y=0;y<ny;y++)
                XDrawLine (mydisp, base.window, linegc, y*rectwidth,1,
                            y*rectwidth,HEIGHT);
                XDrawString (mydisp, base.window, textgc,325,30,"Heat2D",6);
                if (toggle == 0)
                XDrawString (mydisp, base.window, textgc,325,735,"INITIAL",7);
                else
                XDrawString (mydisp,base.window,textgc,330,735,"FINAL",5);
                }

            else if (myevent.xexpose.window == quit.window)
                XDrawString (mydisp, quit.window, itemgc, 12,20, exitword,
                            strlen(exitword));
            else if (myevent.xexpose.window == more.window)
                XDrawString (mydisp, more.window, itemgc, 12,20, moreword,
                            strlen(moreword));
        }   /* case Expose */
        break;

    case ButtonPress:
        if (myevent.xbutton.window == quit.window)
            done = 1;
        else if (myevent.xbutton.window == more.window) {
            if (toggle==0)
            toggle = 1;
            else if (toggle==1)
            toggle = 0;
            for (x=0;x<nx;x++) {
            for (y=0;y<ny;y++) {
                if (toggle==0)
                point = scale * *(a1+x*ny+y);
                else
                point = scale * *(a2+x*ny+y);
                k = (int) point;
                if (k >= NCOLORS) k = NCOLORS-1;
                XFillRectangle (mydisp, base.window, rectgc[k], y*rectwidth,
                                x*rectheight, rectwidth, rectheight);
                }
            }
            for (x=0;x<nx;x++)
                XDrawLine (mydisp, base.window, linegc, 1,x*rectheight,WIDTH,
                        x*rectheight);
            for (y=0;y<ny;y++)
                XDrawLine (mydisp, base.window, linegc, y*rectwidth,1,
                        y*rectwidth,HEIGHT);
            XDrawString (mydisp, base.window, textgc,325,30,"heat2D",6);
            if (toggle==0)
                XDrawString (mydisp, base.window, textgc,325,735,"INITIAL",7);
            else
                XDrawString (mydisp,base.window,textgc,330,735,"FINAL",5);
        }

        break;

    case MappingNotify:
    /*
        XRefreshKeyboardMapping (&myevent);
    */
        break;

        }  /* switch (myevent.type) */

    }   /* while (! done) */

    XDestroyWindow (mydisp, base.window);
    XCloseDisplay (mydisp);
}

void inidat(int nx, int ny, float *u) {
    int ix, iy;
    for (ix = 0; ix <= nx-1; ix++)
        for (iy = 0; iy <= ny-1; iy++)
            *(u+ix*ny+iy) = (float)(ix * (nx - ix - 1) * iy * (ny - iy - 1));
}

void prtdat(int nx, int ny, float *u1, char *fnam) {
    int ix, iy;
    FILE *fp;
    fp = fopen(fnam, "w");
    for (iy = ny-1; iy >= 0; iy--) {
        for (ix = 0; ix <= nx-1; ix++) {
            fprintf(fp, "%8.1f", *(u1+ix*ny+iy));
            if (ix != nx-1)
            fprintf(fp, " ");
            else
            fprintf(fp, "\n");
            }
    }
    fclose(fp);
}

void update(int start, int end, int ny, float *u1, float *u2) {
    int ix, iy;
    for (ix = start; ix <= end; ix++)
        for (iy = 1; iy <= ny-2; iy++)
            *(u2+ix*ny+iy) = *(u1+ix*ny+iy)  +
                            parms.cx * (*(u1+(ix+1)*ny+iy) +
                            *(u1+(ix-1)*ny+iy) -
                            2.0 * *(u1+ix*ny+iy)) +
                            parms.cy * (*(u1+ix*ny+iy+1) +
                            *(u1+ix*ny+iy-1) -
                            2.0 * *(u1+ix*ny+iy));
}


int main (int argc, char *argv[]) {
    float  u[2][NXPROB][NYPROB];        /* array for grid */
    int	taskid,                     /* this task's unique id */
        numworkers,                 /* number of worker processes */
        numtasks,                   /* number of tasks */
        averow,rows,offset,extra,   /* for sending rows of data */
        dest, source,               /* to - from for message send-receive */
        left,right,        /* neighbor tasks */
        msgtype,                    /* for message types */
        rc,start,end,               /* misc */
        i,ix,iy,iz,it;              /* loop variables */
    MPI_Status status;


    /* First, find out my taskid and how many tasks are running */
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    numworkers = numtasks-1;

    if (taskid == MASTER) {
        /************************* master code *******************************/
        /* Check if numworkers is within range - quit if not */
        if ((numworkers > MAXWORKER) || (numworkers < MINWORKER)) {
            printf("ERROR: the number of tasks must be between %d and %d.\n",
                    MINWORKER+1,MAXWORKER+1);
            printf("Quitting...\n");
            MPI_Abort(MPI_COMM_WORLD, rc);
            exit(1);
        }
        printf ("Starting mpi_heat2D with %d worker tasks.\n", numworkers);

        /* Initialize grid */
        printf("Grid size: X= %d  Y= %d  Time steps= %d\n",NXPROB,NYPROB,STEPS);
        printf("Initializing grid and writing initial.dat file...\n");
        inidat(NXPROB, NYPROB, u);
        prtdat(NXPROB, NYPROB, u, "initial.dat");

        averow = NXPROB/numworkers;
        extra = NXPROB%numworkers;
        offset = 0;
        for (i=1; i<=numworkers; i++) {
            rows = (i <= extra) ? averow+1 : averow;

            if (i == 1)
                left = NONE;
            else
                left = i - 1;
            if (i == numworkers)
                right = NONE;
            else
                right = i + 1;
            /*  Now send startup information to each worker  */
            dest = i;
            MPI_Send(&offset, 1, MPI_INT, dest, BEGIN, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, BEGIN, MPI_COMM_WORLD);
            MPI_Send(&left, 1, MPI_INT, dest, BEGIN, MPI_COMM_WORLD);
            MPI_Send(&right, 1, MPI_INT, dest, BEGIN, MPI_COMM_WORLD);
            MPI_Send(&u[0][offset][0], rows*NYPROB, MPI_FLOAT, dest, BEGIN,
                    MPI_COMM_WORLD);
            printf("Sent to task %d: rows= %d offset= %d ",dest,rows,offset);
            printf("left= %d right= %d\n",left,right);
            offset = offset + rows;
        }
        for (i=1; i<=numworkers; i++)
        {
            source = i;
            msgtype = DONE;
            MPI_Recv(&offset, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD,
                    &status);
            MPI_Recv(&rows, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status);
            MPI_Recv(&u[0][offset][0], rows*NYPROB, MPI_FLOAT, source,
                    msgtype, MPI_COMM_WORLD, &status);
        }

        /* Write final output, call X graph and finalize MPI */
        printf("Writing final.dat file and generating graph...\n");
        prtdat(NXPROB, NYPROB, &u[0][0][0], "final.dat");
        printf("Click on MORE button to view initial/final states.\n");
        printf("Click on EXIT button to quit program.\n");
        draw_heat(NXPROB,NYPROB);
        MPI_Finalize();
    }   /* End of master code */



    /************************* workers code **********************************/
    if (taskid != MASTER) {
        /* Initialize everything - including the borders - to zero */
        for (iz=0; iz<2; iz++)
            for (ix=0; ix<NXPROB; ix++)
                for (iy=0; iy<NYPROB; iy++)
                u[iz][ix][iy] = 0.0;

        /* Receive my offset, rows, neighbors and grid partition from master */
        source = MASTER;
        msgtype = BEGIN;
        MPI_Recv(&offset, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&left, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&right, 1, MPI_INT, source, msgtype, MPI_COMM_WORLD, &status);
        MPI_Recv(&u[0][offset][0], rows*NYPROB, MPI_FLOAT, source, msgtype,
                MPI_COMM_WORLD, &status);

        /* Determine border elements.  Need to consider first and last columns. */
        /* Obviously, row 0 can't exchange with row 0-1.  Likewise, the last */
        /* row can't exchange with last+1.  */
        start=offset;
        end=offset+rows-1;
        if (offset==0)
            start=1;
        if ((offset+rows)==NXPROB)
            end--;
        printf("task=%d  start=%d  end=%d\n",taskid,start,end);

        /* Begin doing STEPS iterations.  Must communicate border rows with */
        /* neighbors.  If I have the first or last grid row, then I only need */
        /*  to  communicate with one neighbor  */
        printf("Task %d received work. Beginning time steps...\n",taskid);
        iz = 0;
        for (it = 1; it <= STEPS; it++) {
            if (left != NONE) {
                MPI_Send(&u[iz][offset][0], NYPROB, MPI_FLOAT, left,
                        RTAG, MPI_COMM_WORLD);
                source = left;
                msgtype = LTAG;
                MPI_Recv(&u[iz][offset-1][0], NYPROB, MPI_FLOAT, source,
                        msgtype, MPI_COMM_WORLD, &status);
            }
            if (right != NONE) {
                MPI_Send(&u[iz][offset+rows-1][0], NYPROB, MPI_FLOAT, right,
                        LTAG, MPI_COMM_WORLD);
                source = right;
                msgtype = RTAG;
                MPI_Recv(&u[iz][offset+rows][0], NYPROB, MPI_FLOAT, source, msgtype,
                        MPI_COMM_WORLD, &status);
            }
            /* Now call update to update the value of grid points */
            update(start,end,NYPROB,&u[iz][0][0],&u[1-iz][0][0]);
            iz = 1 - iz;
        }
        /* Finally, send my portion of final results back to master */
        MPI_Send(&offset, 1, MPI_INT, MASTER, DONE, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, MASTER, DONE, MPI_COMM_WORLD);
        MPI_Send(&u[iz][offset][0], rows*NYPROB, MPI_FLOAT, MASTER, DONE,
                MPI_COMM_WORLD);
        MPI_Finalize();
    }
}
