// Editor.Window.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//



#include "framework.h"
#include "Editor.Window.h"
#include "resource.h"
#include <iostream>

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

bool isDrawing = false;
static int startX, startY;
static int oldX, oldY;

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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      0, 0, 1600, 900, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

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
    
    HDC hdc;
    //SetTextAlign(hdc, TA_CENTER); // 가운데 정렬
    PAINTSTRUCT ps;
    static TCHAR str[256] = { 0, };
    int strLen;
    
    
    switch (message)
    {
    
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
    case WM_CHAR:
    {
        strLen = _tcslen(str);
        str[strLen] = (TCHAR)wParam;
        str[strLen + 1] = '\0';
        InvalidateRect(hWnd, NULL, FALSE);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        isDrawing = true;
        startX = LOWORD(lParam);
        startY = HIWORD(lParam);

        oldX = startX;
        oldY = startY;
        return 0;
      
    }
    case WM_MOUSEMOVE:
    {
        if (isDrawing == false)
        {
            return 0;
        }
        HDC hdc = GetDC(hWnd);

        int nowX = LOWORD(lParam);
        int nowY = HIWORD(lParam);

        // 이전에 그렸던 직선을 배경색으로 다시 그려줌
        HPEN oldPen = (HPEN)SelectObject(hdc, GetStockObject(WHITE_PEN));
        MoveToEx(hdc, startX, startY, NULL);
        LineTo(hdc, oldX, oldY);

        // 새로운 직선은 검은색으로 그림
        SelectObject(hdc, oldPen);
        MoveToEx(hdc, startX, startY, NULL);
        LineTo(hdc, nowX, nowY);

        oldX = nowX;
        oldY = nowY;

        ReleaseDC(hWnd, hdc);
        return 0;
    }
    case WM_LBUTTONUP:
    {
        isDrawing = false;
        return 0;
    }
    case WM_KEYDOWN:
    {
        //hdc = GetDC(hWnd);
        //SetTextAlign(hdc, TA_CENTER); // 가운데 정렬, 한번 출력되고 난후 초기값(왼쪽정렬로) 초기화 진행
        //TextOut(hdc, 100, 100, TEXT("Hello"), _tcslen(TEXT("Hello")));
        //ReleaseDC(hWnd, hdc);
        //return 0;
        //글씨가 출력되는것처럼 보이나, 출력된 정보가 따로 저장되지 않음
        //따라서 윈도우의 화면이 출력범위를 벗어날경우 그림정보가 지워지게 됨
        //이것을 방지하기 위해 WM_PAINT를 활용하여 그림정보를 저장함
        
    }
    case WM_PAINT:
        {
        //DC란 화면 출력에 필요한 모든 정보를 가지는 데이터 구조체
        //GDI 모듈에 의해 관리된다.
        //어떤 폰트, 어떤 선의 굵기를 정해줄건가, 어떤 색상을 그려줄건가
        //화면 출력에 필요한 모든 경우는 win api에선 DC를 통해 그려줄수 있다.
            
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        hdc = BeginPaint(hWnd, &ps);
        TextOut(hdc, 100, 100, str, _tcslen(str));
        EndPaint(hWnd, &ps);
        return 0;
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
