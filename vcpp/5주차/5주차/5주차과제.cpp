#include <windows.h>

// 전역 변수
HINSTANCE hInst;
HWND hWnd;
POINT startPoint, endPoint;
BOOL isDrawing = FALSE;

// 윈도우 프로시저 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // 초기화
        return 0;

    case WM_LBUTTONDOWN:
        // 왼쪽 마우스 버튼 클릭 시 그림 그리기 시작
        isDrawing = TRUE;
        startPoint.x = LOWORD(lParam);
        startPoint.y = HIWORD(lParam);
        endPoint = startPoint;
        return 0;

    case WM_MOUSEMOVE:
        // 마우스 이동 중 그림 그리기
        if (isDrawing)
        {
            HDC hdc = GetDC(hWnd);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
            endPoint.x = LOWORD(lParam);
            endPoint.y = HIWORD(lParam);
            Rectangle(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
            ReleaseDC(hWnd, hdc);
        }
        return 0;

    case WM_LBUTTONUP:
        // 왼쪽 마우스 버튼 떼면 그림 그리기 종료
        isDrawing = FALSE;
        return 0;

    case WM_PAINT:
        // 화면 다시 그리기
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    return 0;

    case WM_DESTROY:
        // 윈도우 파괴 시 종료
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    hInst = hInstance;
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, TEXT("MyDrawingApp"), NULL };
    RegisterClassEx(&wcex);
    hWnd = CreateWindow(wcex.lpszClassName, TEXT("Simple Drawing App"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}
