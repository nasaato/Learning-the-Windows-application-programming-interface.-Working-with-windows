#include "framework.h"
#include "WinTab.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hwndList;                                  // Listbox handle
HWND hwndChildList;                             // Child windows listbox handle
HWND hwndButtonCreate;                          // Create window botton handle
HWND hwndButtonClose;                           // Close window botton handle
HWND hwndButtonMoveAndResize;                   // Move and resize window botton handle
HWND hwndButtonSetStyle;                        // Set style button handle
HWND hwndButtonRefresh;                         // Refresh button handle
HWND hwndButtonSearch;                          // Search windows button handle
HWND hwndButtonRename;                          // Rename window button handle

HWND hwndXEdit, hwndYEdit, hwndWidthEdit, hwndHeightEdit, hwndTitleEdit, hwndSearchEdit, hwndStyleEdit, hwndWindowPosEdit;
HWND selectedHwnd, hwndListLabel, hwndChildListLabel, hwndTitleLabel, hwndXYLabel, hwndWithHeightLabel, hwndSearchLabel, hwndStyleLabel;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK       EnumWindowsProc(HWND hwnd, LPARAM lParam);
BOOL CALLBACK       EnumChildProc(HWND hwnd, LPARAM lParam);
void                CreateNewWindow();          // Function tot create new window
void                SearchWindows();            // Function to search windows
void                PrintWindowStyle(HWND hwnd);// Function to display selected window


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINTAB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINTAB));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

// FUNCTION: MyRegisterClass()
// PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINTAB));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINTAB);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

// FUNCTION: InitInstance(HINSTANCE, int)
// PURPOSE: Saves instance handle and creates main window
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        CW_USEDEFAULT, 0, 900, 600, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    return TRUE;
}

// FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
// PURPOSE: Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
        // Area for Get active the windows list
        hwndListLabel = CreateWindow(L"STATIC", L"Windows:", WS_CHILD | WS_VISIBLE,
            10, 0, 70, 20, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        
        hwndList = CreateWindow(L"LISTBOX", L"", WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_STANDARD | LBS_NOTIFY,
            10, 20, 400, 300, hWnd, (HMENU)IDC_LISTBOX, hInst, NULL);

        // Area for Get active child windows list the window
        hwndChildListLabel = CreateWindow(L"STATIC", L"Child windows:", WS_CHILD | WS_VISIBLE,
            10, 320, 100, 20, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

        hwndChildList = CreateWindow(L"LISTBOX", L"", WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_STANDARD,
            10, 340, 400, 200, hWnd, (HMENU)IDC_CHILD_LISTBOX, hInst, NULL);

        // Button for refresh the windows list
        hwndButtonRefresh = CreateWindow(L"BUTTON", L"Refresh", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            420, 20, 100, 30, hWnd, (HMENU)IDC_REFRESH_BUTTON, hInst, NULL);

        // Button for create new window
        hwndButtonCreate = CreateWindow(L"BUTTON", L"New window", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            545, 20, 100, 30, hWnd, (HMENU)IDC_CREATE_WINDOW, hInst, NULL);

        // Button for close the window
        hwndButtonClose = CreateWindow(L"BUTTON", L"Close", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            670, 20, 100, 30, hWnd, (HMENU)IDC_CLOSE_WINDOW, hInst, NULL);

        // Area for get and set the windows title
        hwndTitleLabel = CreateWindow(L"STATIC", L"Title:", WS_CHILD | WS_VISIBLE,
            420, 60, 50, 20, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

        hwndTitleEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            420, 80, 330, 30, hWnd, (HMENU)IDC_TITLE_EDIT, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

        hwndButtonRename = CreateWindow(L"BUTTON", L"Rename", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            770, 80, 100, 30, hWnd, (HMENU)IDC_RENAME_TITLE, hInst, NULL);

        // Area for enter move and resize
        hwndXYLabel = CreateWindow(L"STATIC", L"X:\t  Y:", WS_CHILD | WS_VISIBLE, 420, 130, 110, 20, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        hwndXEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER, 420, 150, 50, 25, hWnd, (HMENU)IDC_X_EDIT, NULL, NULL);
        hwndYEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER, 480, 150, 50, 25, hWnd, (HMENU)IDC_Y_EDIT, NULL, NULL);

        hwndWithHeightLabel = CreateWindow(L"STATIC", L"Width:\t  Height:   Pos:", WS_CHILD | WS_VISIBLE, 580, 130, 170, 20, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
        hwndWidthEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER, 580, 150, 50, 25, hWnd, (HMENU)IDC_WIDTH_EDIT, NULL, NULL);
        hwndHeightEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER, 640, 150, 50, 25, hWnd, (HMENU)IDC_HEIGHT_EDIT, NULL, NULL);
        hwndWindowPosEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_NUMBER, 700, 150, 50, 25, hWnd, (HMENU)IDC_WINDOWPOS_EDIT, NULL, NULL);

        hwndButtonMoveAndResize = CreateWindow(L"BUTTON", L"Move/resize", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            770, 145, 100, 30, hWnd, (HMENU)IDC_MOVEANDRESIZE_BUTTON, hInst, NULL);

        // Area for search window
        hwndSearchLabel = CreateWindow(L"STATIC", L"Search window:", WS_CHILD | WS_VISIBLE,
            420, 190, 110, 20, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

        hwndSearchEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            420, 210, 330, 30, hWnd, (HMENU)IDC_SEARCH_EDIT, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

        hwndButtonSearch = CreateWindow(L"BUTTON", L"Search", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            770, 210, 100, 30, hWnd, (HMENU)IDC_SEARCH_BUTTON, hInst, NULL);
        
        // Area for get and set the window style
        hwndStyleLabel = CreateWindow(L"STATIC", L"Style window:", WS_CHILD | WS_VISIBLE,
            420, 260, 110, 20, hWnd, NULL, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

        hwndStyleEdit = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            420, 280, 150, 30, hWnd, (HMENU)IDC_STYLE_EDIT, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

        hwndButtonSetStyle = CreateWindow(L"BUTTON", L"Set Style", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
            590, 280, 100, 30, hWnd, (HMENU)IDC_SET_STYLE, hInst, NULL);

        SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
        EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(hwndList));
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDC_REFRESH_BUTTON:
            selectedHwnd = NULL;
            SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
            EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(hwndList));
            SendMessage(hwndChildList, LB_RESETCONTENT, 0, 0); // Reset child list
            break;

        case IDC_CREATE_WINDOW:
            CreateNewWindow();
            break;

        case IDC_CLOSE_WINDOW:
            // Close window
            if (selectedHwnd)
            {
                SendMessage(selectedHwnd, WM_CLOSE, 0, 0);
                selectedHwnd = NULL;
                SendMessage(hwndList, LB_RESETCONTENT, 0, 0);
                EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(hwndList));
                SendMessage(hwndChildList, LB_RESETCONTENT, 0, 0); // Reset child list
            }
            break;

        case IDC_RENAME_TITLE:
        {
            if (selectedHwnd)
            {
                wchar_t newTitle[256];
                GetWindowText(hwndTitleEdit, newTitle, 256);
                SetWindowText(selectedHwnd, newTitle);
            }
        }
        break;

        case IDC_MOVEANDRESIZE_BUTTON:
        {
            if (selectedHwnd)
            {
                int x = GetDlgItemInt(hWnd, IDC_X_EDIT, NULL, TRUE);
                int y = GetDlgItemInt(hWnd, IDC_Y_EDIT, NULL, TRUE);
                int width = GetDlgItemInt(hWnd, IDC_WIDTH_EDIT, NULL, TRUE);
                int height = GetDlgItemInt(hWnd, IDC_HEIGHT_EDIT, NULL, TRUE);
                int zOrder = GetDlgItemInt(hWnd, IDC_WINDOWPOS_EDIT, NULL, TRUE);

                // Move and resize the window
                SetWindowPos(selectedHwnd, NULL, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);

                // Change the z-order
                HWND hwndInsertAfter = HWND_TOP;
                if (zOrder > 0) {
                    HWND hwnd = GetTopWindow(NULL);
                    while (hwnd && zOrder > 0) {
                        hwndInsertAfter = hwnd;
                        hwnd = GetNextWindow(hwnd, GW_HWNDNEXT);
                        zOrder--;
                    }
                }
                SetWindowPos(selectedHwnd, hwndInsertAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
            }
        }
        break;

        case IDC_SEARCH_BUTTON:
            SearchWindows();
            break;

        case IDC_SET_STYLE:
            if (selectedHwnd) {
                wchar_t styleText[16];
                GetWindowText(hwndStyleEdit, styleText, 16);
                LONG_PTR newStyle = wcstol(styleText, NULL, 16);
                SetWindowLongPtr(selectedHwnd, GWL_STYLE, newStyle);
                SetWindowPos(selectedHwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
            }
            break;

        case IDC_LISTBOX:
            if (HIWORD(wParam) == LBN_SELCHANGE) {
                int index = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
                if (index != LB_ERR) {
                    TCHAR title[256];
                    SendMessage(hwndList, LB_GETTEXT, index, (LPARAM)title);

                    // Get HWND the window to the title
                    selectedHwnd = FindWindow(NULL, title);
                    if (selectedHwnd) {
                        SetWindowText(hwndTitleEdit, title); // Set title name

                        RECT rect;
                        GetWindowRect(selectedHwnd, &rect);
                        SetDlgItemInt(hWnd, IDC_X_EDIT, rect.left, TRUE);
                        SetDlgItemInt(hWnd, IDC_Y_EDIT, rect.top, TRUE);
                        SetDlgItemInt(hWnd, IDC_WIDTH_EDIT, rect.right - rect.left, TRUE);
                        SetDlgItemInt(hWnd, IDC_HEIGHT_EDIT, rect.bottom - rect.top, TRUE);

                        // Enumerate and list child windows
                        SendMessage(hwndChildList, LB_RESETCONTENT, 0, 0); // Reset child list
                        EnumChildWindows(selectedHwnd, EnumChildProc, reinterpret_cast<LPARAM>(hwndChildList));

                        // Display window style
                        PrintWindowStyle(selectedHwnd);
                    }
                }
            }
            break;

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

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

// EnumWindows callback function
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    HWND hwndList = reinterpret_cast<HWND>(lParam);
    char title[256];
    GetWindowTextA(hwnd, title, sizeof(title));

    if (IsWindowVisible(hwnd) && strlen(title) > 0 &&
        strcmp(title, "Windows Input Experience") != 0 &&
        strcmp(title, "Program Manager") != 0 &&
        strcmp(title, "C:\\Users\\nasaa\\OneDrive\\Документы\\Rainmeter\\Skins\\Mond\\Clock\\Clock.ini") != 0)
    {
        SendMessageA(hwndList, LB_ADDSTRING, 0, (LPARAM)title);
    }

    return TRUE;
}

// EnumChildProc callback function
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
    HWND hwndList = reinterpret_cast<HWND>(lParam);
    char title[256];
    GetWindowTextA(hwnd, title, sizeof(title));

    if (IsWindowVisible(hwnd) && strlen(title) > 0)
    {
        SendMessageA(hwndList, LB_ADDSTRING, 0, (LPARAM)title);
    }

    return TRUE;
}

