/* Sequential Mandelbrot program */


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define         X_RESN  800       /* x resolution */
#define         Y_RESN  800       /* y resolution */
#define         Vibra_Limit 1024   /* Set the vibration limit */

typedef struct complextype
        {
        float real, imag;
        } Compl;


void main ()
{
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
        XGCValues       values;
        Display         *display;
        XSizeHints      size_hints;
        Pixmap          bitmap;
        XPoint          points[800];
        FILE            *fp, *fopen ();
        char            str[100];

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

       /* Mandlebrot variables */
        int i, j, k;
        Compl   z, c;
        float   lengthsq, temp;

        // Start counting time
        double totaltime;
        struct timeval t_val, t_val_end, t_result;
        gettimeofday(&t_val, NULL);

        /* Calculate and draw points */

        for(i=0; i < X_RESN; i++)
        for(j=0; j < Y_RESN; j++) {

          z.real = z.imag = 0.079468;
          c.real = ((float) j - 600)/800.0;               /* scale factors for 800 x 800 window */
          c.imag = ((float) i - 1100)/800.0;
          k = 0;

          do  {                                             /* iterate for pixel color */

            temp = z.real*z.real - z.imag*z.imag + c.real;
            z.imag = 2.0*z.real*z.imag + c.imag;
            z.real = temp;
            lengthsq = z.real*z.real+z.imag*z.imag;
            k++;

        } while (lengthsq < 4.0 && k < Vibra_Limit);

          XSetForeground (display, gc, k * 1000);
          XDrawPoint (display, win, gc, j, i);

        // if (k == Vibra_Limit) XDrawPoint (display, win, gc, j, i);

        }

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

        sleep (30);
        XFlush (display);


        /* Program Finished */

}
