#include <stdlib.h>
#include <stdio.h>

#ifdef __WIN32

#include <windows.h>
#include <conio.h>
#include <signal.h>

//#define gotoxy(h,x,y) SetConsoleCursorPosition((h), (COORD){(x),(y)})
//#define setColor(h,c) SetConsoleTextAttribute((hHandle), (c))
#define SETCOLOR(c) SetConsoleTextAttribute((hHandle), (c))
#define GOTO_XY(x,y) SetConsoleCursorPosition((hHandle), (COORD){(x),(y)})
#define PRINT_GREEN(x) SETCOLOR(GREEN);printf("x\n")
#define SEM_WAIT(sem) WaitForSingleObject(sem,MAXLONG)
#define SEM_SIGNAL(sem) ReleaseSemaphore(sem,1,NULL)
#define SEM_CREATE(n,m) n=CreateSemaphore(NULL,m,MAXLONG,NULL)
#define SEMAPHORE HANDLE

HANDLE hHandle;

#elif __unix

#include <semaphore.h>

#define SEM_NAME shared_sem
#define SEMAPHORE sem_t
#define SEM_CREATE(n,m) sem_init(n,0,m)
#define SEM_SIGNAL(s) sem_post(s)
#define SEM_WAIT(s) sem_wait(s)

#define GOTO_XY(x,y) printf("\033[%d;%dH", x, y)
#define PRINT_GREEN(x) printf("\033[40;32m%c\033[5m", x);

sem_t SEM_NAME;

#endif

#define RAIN_LEN 15
#define HEIGHT 40
#define CS 33
#define CE 127

typedef struct _RainThread{
	int pos;
	int start;
	int end;
	int delay;
} RainThread;

void* rain_thread(void*);
void rain_randomise(RainThread *rain_thread);
void clear();
void signal_handler(int);

HANDLE handle;
PCONSOLE_SCREEN_BUFFER_INFO pbuffer_info;

semaphore main_sem;
semaphore mutex;

int main(int argc, char *argv[]){
	COORD coord;
	int i, height_max, width_max;

    signal(SIGINT, signal_handler);

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	pbuffer_info = (PCONSOLE_SCREEN_BUFFER_INFO)malloc(sizeof(CONSOLE_SCREEN_BUFFER_INFO));
	GetConsoleScreenBufferInfo(handle, pbuffer_info);

	coord = pbuffer_info->dwMaximumWindowSize;
	//printf("dwMaximumWindowSize:(%d,%d)\n", coord.X, coord.Y);

	srand(time(NULL));
	width_max = coord.X; 
	height_max = coord.Y;

	SEM_CREATE(main_sem, 0);
	SEM_CREATE(mutex, 1);

    clear();
	for(i = 0; i < width_max; i++){
		RainThread *r = (RainThread*)malloc(sizeof(RainThread));
		r->pos = i;
		rain_randomise(r);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rain_thread, (void*)r, 0, NULL);
	}

	SEM_WAIT(main_sem);

	return 0;
}

void rain_randomise(RainThread *r){
	r->delay = rand() % (r->pos+1) * 100;
	r->start = rand() % HEIGHT;
	if(r->start > HEIGHT/2) r->start = 0; 
	r->end = rand() % HEIGHT;
	if(r->end <= r->start) r->end = HEIGHT;
}

void* rain_thread(void *p){
	int i,j,tmpdelay;
	RainThread t;
	int c1, c2, c3, c4;

	t = *((RainThread*)p);
	free((RainThread*)p);
	tmpdelay = t.delay;

	while(1){
		//Sleep(tmpdelay);
		Sleep(t.delay);
		j = rand() % CE;
		if(j < CS) j += CS;
		for(i = t.start; i< t.end+RAIN_LEN; i++){
			SEM_WAIT(mutex);
			if(i < t.end){
				//gotoxy(handle, t.pos, i);
				//setColor(handle, 7);
				//printf("%c", c1);
                GOTO_XY(handle, t.pos, i);
                PRINT_COLOR(c1, 7);

				c4 = c3; c3 = c2; c2 = c1; c1 = j;
				j = rand() % CE;;
				if(j < CS) j += CS;

				if(i > 0){
					//setColor(handle, 10);
					//gotoxy(handle, t.pos, i-1);
					//printf("%c", c2);
                    GOTO_XY(t.pos, i-1);
                    PRINT_COLOR(c2, 10);
				}
				if(i > 1){
					//setColor(handle, 10);
					//gotoxy(handle, t.pos, i-2);
					//printf("%c", c3);
                    GOTO_XY(t.pos, i-2);
                    PRINT_COLOR(c3, 10);
				}
				if(i > 2){
					//setColor(handle, 2);
					//gotoxy(handle, t.pos, i-3);
					//printf("%c", c4);
                    GOTO_XY(t.pos, i-3);
                    PRINT_COLOR(c4, 2);
				}
			}

			if(i == t.end){
				//setColor(handle, 10);
				//gotoxy(handle, t.pos, i-1);
				//printf("%c", c1);
                GOTO_XY(t.pos, i-1);
                PRINT_COLOR(c1, 10);
				//setColor(handle, 2);
				//gotoxy(handle, t.pos, i-3);
				//printf("%c", c3);
                GOTO_XY(t.pos, i-3);
                PRINT_COLOR(c3, 2);
			}
			if(i == (t.end+1)){
				//setColor(handle, 2);
				//gotoxy(handle, t.pos, i-3);
				//printf("%c", c2);
                GOTO_XY(t.pos, i-3);
                PRINT_COLOR(c2, 2);
			}
			if(i == (t.end+2)){
				//setColor(handle, 2);
				//gotoxy(handle, t.pos, i-3);
				//printf("%c", c1);
                GOTO_XY(t.pos, i-3);
                PRINT_COLOR(c1, 2);
			}
	
			if(i >= RAIN_LEN){
				//gotoxy(handle, t.pos, i-RAIN_LEN);
                GOTO_XY(t.pos, i-RAIN_LEN);
				printf(" ");
			}

			SEM_SIGNAL(mutex);
	
			Sleep(50);
		}
		SEM_WAIT(mutex);
		rain_randomise(&t);
		SEM_SIGNAL(mutex);
		//tmpdelay = rand() % t.pos * 100;
	}
}

void clear(){
    DWORD cellCount;
    COORD coords = {0,0};

    cellCount = pbuffer_info->dwSize.X * pbuffer_info->dwSize.Y;
    FillConsoleOutputCharacterA(handle, (TCHAR)' ', cellCount, coords, NULL);
}

void signal_handler(int num){
    SEM_WAIT(mutex);
    clear();
    gotoxy(handle, 0,0);
    setColor(handle, 7);
    exit(num);
}

