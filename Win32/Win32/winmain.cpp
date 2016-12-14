#include<windows.h>
#include"resource.h"
#include<time.h>

LRESULT CALLBACK MY_WNDPROC(HWND, UINT, WPARAM, LPARAM);


HBITMAP hbitmap;
HBITMAP Ubitmap;
HBITMAP Dbitmap;
HBITMAP Lbitmap;
HBITMAP Rbitmap;
HBITMAP Sbitmap;
HBITMAP BEAbitmap;

typedef struct NODE
{
	int h;
	int l;
	struct NODE *pnext;
	struct NODE *prev;

}Snake,Dou;

typedef enum FangXiang{上,下,左,右}FX;
FX fx={左};

Dou dou;
Snake snake;
Snake *pTop=NULL;
Snake *pEnd=NULL;

void MY_loadbitmap(HINSTANCE hinstance);
void SHowAll(HDC hdc);
void ShowSnake(HDC hdc);
void ShowDou(HDC hdc);
void ShowBack(HDC hdc);

void INITsnack();
void AddSnakeNode(int m_x,int m_y);

void PA();
bool ChiDouMA();
void Chuxindou();
bool ZHuangqiang();
bool Ziyao();

int CALLBACK WinMain(HINSTANCE hinstance,HINSTANCE prvhinsatnce,LPSTR lpCmdLine,int nCmdShow )
{
	//设计窗口
	WNDCLASSEX EX;
	HWND hwnd;
	MSG msg;//用于存储捕获到的消息
	//加载位图
	MY_loadbitmap( hinstance);
	//初始化豆与蛇的位置
	dou.h=3;
	dou.l=4;
	snake.h=6;
	snake. l=8;



	EX.cbSize=sizeof(EX);
	EX.cbClsExtra=0;
	EX.cbWndExtra=0;
	EX.style=0;//CS_HREDRAW|CS_VREDRAW;//shui ping shua xin yu chuizhi shua xin 
	EX.hbrBackground=CreateSolidBrush(RGB(300,158,691));
	EX.hCursor=0;
	EX.hIcon=0;
	EX.hInstance=hinstance;
	EX.hIconSm=0;
	EX.lpszClassName=L"zpp";
	EX.lpszMenuName=NULL;
	EX.lpfnWndProc=MY_WNDPROC;//处理消息
	//注册窗口
	RegisterClassEx(&EX);
	//创建窗口
	hwnd=CreateWindow(L"zpp",L"zppwindows",WS_OVERLAPPEDWINDOW,100,200,630,630,NULL,NULL,hinstance,NULL);
	//显示窗口
	ShowWindow(hwnd,nCmdShow);

	INITsnack();

	//消息循环
	//1. 获得消息
	while(GetMessage(&msg,0,/* 不传hwnd 则捕获整个屏幕的消息，否则只捕获hwnd这个窗口的消息*/0,0))
	{
		//2. 翻译消息
		TranslateMessage(&msg);
		//3. 处理消息
		DispatchMessage(&msg);//该函数调度一个消息给窗口程序。通常调度从GetMessage取得的消息。
	}

}

LRESULT CALLBACK MY_WNDPROC(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)//msg 处理函数
{
	HDC hdc;
	HDC temphdc;
	PAINTSTRUCT ps;
	static int x=50;
	HBRUSH newbrush;
	HBRUSH oldbrush;
	switch (msg)
	{
		/*case WM_CREATE:
			SetTimer (hwnd, 1, 100, NULL) ;
			break;
		case WM_TIMER:
			hdc=GetDC(hwnd);
			newbrush=CreateSolidBrush(RGB(1,2,3));
			oldbrush=(HBRUSH)SelectObject(hdc,newbrush);
			Rectangle(hdc,0,0,500,300);
			newbrush=(HBRUSH)SelectObject(hdc,oldbrush);
			Ellipse(hdc,x,150,60+x,60);
			x+=10;
			DeleteObject(newbrush);
			ReleaseDC(hwnd,hdc);
			break;*/
	case WM_CREATE:
		SetTimer(hwnd,1,500,NULL);
		break;
	case WM_TIMER:
		hdc=GetDC(hwnd);
		PA();
		if(ChiDouMA())
		{
			Chuxindou();
		}
		if(Ziyao()||ZHuangqiang())
		{
			PostQuitMessage(0);
		}
		SHowAll(hdc);
		ReleaseDC(hwnd,hdc);
		break;
	case WM_KEYDOWN:	
		switch (wparam)
		{
		case VK_LEFT:
			fx=左;
			break;
		case VK_RIGHT:
			fx=右;
			break;
		case VK_UP:
			fx=上;
			break;
		case VK_DOWN:
			fx=下;
			break;
		}
		break;
		case WM_PAINT:
			hdc = BeginPaint (hwnd, &ps) ;
			//Ellipse(hdc, 20,20, 60, 60);
			SHowAll(hdc);
			EndPaint (hwnd,&ps) ;
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		/*case WM_RBUTTONDOWN:
			hdc=GetDC(hwnd);
			Ellipse(hdc, 200,80, 60, 60);*/
			/*break; */
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);//函数功能：该函数调用缺省的窗口过程来为应用程序没有处理的任何窗口消息提供缺省的处理。该函数确保每一个消息得到处理。调用DefWindowProc函数时使用窗口过程接收的相同参数。
}


