#ifndef TASKBARGETTER_H
#define TASKBARGETTER_H

//#include "windows.foundation.h"
#include "windows.h"
#include <stdlib.h>
#include <QtWinExtras/qwinfunctions.h>
#include <QString>
#include <QIcon>
#include <QVector>
#include <QDebug>
#include "iconconverter.h"
#include "application.h"
#include "cstring"
#include "tchar.h"
#include "psapi.h"
#include "tchar.h"
#include "handle2path.h"
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "psapi.lib")

class TaskBarGetter
{
public:
    QVector<Application> get();

private:
    static QVector<Application> applications;

    static BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam);

    static bool GetProcessFilePath(IN HANDLE hProcess, OUT std::wstring& szFilePath);
    static int GetFullPathByWindow(HWND hWnd, LPWSTR lpFilePathBuf, int nBufSize);
};

#endif // TASKBARGETTER_H
