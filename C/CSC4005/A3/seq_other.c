// #include <mpi.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#define X_MIN_LIM 100
#define Y_MIN_LIM 100
#define X_MAX_LIM 400
#define Y_MAX_LIM 400
#define MASS_LIM 95
#define INTERVAL 0.05
#define G 6.6742E-11
#define COLLISION_FACTOR 0.1


typedef struct body{
    double num,mass;
    double x,y;
    double vx,vy;
    double ax,ay;
    double fx,fy;
} body;

double dist(body* body1, body* body2){
    return sqrt(pow(body1->x-body2->x,2)+pow(body1->y-body2->y,2));
}

body* generate_body(int n){
    int i;
    int scaleX = (800 - (X_MIN_LIM+X_MAX_LIM))/2;
    int scaleY = (800 - (Y_MIN_LIM+Y_MAX_LIM))/2;
    body* body_list = (body*) malloc(sizeof(body) * n);
    for (i = 0; i < n; i++){
        body_list[i].x = X_MIN_LIM + rand()/(double)(RAND_MAX/(X_MAX_LIM-X_MIN_LIM)) + scaleX;
        body_list[i].y = Y_MIN_LIM + rand()/(double)(RAND_MAX/(Y_MAX_LIM-Y_MIN_LIM)) + scaleY;
        body_list[i].mass = (rand()/(double)(RAND_MAX /MASS_LIM)+5)* 1E15;
        body_list[i].vx = body_list[i].vy = body_list[i].ax = body_list[i].ay = body_list[i].fx = body_list[i].ay =0;
        body_list[i].num = i;
    }
    return body_list;
}

void draw_body(body* body_list, int n, Display *display, Window win, GC gc){
    int i;
    XClearWindow(display,win);
    for (i = 0; i < n; i++){
        XDrawPoint(display,win,gc,(int)floor(body_list[i].x), (int)floor(body_list[i].y));
    }
    XFlush(display);
    usleep(1E4);
}

void check_col_body(body* body1, body* body2){
    if (dist(body1,body2) <= 1){
//        double vx1 = body1->vx;
//        double vy1 = body1->vy;
        body1->vx = ((body1->mass - body2->mass)*body1->vx + 2*body2->mass*body2->vx)/(body1->mass+body2->mass)*COLLISION_FACTOR;
        body1->vy = ((body1->mass - body2->mass)*body1->vy + 2*body2->mass*body2->vy)/(body1->mass+body2->mass)*COLLISION_FACTOR;
//        body2->vx = ((body2->mass - body1->mass)*body2->vx + 2*body1->mass*vx1)/(body1->mass+body2->mass)*COLLISION_FACTOR;
//        body2->vy = ((body2->mass - body1->mass)*body2->vy + 2*body1->mass*vy1)/(body1->mass+body2->mass)*COLLISION_FACTOR;
    }
}

void check_col_wall(body* body, double x, double y,double vx,double vy) {
    int scaleX = (800 - (X_MIN_LIM + X_MAX_LIM)) / 2,
            scaleY = (800 - (Y_MIN_LIM + Y_MAX_LIM)) / 2,
    /*Position of the walls*/
            wxr = X_MAX_LIM + scaleX,
            wxl = X_MIN_LIM + scaleX,
            wyu = Y_MAX_LIM + scaleY,
            wyl = Y_MIN_LIM + scaleY;

    if (x > wxr) {
        // body_list[i].x -= X_MAX_LIM-X_MIN_LIM;
        body->x = 2 * wxr - x;
        body->vx = -vx * COLLISION_FACTOR;
    } else if (x < wxl) {
        body->x = 2 * wxl - x;
        body->vx = -vx * COLLISION_FACTOR;
    } else {
        body->x = x;
        body->vx = vx;
    }

    if (y > wyu) {
        // body_list[i].y -= Y_MAX_LIM-Y_MIN_LIM;
        body->y = 2 * wyu - y;
        body->vy = -vy * COLLISION_FACTOR;
    } else if (y < wyl) {
        // body_list[i].y += Y_MAX_LIM-Y_MIN_LIM;
        body->y = 2 * wyl - y;
        body->vy = -vy * COLLISION_FACTOR;
    } else {
        body->y = y;
        body->vy = vy;
    }
}

