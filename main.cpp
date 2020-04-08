#include <stdio.h>
#include <time.h>
#include <graphics.h>

#define WIDTH 10
#define HEIGHT 10
#define SIZE 30
#define TITLE 1

int row, col, boom;
int **box;
int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
HWND pHwnd;
POINT pNow;
IMAGE img[14];

void menu();                               // ���˵�����
void InitGame(int row, int col, int boom); // ��ʼ����Ϸ
void ShowImage();                          // ��ͼ
void OpenZero(int x, int y);               // �ݹ��0
void PlayGame();                           // ��ʼ��Ϸ
void RePlay();                             // ���¿�ʼ��Ϸ

// ��ʼ����Ϸ
void InitGame(int row, int col, int boom)
{
	// ��ʼ����Ϸ����
	initgraph(col * SIZE, row * SIZE);
	// ����ͼƬ
	loadimage(&img[0], L"./image/0.gif", SIZE, SIZE);
	loadimage(&img[1], L"./image/1.gif", SIZE, SIZE);
	loadimage(&img[2], L"./image/2.gif", SIZE, SIZE);
	loadimage(&img[3], L"./image/3.gif", SIZE, SIZE);
	loadimage(&img[4], L"./image/4.gif", SIZE, SIZE);
	loadimage(&img[5], L"./image/5.gif", SIZE, SIZE);
	loadimage(&img[6], L"./image/6.gif", SIZE, SIZE);
	loadimage(&img[7], L"./image/7.gif", SIZE, SIZE);
	loadimage(&img[8], L"./image/8.gif", SIZE, SIZE);
	loadimage(&img[9], L"./image/flag.gif", SIZE, SIZE);
	loadimage(&img[10], L"./image/mine.gif", SIZE, SIZE);
	loadimage(&img[11], L"./image/mine2.gif", SIZE, SIZE);
	loadimage(&img[12], L"./image/blank.gif", SIZE, SIZE);
	// ��ʼ������
	int i, j;
	box = (int**)calloc(row, sizeof(int*));
	for (i = 0;i < row;++i)
	{
		box[i] = (int*)calloc(col, sizeof(int));
	}
	// �������
	int x, y;
	srand((unsigned int)time(NULL));
	for (i = 0;i < boom;)
	{
		x = rand() % row;
		y = rand() % col;
		if (box[x][y] != -1)
		{
			box[x][y] = -1;
			++i;
		}
	}
	// ����������������
	int k, nx, ny, cnt;
	for (i = 0;i < row;++i)
	{
		for (j = 0;j < col;++j)
		{
			if (box[i][j] == 0)
			{
				for (k = 0, cnt = 0;k < 8;++k)
				{
					nx = i + dx[k];
					ny = j + dy[k];
					if (nx >= 0 && nx < row && ny >= 0 && ny < col && box[nx][ny] == -1)
						++box[i][j];
				}
			}
		}
	}
	// �򵥼��� ���ܺ����� 19 - 28
	for (i = 0;i < row;++i)
	{
		for (j = 0;j < col;++j)
			box[i][j] += 20;
	}
}

// ��ͼ
void ShowImage()
{
	int i, j;
	for (i = 0;i < row;++i)
	{
		for (j = 0;j < col;++j)
		{
			if (box[i][j] == -1)
			{
				putimage(j * SIZE, i * SIZE, &img[11]);
			}
			else if (box[i][j] >= 0 && box[i][j] <= 8)
			{
				putimage(j * SIZE, i * SIZE, &img[box[i][j]]);
			}
			else if (box[i][j] >= 19 && box[i][j] <= 28)
			{
				putimage(j * SIZE, i * SIZE, &img[12]);
			}
			else if (box[i][j] > 30)
			{
				putimage(j * SIZE, i * SIZE, &img[9]);
			}
		}
	}
}

// ��0
void OpenZero(int x, int y)
{
	int nx, ny, k;
	for (k = 0;k < 8;++k)
	{
		nx = x + dx[k];
		ny = y + dy[k];
		if (nx >= 0 && nx < row && ny >= 0 && ny < col)
		{
			if (box[nx][ny] >= 20 && box[nx][ny] <= 28)
			{
				box[nx][ny] -= 20;
				if (box[nx][ny] == 0)
					OpenZero(nx, ny);
			}
		}
	}
}

