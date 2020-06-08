#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <curses.h>
#include <termios.h>
#include <fcntl.h>
#include <pthread.h>

#define NUM_THREAD 10
#define ROW 10
#define COLUMN 50
char map[ROW+10][COLUMN];
pthread_mutex_t mapLock;
bool winFlag = false, qFlag = false, runFlag = true;
int logs[ROW + 10];
int draft[ROW + 10];

struct Node{
	int x , y;
	Node( int _x , int _y ) : x( _x ) , y( _y ) {};
	Node(){} ;
} frog ;

// Determine a keyboard is hit or not. If yes, return 1. If not, return 0.
int kbhit(void){
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);

	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);

	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}


void *logs_move( void *t ){
    /*  Move the logs  */
	/*  Check keyboard hits, to change frog's position or quit the game. */
	/*  Check game's status  */
	/*  Print the map on the screen  */
    int id = *((int*)&t);
    while (runFlag) {
        pthread_mutex_lock(&mapLock);

        /* Capture the keyboard instructions */
        if( kbhit() ){
			char dir = getchar() ;
			switch (dir) {
                case 'w' :
                    frog.x--;
                    break;
                case 's' :
                    frog.x++;
                    if (frog.x > ROW) {
                        frog.x = ROW;
                    }
                    break;
                case 'a' :
                    frog.y--;
                    break;
                case 'd' :
                    frog.y++;
                    break;
                case 'q' :
                    runFlag = false;
                    qFlag = true;
                    break;
            }
		}
         /* Capturing ends */

        /* initiallize this thread's line */
        int i, j;
        for (i = 0; i < COLUMN - 1; i++) {
            map[id][i] = ' ';
        }

        for(i = 0, j = logs[id]; i < 15; ++i, ++j){
			map[id][ j % (COLUMN - 1) ] = '=' ;
		}

        for( j = 0; j < COLUMN - 1; ++j ) {
            map[0][j] = map[ROW][j] = '|';
        }
        /* initiallizing ends */

        /* update the log in this thread */
        if (id % 2 == 0) {
            logs[id] = (logs[id] + 1) % (COLUMN - 1);

            // making the frog moving with the log
            if (frog.x == id && map[frog.x][frog.y] == '=') {
                frog.y++;
            }
        } else {
            logs[id] = (logs[id] - 1) % (COLUMN - 1);
            if(logs[id] < 0) {
                logs[id] += (COLUMN - 1);
            }

            // making the frog moving with the log
            if (frog.x == id && map[frog.x][frog.y] == '=') {
                frog.y--;
            }
        }
        /* Updating ends */

        // This is only for testing
        //printf("%d, %d\n", frog.x, frog.y);

        /* Checking states and change flags */
        if (frog.x == ROW) {
            if (frog.y < 0) {
                frog.y = 0;
            } else if (frog.y == COLUMN - 1) {
                frog.y = COLUMN - 2;
            }
        } else if (frog.x == 0) {
            runFlag = false;
            winFlag = true;
        } else if (frog.y < 0 || frog.y > COLUMN - 1) {
            runFlag = false;
            winFlag = false;
        } else if (map[frog.x][frog.y] == ' ') {
            runFlag = false;
            winFlag = false;
        }
        /* checking ends */

        /* displaying the result */
        if (runFlag) {
            map[frog.x][frog.y] = '0';

            printf("\033[0;0H\033[2J");

            usleep(1000);
            for( i = 0; i <= ROW; ++i){
                puts(map[i]);
            }
        }
        /* displaying ends*/

        pthread_mutex_unlock(&mapLock);
        usleep(draft[id] * 5000);
    }
    pthread_exit(NULL);
}

int main( int argc, char *argv[] ){
    /* Initialize all the variables */
    pthread_t threads[NUM_THREAD];
    pthread_mutex_init(&mapLock, NULL);

	// Initialize the river map and frog's starting position
	memset( map , 0, sizeof( map ) ) ;
	long i , j ;
	for( i = 1; i < ROW; ++i ){
		for( j = 0; j < COLUMN - 1; ++j ){
            map[i][j] = ' ' ;
        }
        logs[i] = 0;
        draft[i] = rand() % 20 + 10;
	}

	for( j = 0; j < COLUMN - 1; ++j ) {
        map[0][j] = map[ROW][j] = '|';
    }

	frog = Node( ROW, (COLUMN-1) / 2 ) ;
	map[frog.x][frog.y] = '0' ;

    //Print the map into screen
    for(i = 0; i <= ROW; ++i) {
        puts(map[i]);
    }

	/*  Create pthreads for wood move and frog control.  */

    for(i = 1; i < NUM_THREAD; i++) {
        pthread_create(&threads[i], NULL, logs_move, (void *)i);
        usleep(200) ;
    }

    for(i = 1; i < NUM_THREAD; i++){
		pthread_join(threads[i], NULL);
	}

    /* Creating and joining ends */

	/*  Display the output for user: win, lose or quit.  */
    printf("\033[0;0H\033[2J");
    if (qFlag){
        puts("You exit the game.");
    } else if (winFlag) {
        puts("You win the game!!");
    } else {
        puts("You lose the game!!");
    }
    /* displaying ends */

	pthread_mutex_destroy(&mapLock); // destroy mutex
	pthread_exit(NULL) ;	//exit

	return 0;

}