void calculate_force(body* b, body* temp_list, int n){
    int j;
    double ds = pow(floor((X_MAX_LIM-X_MIN_LIM)/sqrt(n)/2-0.1),2);
    double rs,dx,dy;
    b->fx = b->fy = 0;
    for (j = 0; j < n; j++){
//        check_col_body(&body_list[i],&body_list[j]);
        dx = temp_list[(int)b->num].x - temp_list[j].x;
        dy = temp_list[(int)b->num].y - temp_list[j].y;
        rs = pow(dx, 2) + pow(dy, 2);
        b->fx += -(G * temp_list[(int)b->num].mass * temp_list[j].mass) * dx / pow(rs+ds,1.5);
        b->fy += -(G * temp_list[(int)b->num].mass * temp_list[j].mass) * dy / pow(rs+ds,1.5);
    }
}

body* next_state(body* body_list, int n){
    double vx,vy,x,y;
    body* temp_list = (body*) malloc(sizeof(body)*n);
    memcpy(temp_list, body_list, sizeof(body)*n);
    int i;
    for (i = 0; i<n; i++){
        calculate_force(&body_list[i],temp_list,n);
        body_list[i].ax = body_list[i].fx * INTERVAL/ body_list[i].mass;
        body_list[i].ay = body_list[i].fy * INTERVAL/ body_list[i].mass;

        vx = body_list[i].vx + body_list[i].ax * INTERVAL;
        vy = body_list[i].vy + body_list[i].ay * INTERVAL;

        x = (vx + body_list[i].vx)*INTERVAL/2 + body_list[i].x;
        y = (vy + body_list[i].vy)*INTERVAL/2 + body_list[i].y;

        check_col_wall(&body_list[i],x,y,vx,vy);
    }
    free(temp_list);
    return body_list;
}

int main(int argc, char ** argv)
{

    Window          win;                            /* initialization for a window */
    int             width, height,                  /* window size */
            x, y,                           /* window position */
            border_width,                   /*border width in pixels */
            display_width, display_height,  /* size of screen */
            screen;                         /* which screen */

    char            *window_name = "N-Body", *display_name = NULL;
    GC              gc;
    unsigned long   valuemask = 0;
    XGCValues       values;
    Display         *display;
    XSizeHints      size_hints;
    Pixmap          bitmap;
    XPoint          points[800];
    FILE            *fp, *fopen ();
    char            str[100];

    XSetWindowAttributes attr[1];

    /* connect to Xserver */

    if ((display = XOpenDisplay (display_name)) == NULL) {
        fprintf (stderr, "drawon: cannot connect to X server %s\n",
                 XDisplayName (display_name) );
        exit (-1);
    }

    /* get screen size */

    screen = (unsigned int) DefaultScreen (display);
    display_width = (unsigned int) DisplayWidth (display, screen);
    display_height = (unsigned int) DisplayHeight (display, screen);

    /* set window size */

    width = 800;
    height = 800;

    /* set window position */

    x = 10;
    y = 10;

    /* create opaque window */

    unsigned long black = BlackPixel (display, screen);
    unsigned long white = WhitePixel (display, screen);
    border_width = 4;
    win = XCreateSimpleWindow (display, RootWindow (display, screen),
                               x, y, width, height, border_width,
                               black,black);

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

    XSetForeground (display, gc, white);
    XSetLineAttributes (display, gc, 1, LineSolid, CapRound, JoinRound);

    attr[0].backing_store = Always;
    attr[0].backing_planes = 1;
    attr[0].backing_pixel = BlackPixel(display, screen);

    XChangeWindowAttributes(display, win, CWBackingStore | CWBackingPlanes | CWBackingPixel, attr);

    XMapWindow (display, win);
    XSync(display, 0);
    XEvent event;
    XSelectInput(display,win, KeyPressMask|KeyReleaseMask);

    /* Main Program */
    int i,j,k;
    int n = atoi(argv[1]);
    body* body_list;
    body_list = generate_body(n);

    while(1){
        if (XCheckMaskEvent(display, KeyPressMask, &event)){
            if ((int)event.xkey.keycode == 20){
                break;
            }else{
                XNextEvent(display,&event);
            }

            if (event.type == KeyRelease){
                continue;
            }
        }
        body_list = next_state(body_list,n);

        draw_body(body_list, n, display, win, gc);
    }
}
