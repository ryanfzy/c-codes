#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

#ifdef __WIN32

#include <windows.h>
#include <conio.h>

//#define gotoxy(h,x,y) SetConsoleCursorPosition((h), (COORD){(x),(y)})
//#define setColor(h,c) SetConsoleTextAttribute((hHandle), (c))
#define SETCOLOR(c) SetConsoleTextAttribute((hHandle), (c))
#define GOTO_XY(x,y) SetConsoleCursorPosition((hHandle), (COORD){(x),(y)})

#define WHITE 7
#define GREEN 2

#define SEM_WAIT(sem) WaitForSingleObject(sem,MAXLONG)
#define SEM_SIGNAL(sem) ReleaseSemaphore(sem,1,NULL)
#define SEM_CREATE(n,m) n=CreateSemaphore(NULL,m,MAXLONG,NULL)
#define SEMAPHORE HANDLE

#define SLEEP(n) Sleep(n)

#define THREAD_CREATE(f,a) CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)f,(void*)a,0,NULL);

HANDLE _h;
void get_screen_info(SCREEN_INFO *psi){
    _h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFF_INFO buf;
    GetConsoleScreenBufferInfo(_h,&buf);
    si.width=buf.dwMaximumWindowSize.X;
    si.height=buf.dwMaximumWindowSize.Y;
}

void print_color(char ch, int color, bool intensity){
    if(intensity)
        color = color + 8;
    SetConsoleTextAttribute(hHandle, color)
    printf("%c", ch);
}

void clear(){
    DWORD cellCount;
    COORD coords = {0,0};

    cellCount = pbuffer_info->dwSize.X * pbuffer_info->dwSize.Y;
    FillConsoleOutputCharacterA(handle, (TCHAR)' ', cellCount, coords, NULL);
}

#elif __unix

#include <semaphore.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>

#define SEMAPHORE sem_t
#define SEM_CREATE(n,m) sem_init(&n,0,m)
#define SEM_SIGNAL(s) sem_post(&s)
#define SEM_WAIT(s) sem_wait(&s)

#define SLEEP(n) usleep(n)

#define THREAD_CREATE(fun,arg) pthread_t _t;pthread_create(&_t,NULL,(void*)fun,(void*)arg);

#define GOTO_XY(x,y) printf("\033[%d;%dH",y+1,x+1)

#define WHITE 37
#define GREEN 32

#endif

#define HEIGHT 40
//ANSCII code to print
#define CS 33
#define CE 127

typedef struct _RainThread{
	int pos;
	int start;
    int end;
	int delay;
} RainThread;

typedef struct _ScreenInfo{
    int width;
    int height;
} ScreenInfo;

void signal_handler(int);

//HANDLE hHandle;
//PCONSOLE_SCREEN_BUFFER_INFO pbuffer_info;

SEMAPHORE main_sem;
SEMAPHORE mutex;
int rain_len;
int max_height;
int max_width;

#ifdef __unix

void print_color(char ch, int color, bool intensity){
    if(intensity)
        printf("\033[%d;1m%c",color,ch);
    else
        printf("\033[%dm%c",color,ch);
}

void get_screen_info(ScreenInfo *psi){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    psi->width = w.ws_col;
    psi->height = w.ws_row;
}

void clear(){
    printf("\033[2J");
}

#endif

void* test_thread(void *param){
    while(true){
        int i = *((int*)param);
        printf("%d", i);
    }
}

void rain_randomise(RainThread *r){
    /*
	r->delay = rand() % (r->pos+1) * 100;
	r->start = rand() % HEIGHT;
	if(r->start > HEIGHT/2) r->start = 0; 
	r->end = rand() % HEIGHT;
	if(r->end <= r->start) r->end = HEIGHT;
    */
	r->delay = rand() % (r->pos+1) * 100;
	r->start = rand() % max_height;
	if(r->start > max_height/2) r->start = 0; 
	r->end = rand() % max_height;
	if(r->end <= r->start) r->end = max_height;

}

