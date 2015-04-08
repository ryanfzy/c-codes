#include <stdlib.h>
#include <stdio.h>

#ifdef __WIN32

#include <windows.h>
#include <conio.h>
#include <signal.h>

//#define gotoxy(h,x,y) SetConsoleCursorPosition((h), (COORD){(x),(y)})
//#define setColor(h,c) SetConsoleTextAttribute((hHandle), (c))
#define setColor(c) SetConsoleTextAttribute((hHandle), (c))
#define GOTO_XY(x,y) SetConsoleCursorPosition((hHandle), (COORD){(x),(y)})
#define PRINT_GREEN(x) setColor(GREEN);printf("x\n");
#define sem_wait(sem) WaitForSingleObject(sem,MAXLONG)
#define sem_signal(sem) ReleaseSemaphore(sem,1,NULL)
#define sem_create(n) CreateSemaphore(NULL,n,MAXLONG,NULL);
#define semaphore HANDLE

HANDLE hHandle;

#elif __linux

#define GOTO_XY(x,y)
#define PRINT_GREEN(x) printf("\033[40;32mx\033[5m");

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

	main_sem = sem_create(0);
	mutex = sem_create(1);

    clear();
	for(i = 0; i < width_max; i++){
		RainThread *r = (RainThread*)malloc(sizeof(RainThread));
		r->pos = i;
		rain_randomise(r);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rain_thread, (void*)r, 0, NULL);
	}

	sem_wait(main_sem);

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
			sem_wait(mutex);
			if(i < t.end){
				//gotoxy(handle, t.pos, i);
				//setColor(handle, 7);
				//printf("%c", c1);
                GOTO_XY(handle, t.pos, i);
                PRINT_GREEN(c1);

				c4 = c3; c3 = c2; c2 = c1; c1 = j;
				j = rand() % CE;;
				if(j < CS) j += CS;

				if(i > 0){
					setColor(handle, 10);
					gotoxy(handle, t.pos, i-1);
					printf("%c", c2);
				}
				if(i > 1){
					setColor(handle, 10);
					gotoxy(handle, t.pos, i-2);
					printf("%c", c3);
				}
				if(i > 2){
					setColor(handle, 2);
					gotoxy(handle, t.pos, i-3);
					printf("%c", c4);
				}
			}

			if(i == t.end){
				setColor(handle, 10);
				gotoxy(handle, t.pos, i-1);
				printf("%c", c1);
				setColor(handle, 2);
				gotoxy(handle, t.pos, i-3);
				printf("%c", c3);
			}
			if(i == (t.end+1)){
				setColor(handle, 2);
				gotoxy(handle, t.pos, i-3);
				printf("%c", c2);
			}
			if(i == (t.end+2)){
				setColor(handle, 2);
				gotoxy(handle, t.pos, i-3);
				printf("%c", c1);
			}
	
			if(i >= RAIN_LEN){
				gotoxy(handle, t.pos, i-RAIN_LEN);
				printf(" ");
			}

			sem_signal(mutex);
	
			Sleep(50);
		}
		sem_wait(mutex);
		rain_randomise(&t);
		sem_signal(mutex);
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
    sem_wait(mutex);
    clear();
    gotoxy(handle, 0,0);
    setColor(handle, 7);
    exit(num);
}