void MY_loadbitmap(HINSTANCE hinstance)
{
hbitmap=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP1));
Ubitmap=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP5));
Dbitmap=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP7));
Lbitmap=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP6));
Rbitmap=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP4));
Sbitmap=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP3));
BEAbitmap=LoadBitmap(hinstance,MAKEINTRESOURCE(IDB_BITMAP2));
}

void SHowAll(HDC hdc)
{
ShowBack( hdc);
ShowDou( hdc);
ShowSnake( hdc);
}
void ShowSnake(HDC hdc)
{
	HDC temphdc;
	Snake *jPtop=pTop->pnext;
	temphdc=CreateCompatibleDC(hdc);
	switch (fx)
	{
	case 上:
		SelectObject(temphdc,Ubitmap);
		break ;
	case 下:
		SelectObject(temphdc,Dbitmap);

		break;
	case 左 :
		SelectObject(temphdc,Lbitmap);

		break;
	case 右:
		SelectObject(temphdc,Rbitmap);

		break;
	default:
		break;
	}
	BitBlt(hdc,pTop->h*30+30,pTop->l*30+30,30,30,temphdc,0,0,SRCCOPY);
	DeleteObject(temphdc);
	temphdc=CreateCompatibleDC(hdc);
	SelectObject(temphdc,Sbitmap);
	while(jPtop!=NULL)
	{
		BitBlt( hdc,jPtop->h*30+30,jPtop->l*30+30,30,30,temphdc ,0,0,SRCCOPY);
		jPtop=jPtop->pnext;
	}
	DeleteObject(temphdc);

}
void ShowDou(HDC hdc)
{
	HDC temphdc;
	temphdc=CreateCompatibleDC(hdc);
	SelectObject(temphdc,BEAbitmap);
	BitBlt(hdc,dou.h*30+30,dou.l*30+30,30,30,temphdc,0,0,SRCCOPY);
	DeleteObject(temphdc);
}

void ShowBack(HDC hdc)
{
	HDC temphdc;
	temphdc=CreateCompatibleDC (hdc) ;//chuang jian jian rong xing dc shi wei le jiejue yuanshi dc jiazai huanman de shnapin wenti 
	SelectObject (temphdc, hbitmap) ;
	BitBlt(hdc,0,0,600,600,temphdc,0,0,SRCCOPY);
	DeleteObject(temphdc);

}

void INITsnack()
{
	int i=0;
	for(i;i<3;i++)
	{
		AddSnakeNode(i+9,10);//初始从第九行第十咧开始
	}

}
void AddSnakeNode(int m_h,int m_l)
{
	Snake *s =(Snake *)malloc(sizeof(Snake));
	s->pnext=NULL;
	s->prev=NULL;
	s->h=m_h;
	s->l=m_l;
	if(pTop==NULL)
	{
		pTop=s;

	}else
	{
		pEnd->pnext=s;
		s->prev=pEnd;
	}
	pEnd=s;

}



void PA()
{
	Snake *jpend=pEnd;
	while(jpend->prev!=NULL)
	{
		jpend->h=jpend->prev->h;
		jpend->l=jpend->prev->l;
		jpend=jpend->prev;
	}
	switch(fx)
	{
	case 上:
		pTop->l-=1;
		break;
	case 下:
		pTop->l+=1;
		break;
	case 左:
		pTop->h-=1;
		break;
	case 右:
		pTop->h+=1;
		break;
	}
	
}
bool ChiDouMA()
{
	if(dou.h==pTop->h&&dou.l==pTop->l)
	{
		AddSnakeNode(pEnd->h,pEnd->l);
		return 1;
	}
return 0;
}
void Chuxindou()
{
	srand(time(0));
	int x=rand()%20-1;
	int y=rand()%20-1;
	dou.h=x;
	dou.l=y;
}
bool ZHuangqiang()
{
	if(pTop->h==0||pTop->h==20||pTop->l==0||pTop->l==20)
	{
		return 1;
	}
	return 0;
}
bool Ziyao()
{

	return 0;
}
