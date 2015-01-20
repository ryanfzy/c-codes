#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#define gotoxy(h,x,y) SetConsoleCursorPosition((h), (COORD){(x),(y)})
#define setColor(h,c) SetConsoleTextAttribute((h), (c))
#define wait(sem) WaitForSingleObject(sem,MAXLONG)
#define signal(sem) ReleaseSemaphore(sem,1,NULL)
#define create(n) CreateSemaphore(NULL,n,MAXLONG,NULL);
#define semaphore HANDLE

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

HANDLE handle;
PCONSOLE_SCREEN_BUFFER_INFO pbuffer_info;

semaphore main_sem;
semaphore mutex;

int main(int argc, char *argv[]){
	COORD coord;
	int i, height_max, width_max;

	handle = GetStdHandle(STD_OUTPUT_HANDLE);
	pbuffer_info = (PCONSOLE_SCREEN_BUFFER_INFO)malloc(sizeof(CONSOLE_SCREEN_BUFFER_INFO));
	GetConsoleScreenBufferInfo(handle, pbuffer_info);

	coord = pbuffer_info->dwMaximumWindowSize;
	//printf("dwMaximumWindowSize:(%d,%d)\n", coord.X, coord.Y);

	srand(time(NULL));
	width_max = coord.X; 
	height_max = coord.Y;

	main_sem = create(0);
	mutex = create(1);

	for(i = 0; i < width_max; i++){
		RainThread *r = (RainThread*)malloc(sizeof(RainThread));
		r->pos = i;
		rain_randomise(r);
		/*
		r->delay = rand() % (i+1) * 100;
		r->start = rand() % HEIGHT;
		if(r->start > HEIGHT/2) r->start = 0; 
		r->end = rand() % HEIGHT;
		if(r->end <= r->start) r->end = HEIGHT;
		*/
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)rain_thread, (void*)r, 0, NULL);
	}

	wait(main_sem);

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
			wait(mutex);
			if(i < t.end){
				setColor(handle, 7);
				gotoxy(handle, t.pos, i);
				printf("%c", c1);

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

			signal(mutex);
	
			Sleep(50);
		}
		wait(mutex);
		rain_randomise(&t);
		signal(mutex);
		//tmpdelay = rand() % t.pos * 100;
	}
}

