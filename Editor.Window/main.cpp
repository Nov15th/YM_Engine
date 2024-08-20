// Editor.Window.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//



#include "framework.h"
#include "Editor.Window.h"
#include "resource.h"
#include <Windows.h>
#include <random>
#include <time.h>
using namespace std;

#define MAX_LOADSTRING 100

//#pragma comment (linker, "/entry:WinMainCRTStartup /subsystem:console")
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.

WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

typedef struct {
    COLORREF color;
    POINT pos;
    POINT velocity;
    int radius;
} ball_t;

ball_t balls[500];
int ball_size = sizeof(balls) / sizeof(balls[0]);

void DrawLine(HDC hdc, int startX, int startY, int endX, int endY)
{
    MoveToEx(hdc, startX, startY, NULL);
    LineTo(hdc, endX, endY);
}

//int MessageBox(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,     //프로그램의 인스턴스 핸들
                     _In_opt_ HINSTANCE hPrevInstance, // 바로앞에 실행된 현재 프로그램의 인스턴스 핸들, 없을경우 NULL, 지금은 신경쓰지 않아도 되는 행
                     _In_ LPWSTR    lpCmdLine,      //명령행으로 입력된 프로그램 인수
                     _In_ int       nCmdShow)       //프로그램이 실행될 형태이며, 보통 모양정보등이 전달된다.
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    //깃허브 테스트
    
    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_EDITORWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_EDITORWINDOW));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    //윈도우 클래스 생성
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(MY_ICON));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDR_MENU1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    
    //레지스터 등록
    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.
   //윈도우 핸들 생성 및 받아오기
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 1600, 900, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   //윈도우 띄우기
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    
    switch (message)
    {
    case WM_CREATE:
        srand((unsigned int)time(NULL));
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);

        for (int i = 0; i < ball_size; ++i)
        {
            // 랜덤 위치에서 생성
            balls[i].pos.x = rand() % clientRect.right;
            balls[i].pos.y = rand() % clientRect.bottom;

            // +- (1 ~ 5) 사이의 랜덤 속도로 설정
            balls[i].velocity.x = (rand() % 5 + 1) * (rand() % 2 == 0 ? -1 : 1);
            balls[i].velocity.y = (rand() % 5 + 1) * (rand() % 2 == 0 ? -1 : 1);

            // 공의 크기 (10, 15 ... ~ 30) 까지 간격 5씩 랜덤으로 설정
            balls[i].radius = rand() % 5 * 5 + 10;

            // 공 색깔 랜덤으로 설정
            balls[i].color = RGB(rand() % 255, rand() % 255, rand() % 255);
        }

        // 0.025초 간격으로 타이머 설정
        SetTimer(hWnd, 1, 25, NULL);
        break;
    case WM_TIMER:
    {
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        for (int i = 0; i < ball_size; ++i)
        {
            // 충돌하면 위치 조정하고, 방향 반전시킴
            {
                // 왼쪽 충돌
                if (balls[i].pos.x <= balls[i].radius)
                {
                    balls[i].pos.x = balls[i].radius;
                    balls[i].velocity.x *= -1;
                }
                // 오른쪽 충돌
                if (balls[i].pos.x >= clientRect.right - balls[i].radius)
                {
                    balls[i].pos.x = clientRect.right - balls[i].radius;
                    balls[i].velocity.x *= -1;
                }
                // 위쪽 충돌
                if (balls[i].pos.y <= balls[i].radius)
                {
                    balls[i].pos.y = balls[i].radius;
                    balls[i].velocity.y *= -1;
                }
                // 아래쪽 충돌
                if (balls[i].pos.y >= clientRect.bottom - balls[i].radius)
                {
                    balls[i].pos.y = clientRect.bottom - balls[i].radius;
                    balls[i].velocity.y *= -1;
                }
            }

            // 정해진 방향으로 공 이동
            balls[i].pos.x += balls[i].velocity.x;
            balls[i].pos.y += balls[i].velocity.y;
        }
        InvalidateRect(hWnd, NULL, FALSE);
        break;
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다:
            //switch (wmId)
            //{            
            //case IDM_ABOUT:
            //    DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            //    break;
            //case IDM_EXIT:
            //    DestroyWindow(hWnd);
            //    break;
            //case ID_FILE_INFO:
            //default:
            //    return DefWindowProc(hWnd, message, wParam, lParam);
            //}
            switch (wmId)
            {
            case ID_FILE_EXIT:
                if (MessageBox(hWnd, TEXT("정말로 종료하시겠습니까?"), TEXT("경고"), MB_YESNO | MB_ICONWARNING) == IDYES)
                {
                    DestroyWindow(hWnd);
                }
                break;
            case ID_VER1_VER1:
                MessageBox(hWnd, TEXT("버전 1.1 입니다."), TEXT("정보"), MB_OK | MB_ICONINFORMATION);
                break;
            case ID_INFO_VER2:
                MessageBox(hWnd, TEXT("현재 버전은 준비 중입니다."), TEXT("에러"), MB_OK | MB_ICONERROR);
                break;
            }
            return 0;
        }
        break;
   
    case WM_PAINT:
    {

        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        HDC memDC = CreateCompatibleDC(hdc);
        RECT clientRect;
        GetClientRect(hWnd, &clientRect);
        FillRect(memDC, &clientRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
        
        HBITMAP memBitmap = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
        HBITMAP rollbackBitmap = (HBITMAP)SelectObject(memDC, memBitmap);
        
        // 공 그리기
        for (int i = 0; i < ball_size; ++i)
        {
            // 공 색깔의 브러쉬 생성 및 선택
            SelectObject(memDC, GetStockObject(DC_BRUSH));
            SetDCBrushColor(memDC, balls[i].color);

            // x, y를 중점으로 한 원 출력
            Ellipse(memDC, balls[i].pos.x - balls[i].radius, balls[i].pos.y - balls[i].radius,
                balls[i].pos.x + balls[i].radius, balls[i].pos.y + balls[i].radius);
        }

        BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);

        DeleteObject(SelectObject(memDC, rollbackBitmap));
        DeleteDC(memDC);


        EndPaint(hWnd, &ps);


        
    
        break;
    }
    case WM_DESTROY:
        KillTimer(hWnd, 1);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
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
