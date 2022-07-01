#define _CRT_SECURE_NO_WARNINGS -1
#include <stdio.h>

#include <graphics.h>  // easyx
#include <conio.h>

#include <time.h>  // srand

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")   // 解决mciSendString无法识别的问题

/*******结构体定义*******/
// 坐标属性
typedef struct point
{
	int x;
	int y;
}Point;
// 蛇
struct Snake
{
	Point xy[100];  // 坐标
	int direction;  // 方向
	int num;        // 长度
}snake;
// food
struct
{
	Point fdxy;  // 坐标
	int flag;    // 标记
	int grade;   // 分数
}food;

// 枚举：方向
enum position{up, down, left, right};

/***********模块化设计功能************/
// 玩蛇
// 初始化蛇
void initSnake()
{
	// 前三节：坐标
	snake.xy[2].x = 0;
	snake.xy[2].y = 0;
	snake.xy[1].x = 10;
	snake.xy[1].y = 0;
	snake.xy[0].x = 20;
	snake.xy[0].y = 0;
	// 方向
	snake.direction = right;
	snake.num = 3;
}
// 画蛇
void drawSnake()
{
	// 绘制填充矩形
	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(BLACK);
		setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		fillrectangle(snake.xy[i].x, snake.xy[i].y, snake.xy[i].x + 10, snake.xy[i].y + 10);
	}
}
// 移动蛇
void moveSnake()
{
	// 坐标变换
	for (int i = snake.num - 1; i > 0; i--) 
	{
		snake.xy[i].x = snake.xy[i - 1].x; snake.xy[i].y = snake.xy[i - 1].y;
	}
	//蛇头的移动
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
//按键操作
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
// 玩食物
// 初始化食物
void initFood()
{
	food.fdxy.x = rand() % 80 * 10;
	food.fdxy.y = rand() % 60 * 10;
	food.flag = 1;//1代表存在
	//食物不能出现在蛇的身上
	for (int i = 0; i < snake.num; i++)
	{
		if (food.fdxy.x == snake.xy[i].x && food.fdxy.y == snake.xy[i].y)
		{
			food.fdxy.x = rand() % 80 * 10;
			food.fdxy.y = rand() % 60 * 10;
		}
	}
}
// 画食物
void drawFood()
{
	for (int i = 0; i < snake.num; i++)
	{
		setlinecolor(BLACK);
		setfillcolor(RGB(rand() % 255, rand() % 255, rand() % 255));
		fillrectangle(food.fdxy.x, food.fdxy.y, food.fdxy.x + 10, food.fdxy.y + 10);
	}
}
// 吃食物
void eatFood()
{
	if (snake.xy[0].x == food.fdxy.x && snake.xy[0].y == food.fdxy.y)
	{
		snake.num++;
		food.flag = 0;
		food.grade += 10;
	}
}
//显示分数
void showGrade() 
{
	//格式化打印
	char grade[20] = "";
	sprintf(grade,"grade: %d", food.grade); 
	settextcolor(LIGHTGREEN);
	settextstyle(25, 0, "楷体"); 
	outtextxy(600, 50, grade);
}
// 撞墙或自杀
HWND hwnd = NULL;  // 窗口句柄
int SnakeDie()
{
	// 撞墙
	if (snake.xy[0].x > 800 || snake.xy[0].y > 600 || snake.xy[0].x < 0 || snake.xy[0].y < 0)
	{
		mciSendString("close 1.mp3", 0, 0, 0);
		MessageBox(hwnd, "撞墙了,游戏结束", "gameover", MB_OK); 
		return 1;
	}
	// 自杀
	for (int i = 1; i < snake.num; i++)
	{
		if (snake.xy[i].x == snake.xy[0].x && snake.xy[i].y == snake.xy[0].y)
		{
			mciSendString("close 1.mp3", 0, 0, 0);
			MessageBox(hwnd, "自杀了,游戏结束", "gameover", MB_OK);
			return 1;
		}
	}
	return 0;
}
int main()
{
	srand((unsigned int)time(NULL));
	// 创建图形窗口：初始化图形环境
	initgraph(800, 600);
	setbkcolor(RGB(110, 120, 119));  // 设置背景颜色
	cleardevice(); // 清屏

	mciSendString("open 1.mp3", 0, 0, 0); 
	mciSendString("play 1.mp3", 0, 0, 0);

	initSnake();
	drawSnake();
	while (1)
	{
		cleardevice(); // 清屏
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
		Sleep(100);   // 可调整游戏难度
	}
	_getch();  // 防止闪屏：等待用户输入一个字符
	closegraph();

	return 0;
}