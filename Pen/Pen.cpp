#include <windows.h>
#include <windowsx.h> // GET_X_LPARAN, GET_Y_LPARAMマクロの定義
#include <string>

static const int MAX_WIDTH = 800;
static const int MAX_HEIGHT = 600;

LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    TCHAR szAppName[] = L"PaintApp";
    WNDCLASS wc;
    HWND hwnd;
    MSG msg;

    // ウィンドウクラスの属性を設定
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = szAppName;

    // ウィンドウクラスを登録
    if (!RegisterClass(&wc)) return 0;

    // ウィンドウを作成
    hwnd = CreateWindow(
        szAppName, L"Paint",
        WS_OVERLAPPEDWINDOW,
        50, 50,
        MAX_WIDTH, MAX_HEIGHT,
        NULL, NULL,
        hInstance, NULL);

    if (!hwnd) return 0;

    // ウィンドウを表示
    ShowWindow(hwnd, nCmdShow);

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HBITMAP  hBitmap;    // ビットマップ
    static HDC      hMemDC;     // オフスクリーン

    HDC hdc;

    switch (uMsg) {
    case WM_CREATE:
        // オフスクリーンをメモリデバイスコンテキストを用いて作成
        hdc = GetDC(hwnd);
        hMemDC = CreateCompatibleDC(hdc);
        hBitmap = CreateCompatibleBitmap(hdc, MAX_WIDTH, MAX_HEIGHT);
        SelectObject(hMemDC, hBitmap);
        // オフスクリーンを白で塗りつぶす（デフォルトは黒）
        PatBlt(hMemDC, 0, 0, MAX_WIDTH, MAX_HEIGHT, WHITENESS);
        ReleaseDC(hwnd, hdc);
        return 0;
    case WM_CLOSE:
        // オフスクリーンの破棄
        DeleteDC(hMemDC);
        DeleteObject(hBitmap);
        DestroyWindow(hwnd);
        return 0;
    case WM_PAINT:
        PAINTSTRUCT paint;
        hdc = BeginPaint(hwnd, &paint);

        // ビットブロック転送
        BitBlt(
            hdc,
            0,
            0,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            hMemDC,
            paint.rcPaint.left,
            paint.rcPaint.top,
            SRCCOPY);

        // 更新領域の左上座標と幅、高さを表示
        // x, y, width, height
        /*
        swprintf_s(buf, 18, format,
            paint.rcPaint.left,
            paint.rcPaint.top,
            paint.rcPaint.right - paint.rcPaint.left,
            paint.rcPaint.bottom - paint.rcPaint.top);
        OutputDebugString(buf);
        */


        EndPaint(hwnd, &paint);

        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}