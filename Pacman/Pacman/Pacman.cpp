// Pacman.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "Pacman.h"
#include "GObject.h"
#include "GMap.h"

#define MAX_LOADSTRING 100
#define WLENTH 700
#define	WHEIGHT 740
#define STAGE_COUNT 3
#define PI (3.1415926f)


// 全局变量:
static HWND g_hwnd;                             //游戏句柄
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PACMAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PACMAN));
	// 当前的关卡
	int s_n = 0; // [0, 1, 2]
	// 地图
	GMap *MapArray[STAGE_COUNT] = { new Stage_1(), new Stage_2(), new Stage_3() };

	//玩家对象
	//自己
	auto g_me = std::make_shared<PacMan>(P_ROW, P_ARRAY);
	//设定4个敌人对象
	auto e1 = std::make_shared<RedOne>(E_ROW, E_ARRAY);
	auto e2 = std::make_shared<RedOne>(E_ROW, E_ARRAY);
	auto e3 = std::make_shared<BlueOne>(E_ROW, E_ARRAY);
	auto e4 = std::make_shared<YellowOne>(E_ROW, E_ARRAY);

	//关卡
	GObject::pStage = MapArray[s_n];
	
	//设定玩家
	Enermy::player = g_me;

	MSG msg;

	DWORD dwLastTime = 0;

	// 主消息循环
	while (!g_me->IsOver() && s_n < STAGE_COUNT) {
		// 判断是否赢得比赛
		if (g_me->IsWin()) {
			s_n++;												// 移动到下一关
			// 重设自己和敌人位置
			g_me->SetPosition(P_ROW, P_ARRAY);
			e1->SetPosition(E_ROW, E_ARRAY);					// 设置敌军一的位置
			e2->SetPosition(E_ROW, E_ARRAY); 					// 设置敌军二的位置
			e3->SetPosition(E_ROW, E_ARRAY); 					// 设置敌军三的位置
			e4->SetPosition(E_ROW, E_ARRAY); 					// 设置敌军四的位置
			// 判断是否完成了3关,如果完成,退出游戏,否则进入下一关
			if (s_n < 3) {
				MessageBox(g_hwnd, _T("恭喜过关"), _T("吃豆子提示"), MB_OK);
				GObject::pStage = MapArray[s_n];				// 
				RECT screenRect;
				screenRect.top = 0;
				screenRect.left = 0;
				screenRect.right = WLENTH;
				screenRect.bottom = WHEIGHT;

				HDC hdc = GetDC(g_hwnd);						// 获取设备
				std::shared_ptr<HDC__> dc(hdc, [](HDC hdc) {	// 智能指针，自动管理资源
					::ReleaseDC(g_hwnd, hdc);
				});
				::FillRect(dc.get(), &screenRect, CreateSolidBrush(RGB(255, 255, 255)));
				GObject::pStage->DrawMap(hdc);					// 画地图
				continue; 									// 继续进行循环
			}
			else {
				// 跳出循环
				break;
			}
		}
		// 获取消息
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {				// WM_QUIT消息，退出循环。
				break;
			}
			TranslateMessage(&msg);					// 翻译消息
			DispatchMessage(&msg);					// 分发消息
		}

		// 判断时间,否则画得太快
		if (GetTickCount() - dwLastTime >= 40) {
			dwLastTime = GetTickCount();						// 记住本次的时间
		}
		else {
			continue; 										// 时间不到,本次不进行绘画
		}
		HDC hdc = ::GetDC(g_hwnd); 					// 获得设备
		std::shared_ptr<HDC__> dc(hdc, [](auto hdc) {	// 不使用时自动释放
			::ReleaseDC(g_hwnd, hdc);					// 释放设备
		});
		MapArray[s_n]->DrawPeas(hdc);					// 画豆子
		MapArray[s_n]->DrawMap(hdc);					// 画地图

		// 画敌人及自动运动
		{
			e1->action();								// 敌军一的行为函数
			e1->DrawBlank(hdc);						// 画敌军一的空白
			e1->Draw(hdc);							// 画敌军一的主体部分

			e2->action();								// 敌军一的行为函数
			e2->DrawBlank(hdc); 						// 画敌军一的空白
			e2->Draw(hdc); 							// 画敌军一的主体部分

			e3->action();								// 敌军一的行为函数
			e3->DrawBlank(hdc); 						// 画敌军一的空白
			e3->Draw(hdc); 							// 画敌军一的主体部分

			e4->action();								// 敌军一的行为函数
			e4->DrawBlank(hdc); 						// 画敌军一的空白
			e4->Draw(hdc); 							// 画敌军一的主体部分
		}

		{
			// 画自己
			g_me->DrawBlank(hdc);
			g_me->Draw(hdc);
			// 自己向前移动
			g_me->action();

			// 获取按键 : 控制自己的方向
			if (GetAsyncKeyState(VK_DOWN) & 0x8000) {	// 检测到下方向键按下
				g_me->SetTwCommand(DOWN);				// 设置下一步的移动方向为向下
			}
			if (GetAsyncKeyState(VK_LEFT) & 0x8000) {	// 检测到左方向键按下
				g_me->SetTwCommand(LEFT); 				// 设置下一步的移动方向为向左
			}
			if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {	// 检测到右方向键按下
				g_me->SetTwCommand(RIGHT); 			// 设置下一步的移动方向为向右
			}
			if (GetAsyncKeyState(VK_UP) & 0x8000) {		// 检测到上方向键按下
				g_me->SetTwCommand(UP); 				// 设置下一步的移动方向为向上
			}
		}
	}
	// 如果游戏结束
	if (g_me->IsOver()) {
		MessageBoxA(NULL, "出师未捷", "吃豆子提示", MB_OK);
	}
	// 否则,提示赢得游戏
	else {
		MessageBoxA(NULL, "恭喜您赢得了胜利\r\n确定后游戏退出", "吃豆子提示", MB_OK);
	}

	return (int)msg.wParam;

}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PACMAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PACMAN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      500, 250, WLENTH, WHEIGHT, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   } 

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   g_hwnd = hWnd; //把当前窗口保存成游戏窗口

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
