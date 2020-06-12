#include "taskbargetter.h"

QVector<Application> TaskBarGetter::applications;

QVector<Application> TaskBarGetter::get()
{
    applications.clear();
    EnumWindows(EnumWindowProc, (LPARAM)this);
    return applications;
}

/**
 * 参考：https://www.cnblogs.com/SaveDictator/articles/7451529.html
 */
BOOL TaskBarGetter::EnumWindowProc(HWND hwnd, LPARAM lParam)
{
    //窗口是否可视
    if (!::IsWindowVisible(hwnd))
        return TRUE;

    //窗口是否可激活
    if (!::IsWindowEnabled(hwnd))
        return TRUE;

    //确定给定窗口是否是最小化（图标化）的窗口。
    //if(IsIconic(hwnd))
      //  return TRUE;

    //窗口是否具有父窗口？
    HWND hParent = (HWND)GetWindowLong(hwnd, GWL_HWNDPARENT);
    //父窗口是否可激活？
    //据 Spy++ 观察，如“运行”对话框等被应列入列表的程序有一个隐藏的，具有 WS_DISABLED 的父窗口
    if (IsWindowEnabled(hParent))
        return TRUE;
    //父窗口是否可视？
    if (IsWindowVisible(hParent))
        return TRUE;

    LONG gwl_style = GetWindowLong(hwnd, GWL_STYLE);
    if ((gwl_style & WS_POPUP) && !(gwl_style & WS_CAPTION))
        return TRUE;

    CHAR caption[256];
    memset(caption, 0, sizeof(caption));
    ::GetWindowTextA(hwnd, caption, 255);

    if (strcmp(caption, ""))
    {
        if (GetClassLong(hwnd, GCL_HICON))
        {
            HICON hIcon =(HICON)GetClassLong(hwnd, GCL_HICON);
            if(hIcon)
            {
//                Application app{hwnd, caption, IconConverter::fromWinHICON(hIcon)};
                Application app{hwnd, QString::fromLocal8Bit(caption), hIcon};
                applications.append(app);
            }
        }
    }

    return TRUE;
}
