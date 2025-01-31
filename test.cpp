#include <windows.h>
#include <vector>


std::string decrypt(std::vector<int> enc) {
    std::string msg;
    for (int c : enc) msg += (char)(c - 3);  
    return msg;
    //its not harmfull
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static int colorSwitch = 0;
    
    switch (uMsg) {
        case WM_CLOSE:
            PostQuitMessage(0);
            break;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
           
            HBRUSH hBrush = CreateSolidBrush(colorSwitch ? RGB(255, 0, 0) : RGB(0, 0, 255)); // Red & Blue flashing
            FillRect(hdc, &ps.rcPaint, hBrush);
            DeleteObject(hBrush);
            
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_TIMER:
            colorSwitch = !colorSwitch; 
            InvalidateRect(hwnd, NULL, TRUE);  
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::vector<int> obfText = { 89, 114, 120, 32, 75, 100, 109, 104, 100, 45, 45 }; // Obfuscated "You Hacked!"
    std::string title = decrypt(obfText);

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "MalGUI";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow("MalGUI", title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, nCmdShow);

    SetTimer(hwnd, 1, 500, NULL);

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
