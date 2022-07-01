#define _CRT_SECURE_NO_WARNINGS -1
#include <stdio.h>

#include <graphics.h>  // easyx
#include <conio.h>

#include <time.h>  // srand

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")   // ���mciSendString�޷�ʶ�������

/*******�ṹ�嶨��*******/
// ��������
typedef struct point
{
	int x;
	int y;
}Point;
// ��
struct Snake
{
	Point xy[100];  // ����
	int direction;  // ����
	int num;        // ����
}snake;
// food
struct
{
	Point fdxy;  // ����
	int flag;    // ���
	int grade;   // ����
}food;

// ö�٣�����
enum position{up, down, left, right};

/***********ģ�黯��ƹ���************/
// ����
// ��ʼ����
void initSnake()
{
	// ǰ���ڣ�����
	snake.xy[2].x = 0;
	snake.xy[2].y = 0;
	snake.xy[1].x = 10;
	snake.xy[1].y = 0;
	snake.xy[0].x = 20;
	snake.xy[0].y = 0;
	// ����
	snake.direction = right;
	snake.num = 3;
}
// ����
void drawSnake()
{
	// ����������
	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(BLACK);
		setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}
}
// �ƶ���
void moveSnake()
{
	// ����任
	for (int i = snake.num - 1; i > 0; i--) 
	{
		snake.xy[i].x = snake.xy[i - 1].x; snake.xy[i].y = snake.xy[i - 1].y;
	}
	//��ͷ���ƶ�
	switch (snake.direction)
	{
	case up:
		snake.xy[0].y -= 10; 
		break;
	case down:
		snake.xy[0].y += 10; 
		break;
	case left:
		snake.xy[0].x -= 10; 
		break;
	case right:
		snake.xy[0].x += 10; 
		break;
	}
}
//��������
void keyDown()
{
	char userKey = _getch();
	switch (userKey)
	{
		case 'w':
		case 'W':
		case 72:
			if (snake.direction != down)
				snake.direction = up;
			break;
		case 's':
		case 'S':
		case 80:
			if (snake.direction != up)
				snake.direction = down;
			break;
		case 'a':
		case 'A':
		case 75:
			if (snake.direction != right)
				snake.direction = left;
			break;
		case 'd':
		case 'D':
		case 77:
			if (snake.direction != left)
				snake.direction = right;
			break;
	}
}
// ��ʳ��
// ��ʼ��ʳ��
void initFood()
{
	food.fdxy.x = rand() % 80 * 10;
	food.fdxy.y = rand() % 60 * 10;
	food.flag = 1;//1�������
	//ʳ�ﲻ�ܳ������ߵ�����
	for (int i = 0; i < snake.num; i++)
	{
		if (food.fdxy.x == snake.xy[i].x && food.fdxy.y == snake.xy[i].y)
		{
			food.fdxy.x = rand() % 80 * 10;
			food.fdxy.y = rand() % 60 * 10;
		}
	}
}
// ��ʳ��
void drawFood()
{
	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(BLACK);
		setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		fillrectangle(food.fdxy.x, food.fdxy.y, food.fdxy.x + 10, food.fdxy.y + 10);
	}
}
// ��ʳ��
void eatFood()
{
	if (snake.xy[0].x == food.fdxy.x && snake.xy[0].y == food.fdxy.y)
	{
		snake.num++;
		food.flag = 0;
		food.grade += 10;
	}
}
//��ʾ����
void showGrade() 
{
	//��ʽ����ӡ
	char grade[20] = "";
	sprintf(grade,"grade: %d", food.grade); 
	settextcolor(LIGHTGREEN);
	settextstyle(25, 0, "����"); 
	outtextxy(600, 50, grade);
}
// ײǽ����ɱ
HWND hwnd = NULL;  // ���ھ��
int SnakeDie()
{
	// ײǽ
	if (snake.xy[0].x > 800 || snake.xy[0].y > 600 || snake.xy[0].x < 0 || snake.xy[0].y < 0)
	{
		mciSendString("close 1.mp3", 0, 0, 0);
		MessageBox(hwnd, "ײǽ��,��Ϸ����", "gameover", MB_OK); 
		return 1;
	}
	// ��ɱ
	for (int i = 1; i < snake.num; i++)
	{
		if (snake.xy[i].x == snake.xy[0].x && snake.xy[i].y == snake.xy[0].y)
		{
			mciSendString("close 1.mp3", 0, 0, 0);
			MessageBox(hwnd, "��ɱ��,��Ϸ����", "gameover", MB_OK);
			return 1;
		}
	}
	return 0;
}
int main()
{
	srand((unsigned int)time(NULL));
	// ����ͼ�δ��ڣ���ʼ��ͼ�λ���
	initgraph(800, 600);
	setbkcolor(RGB(110, 120, 119));  // ���ñ�����ɫ
	cleardevice(); // ����

	mciSendString("open 1.mp3", 0, 0, 0); 
	mciSendString("play 1.mp3", 0, 0, 0);

	initSnake();
	drawSnake();
	while (1)
	{
		cleardevice(); // ����
		moveSnake();
		drawSnake();
		if (food.flag == 0)
			initFood();
		if (_kbhit())
			keyDown();
		drawFood();
		eatFood();
		showGrade();
		if (SnakeDie())
			break;
		Sleep(100);   // �ɵ�����Ϸ�Ѷ�
	}
	_getch();  // ��ֹ�������ȴ��û�����һ���ַ�
	closegraph();

	return 0;
}