// Function to create a new window
void CreateNewWindow()
{
    // Register a new window class
    const wchar_t CLASS_NAME[] = L"NewWindowClass";

    WNDCLASS wc = { };

    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the new window
    HWND hwndNew = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"New Window",                  // Window title
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInst,      // Instance handle
        NULL        // Additional application data
    );

    if (hwndNew)
    {
        ShowWindow(hwndNew, SW_SHOW);
    }
}

// Function to search windows based on the input in the search edit box
void SearchWindows()
{
    wchar_t searchText[256];
    GetWindowText(hwndSearchEdit, searchText, 256);

    SendMessage(hwndList, LB_RESETCONTENT, 0, 0);

    EnumWindows([](HWND hwnd, LPARAM lParam) -> BOOL {
        HWND hwndList = reinterpret_cast<HWND>(lParam);
        char title[256];
        GetWindowTextA(hwnd, title, sizeof(title));

        wchar_t wTitle[256];
        MultiByteToWideChar(CP_ACP, 0, title, -1, wTitle, 256);

        // Get title for search
        wchar_t searchText[256];
        GetWindowText(hwndSearchEdit, searchText, 256);

        // Checking for windows exclusion "Windows Input Experience", "Program Manager" and Rainmeter Clock
        if (IsWindowVisible(hwnd) && wcslen(wTitle) > 0 &&
            wcscmp(wTitle, L"Windows Input Experience") != 0 &&
            wcscmp(wTitle, L"Program Manager") != 0 &&
            wcscmp(wTitle, L"C:\\Users\\nasaa\\OneDrive\\Документы\\Rainmeter\\Skins\\Mond\\Clock\\Clock.ini") != 0 &&
            wcsstr(wTitle, searchText) != NULL)
        {
            SendMessageW(hwndList, LB_ADDSTRING, 0, (LPARAM)wTitle);
        }

        return TRUE;
        }, reinterpret_cast<LPARAM>(hwndList));
}

void PrintWindowStyle(HWND hwnd) {
    if (hwnd) {
        LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
        wchar_t styleText[16];
        swprintf(styleText, 16, L"%08X", style);
        SetWindowText(hwndStyleEdit, styleText);

        // Get and display z-order
        HWND hwndPrev = GetNextWindow(hwnd, GW_HWNDPREV);
        int zOrder = 0;
        while (hwndPrev) {
            zOrder++;
            hwndPrev = GetNextWindow(hwndPrev, GW_HWNDPREV);
        }
        SetDlgItemInt(GetParent(hwndStyleEdit), IDC_WINDOWPOS_EDIT, zOrder, TRUE);
    }
}