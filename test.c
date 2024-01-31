#include <ncurses.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define UP     1
#define DOWN  -1
#define LEFT   2
#define RIGHT -2

int dir;

struct Snake
{
	int hang;
	int lie;
	struct Snake *next;
};

struct Snake *head = NULL;
struct Snake *tail = NULL;
struct Snake food;
void initNcurse()
{
	initscr();
	keypad(stdscr,1);
	noecho();
}

int initSnakeNode(int i,int j)
{
	struct Snake *p = NULL;
	p = head;
	while(p != NULL){
		if(p->hang == i && p->lie == j){
			return 1;
		}
		p = p->next;
	}
	return 0;
}

void initFood()
{
	int x = rand()%20;
	int y = rand()%20;
	food.hang = x;
	food.lie = y;
}


int initFoodPlace(int i,int j)
{
	if(food.hang == i && food.lie == j){
		return 1;
	}
	return 0;
}

void gamePic()
{
	int hang;
	int lie;
	move(0,0);
	for(hang = 0;hang < 20;hang++){
		if(hang == 0){
			for(lie = 0;lie < 20;lie++){
				printw("--");
			}
			printw("\n");
		}

		if(hang >= 0 || hang <= 19){
			for(lie = 0; lie <= 20; lie++){
				if(lie == 0 || lie == 20){
					printw("|");
				}else if(initSnakeNode(hang,lie)){
					printw("[]");
				}else if(initFoodPlace(hang,lie)){
					printw("##");
				}else{
					printw("  ");
				}
			}
			printw("\n");
		}

		if(hang == 19){
			for(lie = 0;lie < 20;lie++){
				printw("--");
			}
			printw("\n");
			printw("By kla\n");
		}
	}
}

void addNode()
{
	struct Snake *new1;
	new1 = (struct Snake *)malloc(sizeof(struct Snake));
	new1->next == NULL;
	switch(dir){
		case UP:
			new1->hang = tail->hang - 1;
			new1->lie = tail->lie;
			break;
		case DOWN:
			new1->hang = tail->hang + 1;
			new1->lie = tail->lie;
			break;
		case LEFT:
			new1->hang = tail->hang;
			new1->lie = tail->lie - 1;
			break;
		case RIGHT:
			new1->hang = tail->hang;
			new1->lie = tail->lie + 1;
			break;
	}
	

	tail->next = new1;
	tail = new1;
	
}

void initSnake()
{
	dir = RIGHT;
	head = (struct Snake *)malloc(sizeof(struct Snake));
	head->hang = 2;
	head->lie = 2;
	head->next = NULL;

	tail = head;
	addNode();
	addNode();
}

void deleteNode()
{
	head = head->next;
}

int ifSnakeDie()
{
	if(tail->hang < 0 || tail->hang == 20 || tail->lie == 0 || tail->lie == 20){
		return 1;
	}
	struct Snake *p;
	p = head;
	while(p->next != NULL){
		if(p->hang == tail->hang && p->lie == tail->lie){
			return 1;
		}
		p = p->next;
	}
	return 0;
}

void moveSnake()
{
	addNode();
	if(initFoodPlace(tail->hang,tail->lie)){
		initFood();
	}else{
		deleteNode();
	}
	if(ifSnakeDie()){
		initSnake();
	}
}

void *refreshjiemian()
{
	while(1){
                moveSnake();
                gamePic();
                refresh();
                usleep(100000);

        }

}

void turn(int direction)
{
	if(abs(dir) != abs(direction)){
		dir = direction;
	}
}

void *changeDir()
{
	int key;
	while(1){
		key = getch();
		switch(key){
			case KEY_UP:
				turn(UP);
				break;
			case KEY_DOWN:
				turn(DOWN);
				break;
			case KEY_LEFT:
				turn(LEFT);
				break;
			case KEY_RIGHT:
				turn(RIGHT);
				break;
		}
	}
}

int main()
{
	pthread_t t1;
	pthread_t t2;
	initNcurse();
	initSnake();
	initFood();
	gamePic();
	
	pthread_create(&t1,NULL,refreshjiemian,NULL);
	pthread_create(&t2,NULL,changeDir,NULL);
	getch();
	while(1);
	endwin();
}
