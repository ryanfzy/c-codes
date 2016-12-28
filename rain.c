#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <signal.h>

#ifdef __WIN32

#include <windows.h>
#include <conio.h>

#elif __unix

#include <semaphore.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>

#endif

#ifdef __WIN32

//#define gotoxy(h,x,y) SetConsoleCursorPosition((h), (COORD){(x),(y)})
//#define setColor(h,c) SetConsoleTextAttribute((hHandle), (c))
#define SETCOLOR(c) SetConsoleTextAttribute((hHandle), (c))
#define GOTO_XY(x,y) SetConsoleCursorPosition((_h), (COORD){(x),(y)})

#define WHITE 7
#define GREEN 2

#define SEM_WAIT(sem) WaitForSingleObject(sem,MAXLONG)
#define SEM_SIGNAL(sem) ReleaseSemaphore(sem,1,NULL)
#define SEM_CREATE(n,m) n=CreateSemaphore(NULL,m,MAXLONG,NULL)
#define SEMAPHORE HANDLE

#define TIME_FACTOR 100
#define SLEEP(n) Sleep(n)

#define THREAD_CREATE(f,a) CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)f,(void*)a,0,NULL);

#elif __unix

#define SEMAPHORE sem_t
#define SEM_CREATE(n,m) sem_init(&n,0,m)
#define SEM_SIGNAL(s) sem_post(&s)
#define SEM_WAIT(s) sem_wait(&s)

#define TIME_FACTOR 100000
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

#define NORMAL 0
#define INTENSE 1
#define FADE 2

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

SEMAPHORE main_sem;
SEMAPHORE mutex;

int rain_len;
ScreenInfo screen;

#ifdef __WIN32

HANDLE _h;
//CONSOLE_SCREEN_BUFFER_INFO _buf;
void get_screen_info(ScreenInfo *psi){
    _h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO buf;
    GetConsoleScreenBufferInfo(_h,&buf);
    psi->width=buf.dwMaximumWindowSize.X;
    // we divide the height by 2 to make it run smoothly
    psi->height=buf.dwMaximumWindowSize.Y/2;
}

void print_color(char ch, int color, bool intensity){
    if(intensity)
        color = color + 8;
    SetConsoleTextAttribute(_h, color);
    printf("%c", ch);
    SetConsoleTextAttribute(_h, WHITE);
}

void clear(){
    DWORD cellCount;
    COORD coords = {0,0};

    CONSOLE_SCREEN_BUFFER_INFO buf;
    GetConsoleScreenBufferInfo(_h,&buf);
    //cellCount = pbuffer_info->dwSize.X * pbuffer_info->dwSize.Y;
    cellCount = buf.dwSize.X * buf.dwSize.Y;
    FillConsoleOutputCharacterA(_h, (TCHAR)' ', cellCount, coords, NULL);
}

#elif __unix

//TODO: windows only support bright but not dim
//  but unix support dim, can use this feature to create better effects
void print_color(char ch, int color, bool intensity){
    if(intensity)
        printf("\033[%d;1m%c\033[0m",color,ch);
    else
        printf("\033[%dm%c\033[0m",color,ch);
}

void get_screen_info(ScreenInfo *psi){
    struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);
    psi->width = w.ws_col;
    psi->height = w.ws_row;
}

void clear(){
    printf("\033[0m");
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
	r->delay = (rand() % (r->pos+1)) * TIME_FACTOR;
	r->start = rand() % screen.height;
	if(r->start > screen.height/2)
        r->start = 0; 
	r->end = rand() % screen.height;
	if(r->end <= r->start || (r->end - r->start) < rain_len)
        r->end = screen.height;
}

void* rain_thread(void *p){
	int i,j,tmpdelay,k;
	RainThread t;
	int c1, c2, c3, c4;

	t = *((RainThread*)p);
	free((RainThread*)p);

	while(1){
		SLEEP(t.delay);
        
        //get a random char
		j = rand() % CE;
		if(j < CS) j += CS;

        c1 = j;
        for(i = t.start; i < t.end+rain_len; i++){
			SEM_WAIT(mutex);
            // hanlde the first 3 characters
			if(i < t.end){
                k = i - t.start;

                GOTO_XY(t.pos, i);
                print_color(c1, WHITE, NORMAL);

                //switch the char and get a new char
				c4 = c3; c3 = c2; c2 = c1; c1 = j;
				j = rand() % CE;
				if(j < CS) j += CS;

                if(k > 0){
                    GOTO_XY(t.pos, i-1);
                    print_color(c2, GREEN, INTENSE);
				}
                if(k > 1){
                    GOTO_XY(t.pos, i-2);
                    print_color(c3, GREEN, INTENSE);
				}
                if(k > 2){
                    GOTO_XY(t.pos, i-3);
                    print_color(c4, GREEN, NORMAL);
				}
			}

			if(i == t.end){
                GOTO_XY(t.pos, i-1);
                print_color(c1, GREEN, INTENSE);
                GOTO_XY(t.pos, i-3);
                print_color(c3, GREEN, NORMAL);
			}
			if(i == (t.end+1)){
                GOTO_XY(t.pos, i-3);
                print_color(c2, GREEN, NORMAL);
			}
			if(i == (t.end+2)){
                GOTO_XY(t.pos, i-3);
                print_color(c1, GREEN, NORMAL);
			}
	
			if(i >= rain_len){
                GOTO_XY(t.pos, i-rain_len);
				printf(" ");
			}

			SEM_SIGNAL(mutex);
	
            //must flush the buffer here
            fflush(stdout);

            SLEEP(0.4*TIME_FACTOR);
		}
		SEM_WAIT(mutex);
		rain_randomise(&t);
		SEM_SIGNAL(mutex);
	}
}

int main(int argc, char *argv[]){
	int i;

    signal(SIGINT, signal_handler);

    get_screen_info(&screen);

	srand(time(NULL));

    rain_len = screen.height / 3;

	SEM_CREATE(main_sem, 0);
	SEM_CREATE(mutex, 1);

    clear();
	for(i = 0; i < screen.width; i++){
		RainThread *r = (RainThread*)malloc(sizeof(RainThread));
		r->pos = i;
	    rain_randomise(r);
        THREAD_CREATE(rain_thread, r);
	}

	SEM_WAIT(main_sem);

	return 0;
}

void signal_handler(int num){
    SEM_WAIT(mutex);
    clear();
    GOTO_XY(0, 0);
    exit(num);
}

