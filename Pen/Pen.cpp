#include <windows.h>
#include <windowsx.h> // GET_X_LPARAN, GET_Y_LPARAMマクロの定義
#include <string>

static const int WIN_WIDTH = 800;
static const int WIN_HEIGHT = 600;

LRESULT CALLBACK WndProc(
    HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    TCHAR szAppName[] = L"PenApp";
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
        WIN_WIDTH, WIN_HEIGHT,
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
    case WM_CREATE: {
        // オフスクリーンをメモリデバイスコンテキストを用いて作成
        hdc = GetDC(hwnd);
        hMemDC = CreateCompatibleDC(hdc);
        hBitmap = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
        SelectObject(hMemDC, hBitmap);

        HBRUSH hBgBrush = CreateSolidBrush(RGB(0xff, 0xdf, 0x00));
        RECT rc = { 0, 0, WIN_WIDTH, WIN_HEIGHT };
        FillRect(hMemDC, &rc, hBgBrush);

        // ペンとブラシを作成
        HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0x00, 0x90, 0x00));
        // HPEN hPen = CreatePen(PS_DASH, 1, RGB(0x00, 0x90, 0x00));
        // HPEN hPen = (HPEN)GetStockObject(BLACK_PEN);

        HBRUSH hSolidBrush = CreateSolidBrush(RGB(0xff, 0x00, 0x00));
        HBRUSH hHatchBrush1 = CreateHatchBrush(
            HS_BDIAGONAL,
            RGB(0xff, 0x00, 0x00)
        );
        HBRUSH hHatchBrush2 = CreateHatchBrush(
            HS_FDIAGONAL,
            RGB(0xff, 0x00, 0x00)
        );
        HBRUSH hStockBrush = (HBRUSH)GetStockObject(GRAY_BRUSH);

        // ペンとブラシを選択
        SelectObject(hMemDC, hPen);
        SelectObject(hMemDC, hHatchBrush1);
        
        // オフスクリーンへ描画
        Rectangle(hMemDC, 50, 50, 200, 150);

        // 背景モード変更
        SetBkMode(hMemDC, TRANSPARENT);
        // ブラシ変更
        SelectObject(hMemDC, hHatchBrush2);
        Rectangle(hMemDC, 100, 100, 250, 200);


        Ellipse(hMemDC, 315, 230, 385, 300);
        Ellipse(hMemDC, 300, 300, 400, 400);


        // ストックオブジェクト以外は解放
        DeleteObject(hPen);
        DeleteObject(hSolidBrush);
        DeleteObject(hHatchBrush1);
        DeleteObject(hHatchBrush2);


        ReleaseDC(hwnd, hdc);
        return 0;
    }
    case WM_PAINT:
        PAINTSTRUCT paint;
        hdc = BeginPaint(hwnd, &paint);
        // ビットブロック転送
        BitBlt(
            hdc,
            0,
            0,
            WIN_WIDTH,
            WIN_HEIGHT,
            hMemDC,
            0,
            0,
            SRCCOPY);
        EndPaint(hwnd, &paint);
        return 0;
    case WM_DESTROY:
        // オフスクリーンの破棄
        DeleteDC(hMemDC);
        DeleteObject(hBitmap);

        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}