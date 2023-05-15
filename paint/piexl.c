#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <Windowsx.h>

// note: paint app current issue paint not on top of mouse
// sol: pt gets screen pos not window pos (solved)

// next: change brush size. (done)

// next: ?

int x = 0;
int size = 1;
int erase_pre = 0;
void create_buttons(HWND hwnd);
void paint_fun(HWND hwnd);

const char g_szClassName[] = "myWindowClass";

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
        create_buttons(hwnd);
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_LBUTTONDOWN:
        x = 1;
        break;

    case WM_LBUTTONUP:
        x = 0;
        break;

        // case WM_MOUSEHOVER:
        //      paint_fun(paint_wnd);
        // break;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1:
            size = size + 1;
            break;

        case 2:
            size = size - 1;
            break;

        case 3:
            erase_pre = 1;
            //   size = 1;
            break;

        case 4:
            erase_pre = 0;
            //   size = 1;
            break;
        }
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow)
{

    WNDCLASSEX wc;
    HWND hwnd, paint_wnd, test; //,paint_wnd_decoy;
    MSG Msg;

    // Step 1: Registering the Window Class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "The title of my window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 600, 502,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
                   MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    paint_wnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,                       // Predefined class; Unicode assumed
        "paint",                             // Button text
        WS_VISIBLE | WS_CHILD | WS_DLGFRAME, // Styles
        5,                                   // x position
        5,                                   // y position
        450,                                 // Button width
        450,                                 // Button height
        hwnd,                                // Parent window
        NULL,                                // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Step 3: The Message Loop
    while (GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        paint_fun(paint_wnd);
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}

void create_buttons(HWND hwnd)
{
    HWND size_up, size_down, erase, draw;

    size_up = CreateWindow(
        "BUTTON",                                 // Predefined class; Unicode assumed
        "+",                                      // Button text
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        500,                                      // x position
        10,                                       // y position
        20,                                       // Button width
        20,                                       // Button height
        hwnd,                                     // Parent window
        (HMENU)1,                                 // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.

    size_down = CreateWindow(
        "BUTTON",                                 // Predefined class; Unicode assumed
        "-",                                      // Button text
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        500,                                      // x position
        50,                                       // y position
        20,                                       // Button width
        20,                                       // Button height
        hwnd,                                     // Parent window
        (HMENU)2,                                 // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.

    erase = CreateWindow(
        "BUTTON",                                 // Predefined class; Unicode assumed
        "erase",                                  // Button text
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        500,                                      // x position
        90,                                       // y position
        70,                                       // Button width
        20,                                       // Button height
        hwnd,                                     // Parent window
        (HMENU)3,                                 // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.

    draw = CreateWindow(
        "BUTTON",                                 // Predefined class; Unicode assumed
        "paint",                                  // Button text
        WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Styles
        500,                                      // x position
        130,                                      // y position
        70,                                       // Button width
        20,                                       // Button height
        hwnd,                                     // Parent window
        (HMENU)4,                                 // No menu.
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL); // Pointer not needed.
}

void paint_fun(HWND hwnd)
{
    int r = 0, g = 0, b = 0;
    if (x == 1)
    {

        if (size < 1)
        {
            size = 1;
        }

        if (erase_pre == 1)
        {
            r = 255;
            b = 255;
            g = 255;
        }
        else
        {
            r = 0;
            b = 255;
            g = 0;
        }

        POINT pt;
        RECT rect_an;
        GetCursorPos(&pt);
        ScreenToClient(hwnd, &pt);
        HDC hDC = GetDC(hwnd);

        if (size != 1)
        {
            rect_an.left = pt.x + (size - 1);
            rect_an.right = pt.x - (size - 1);
            rect_an.top = pt.y - (size - 1);
            rect_an.bottom = pt.y + (size - 1);

            FillRect(hDC, &rect_an, CreateSolidBrush(RGB(r, g, b)));
        }
        else
        {
            SetPixel(hDC, pt.x, pt.y, RGB(r, g, b));
        }

        // for(int i = -1 * ((size - 1) / 2); i <= ((size - 1) / 2); i++)
        // {

        //     for(int j = -1 * ((size - 1) / 2); j <= ((size - 1) / 2); j++)
        //     {

        //       SetPixel(hDC,pt.x + j,pt.y + i,RGB(r,g,b));
        //     }
        // }

        // printf("x:%d  , y:%d \n",(int)pt.x,(int)pt.y);
        // printf("%d",m);
    }
    // WIP

    else if (x == 0)
    {

        if (size < 1)
        {
            size = 1;
        }

        if (erase_pre == 1)
        {
            r = 255;
            b = 0;
            g = 255;
        }
        else
        {
            r = 0;
            b = 254;
            g = 0;
        }

        POINT pt = {0, 0};
        RECT rect_an;
        static POINT pt_prev = {0, 0};
        GetCursorPos(&pt);
        ScreenToClient(hwnd, &pt);
        // printf("pt = %d                ,pt_ prev = %d\n", pt.x, pt_prev.x);
        HDC hDC = GetDC(hwnd);

        if (pt.x != pt_prev.x || pt.y != pt_prev.y)
        {

            if (GetPixel(hDC, pt_prev.x, pt_prev.y) == RGB(r, g, b))
            {
                if (size != 1)
                {
                    rect_an.left = pt_prev.x + size;
                    rect_an.right = pt_prev.x - size;
                    rect_an.top = pt_prev.y - size;
                    rect_an.bottom = pt_prev.y + size;
                    // printf("r = %d  ,  b = %d  ,  g = %d\n", r_pre, b_pre, g_pre);

                    FillRect(hDC, &rect_an, CreateSolidBrush(RGB(255, 255, 255)));
                }
                else
                {
                    // printf("r = %d  ,  b = %d  ,  g = %d\n", r_pre, b_pre, g_pre);
                    SetPixel(hDC, pt_prev.x, pt_prev.y, RGB(255, 255, 255));
                }
            }
            // else if (GetPixel(hDC, pt_prev.x, pt_prev.y) == RGB(0, 0, 255))
            // {
            //     if (size != 1)
            //     {
            //         rect_an.left = pt_prev.x + size;
            //         rect_an.right = pt_prev.x - size;
            //         rect_an.top = pt_prev.y - size;
            //         rect_an.bottom = pt_prev.y + size;
            //         // printf("r = %d  ,  b = %d  ,  g = %d\n", r_pre, b_pre, g_pre);

            //         FillRect(hDC, &rect_an, CreateSolidBrush(RGB(0, 0, 255)));
            //     }
            //     else
            //     {
            //         // printf("r = %d  ,  b = %d  ,  g = %d\n", r_pre, b_pre, g_pre);
            //         SetPixel(hDC, pt_prev.x, pt_prev.y, RGB(0, 0, 255));
            //     }
            // }
        }

        if (size != 1)
        {
            if (GetPixel(hDC, pt.x + size, pt.y) == RGB(255, 255, 255)  &&  GetPixel(hDC, pt.x - size, pt.y) == RGB(255, 255, 255)  && GetPixel(hDC, pt.x, pt.y + size) == RGB(255, 255, 255)  &&  GetPixel(hDC, pt.x, pt.y - size) == RGB(255, 255, 255) &&  GetPixel(hDC, pt.x + size, pt.y - size) == RGB(255, 255, 255) &&  GetPixel(hDC, pt.x - size, pt.y - size) == RGB(255, 255, 255) &&  GetPixel(hDC, pt.x + size, pt.y + size) == RGB(255, 255, 255) &&  GetPixel(hDC, pt.x - size, pt.y + size) == RGB(255, 255, 255))
            {
                rect_an.left = pt.x + (size - 1);
                rect_an.right = pt.x - (size - 1);
                rect_an.top = pt.y - (size - 1);
                rect_an.bottom = pt.y + (size - 1);

                FillRect(hDC, &rect_an, CreateSolidBrush(RGB(r, g, b)));
                pt_prev.x = pt.x;
                pt_prev.y = pt.y;
            }
        }
        else
        {
            if (GetPixel(hDC, pt.x, pt.y) == RGB(255, 255, 255))
            {
                SetPixel(hDC, pt.x, pt.y, RGB(r, g, b));

                pt_prev.x = pt.x;
                pt_prev.y = pt.y;
            }
        }

        //     // for(int i = 0; i < size; i++)
        //     // {
        //     //     for(int j = 0; j < size; j++)
        //     //     {
        //     //       SetPixel(hDC,pt.x + j,pt.y + i,RGB(r,g,b));
        //     //       if(pt_prev.x  != pt.x || pt_prev.y  != pt.y)
        //     //       {
        //     //          SetPixel(hDC,pt_prev.x + j,pt_prev.y + i,RGB(255,255,255));
        //     //       }
        //     //     }
        //     // }
    }

    /* (0,0)    (0,1)     (0,2)

       (1,0)    (1,1)     (1,2)

       (2,0)    (2,1)     (2,2)


        -1          0           1

       (-1,-1)    (-1,0)     (-1,1)      -1

       (0,-1)    (0,0)     (0,1)        0

       (1,-1)    (0,1)     (1,1)         1


         -2       -1        0         1        2
       (0,0)    (0,1)     (0,2)     (0,2)    (0,2)
       (1,0)    (1,1)     (1,2)     (1,2)    (1,2)
       (2,0)    (2,1)     (0,0)     (2,2)    (2,2)
       (2,0)    (2,1)     (2,2)     (2,2)    (2,2)
       (2,0)    (2,1)     (2,2)     (2,2)    (2,2)

    */
}