void* rain_thread(void *p){
    printf("here");
	int i,j,tmpdelay,k;
	RainThread t;
	int c1, c2, c3, c4;

	t = *((RainThread*)p);
	free((RainThread*)p);
	tmpdelay = t.delay;

    /*
    printf("pos(%d)\n", t.pos);
    printf("start(%d)\n", t.start);
    printf("end(%d)\n", t.end);
    printf("delay(%d)\n", t.delay);
    return;
    */

	while(1){
		//SLEEP(t.delay);
		j = rand() % CE;
		if(j < CS) j += CS;
        c1 = j;
        for(i = t.start; i < t.end+rain_len; i++){
			SEM_WAIT(mutex);
            k = i - t.start;
            //printf("here(%d)",k);
			if(i < t.end){

                GOTO_XY(t.pos, i);
                print_color(c1, WHITE, 0);

				c4 = c3; c3 = c2; c2 = c1; c1 = j;
				j = rand() % CE;
				if(j < CS) j += CS;

				//if(i > 1){
                if(k>0){
                    GOTO_XY(t.pos, i-1);
                    print_color(c2, GREEN, 1);
				}
				//if(i > 2){
                if(k>1){
                    GOTO_XY(t.pos, i-2);
                    print_color(c3, GREEN, 1);
				}
				//if(i > 3){
                if(k>2){
                    GOTO_XY(t.pos, i-3);
                    print_color(c4, GREEN, 0);
				}
			}

			if(i == t.end){
                GOTO_XY(t.pos, i-1);
                print_color(c1, GREEN, 1);
                GOTO_XY(t.pos, i-3);
                print_color(c3, GREEN, 0);
			}
			if(i == (t.end+1)){
                GOTO_XY(t.pos, i-3);
                print_color(c2, GREEN, 0);
			}
			if(i == (t.end+2)){
                GOTO_XY(t.pos, i-3);
                print_color(c1, GREEN, 0);
			}
	
			if(i >= rain_len){
                GOTO_XY(t.pos, i-rain_len);
				printf(" ");
			}

			SEM_SIGNAL(mutex);
	
            fflush(stdout);
			SLEEP(40000);
		}
		SEM_WAIT(mutex);
		rain_randomise(&t);
		SEM_SIGNAL(mutex);
		//tmpdelay = rand() % t.pos * 100;
	}
}

int main(int argc, char *argv[]){
	//COORD coord;
    ScreenInfo screen;
	int i;

    signal(SIGINT, signal_handler);

	//handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//pbuffer_info = (PCONSOLE_SCREEN_BUFFER_INFO)malloc(sizeof(CONSOLE_SCREEN_BUFFER_INFO));
	//GetConsoleScreenBufferInfo(handle, pbuffer_info);
	//coord = pbuffer_info->dwMaximumWindowSize;
	//printf("dwMaximumWindowSize:(%d,%d)\n", coord.X, coord.Y);

    get_screen_info(&screen);

	srand(time(NULL));
	max_width = screen.width; 
	max_height = screen.height;

    rain_len = max_height / 3;

	SEM_CREATE(main_sem, 0);
	SEM_CREATE(mutex, 1);

    //printf("width(%d)\n", max_width);
    //printf("height(%d)\n", max_height);

    //clear();
	for(i = 0; i < max_width; i++){
        //printf("here(%d)\n", i);
        //printf("%d\n", i);
		RainThread *r = (RainThread*)malloc(sizeof(RainThread));
		r->pos = i;
	    rain_randomise(r);
        //pthread_t thread;
        THREAD_CREATE(rain_thread, r);
        /*
        int *j = (int*)malloc(sizeof(int));
        *j = i+1;
        THREAD_CREATE(test_thread, j);
        */
		//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rain_thread, (void*)r, 0, NULL);
        //pthread_create(&thread, NULL, (void*)rain_thread, (void*)r);
        //printf("%d\n", i);
	}

	SEM_WAIT(main_sem);

	return 0;
}

void signal_handler(int num){
    SEM_WAIT(mutex);
    clear();
    //gotoxy(handle, 0,0);
    //setColor(handle, 7);
    GOTO_XY(0, 0);
    //print_color(' ', WHITE, 0);
    printf("\033[0m");
    exit(num);
}