// ��Ϸʧ��
int IfLose(int n)
{
	if (n == -1)
	{
		int i, j;
		for (i = 0;i < row;++i)
		{
			for (j = 0;j < col;++j)
			{
				if(box[i][j] == 19) // δ�򿪵��״�
					putimage(j * SIZE, i * SIZE, &img[10]);
			}
		}
		pNow = { 0, 0 };
		pHwnd = WindowFromPoint(pNow);
		MessageBox(pHwnd, TEXT("��Ϸʧ�ܣ�"), TEXT("Mine Sweeper"), MB_OK);
		return 1;
	}
	return 0;
}

// ��Ϸʤ��
int IfWin()
{
	int i, j;
	for (i = 0;i < row;++i)
	{
		for (j = 0;j < col;++j)
		{
			if (box[i][j] >= 20 && box[i][j] <= 28) // �������ָ���û��
			{
				return 0;
			}
		}
	}
	pNow = { 0, 0 };
	pHwnd = WindowFromPoint(pNow);
	MessageBox(pHwnd, TEXT("��Ϸʤ����"), TEXT("Mine Sweeper"), MB_OK);
	return 1;
}

// ��ʼ��Ϸ
void PlayGame()
{
	int n = 0;
	MOUSEMSG msg;
	while (1)
	{
		msg = GetMouseMsg();
		if (WM_LBUTTONDOWN == msg.uMsg)
		{
			n = box[msg.y / SIZE][msg.x / SIZE];
			if (n >= 19 && n <= 28)
			{
				n -= 20;
				box[msg.y / SIZE][msg.x / SIZE] = n;
				if (n == 0)
				{
					OpenZero(msg.y / SIZE, msg.x / SIZE);
				}
			}
		}
		else if (WM_RBUTTONDOWN == msg.uMsg)
		{
			n = box[msg.y / SIZE][msg.x / SIZE];
			if (n >= 19 && n <= 28)
				box[msg.y / SIZE][msg.x / SIZE] += 30;
			else if (n > 30)
				box[msg.y / SIZE][msg.x / SIZE] -= 30;
		}
		else
			continue;

		ShowImage();
		if (IfWin() || IfLose(n))
		{
			int re;
			pNow = { 0, 0 };
			pHwnd = WindowFromPoint(pNow);
			re = MessageBox(pHwnd, TEXT("��������һ����"), TEXT("Mine Sweeper"), MB_RETRYCANCEL);
			if (IDCANCEL == re)
			{
				exit(0);
			}
			else if (IDRETRY == re)
			{
				RePlay();
			}
		}
	}
}

// ���¿�ʼ��Ϸ
void RePlay()
{
	menu();
	InitGame(row, col, boom);
	ShowImage();
	PlayGame();
}

// ���˵�
void menu()
{
	initgraph(WIDTH * 50, HEIGHT * 50);
	loadimage(&img[13], L"./image/bg.jpg", WIDTH * 50, HEIGHT * 50);
	putimage(0, 0, &img[13]);
	settextcolor(BLACK);
	settextstyle(35, 0, _T("��Բ"));
	setbkmode(TRANSPARENT);
	fillrectangle(150, 110, 330, 170);
	outtextxy(200, 120, _T("����"));
	fillrectangle(150, 200, 330, 260);
	outtextxy(200, 210, _T("�м�"));
	fillrectangle(150, 290, 330, 350);
	outtextxy(200, 300, _T("�߼�"));

	MOUSEMSG m = { 0 };
	while (1)
	{
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x > 150 && m.x < 330)
		{
			if (m.y > 110 && m.y < 170)
			{
				row = 9;
				col = 9;
				boom = 10;
			}
			else if (m.y > 200 && m.y < 260)
			{
				row = 16;
				col = 16;
				boom = 40;
			}
			else if (m.y > 290 && m.y < 350)
			{
				row = 16;
				col = 30;
				boom = 99;
			}
			else
				continue;
			break;
		}
	}
}

void main()
{
	menu();
	InitGame(row, col, boom);
	ShowImage();
	PlayGame();
	getchar();
}