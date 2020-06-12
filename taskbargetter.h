#ifndef TASKBARGETTER_H
#define TASKBARGETTER_H

//#include "windows.foundation.h"
#include "windows.h"
#include <QtWinExtras/qwinfunctions.h>
#include <QString>
#include <QIcon>
#include <QVector>
#include <QDebug>
#include "iconconverter.h"
#pragma comment(lib, "user32.lib")

struct Application
{
    HWND hwdn;
    QString name;
    HICON icon;
};

class TaskBarGetter
{
public:
    QVector<Application> get();

private:
    static QVector<Application> applications;

    static BOOL CALLBACK EnumWindowProc(HWND hwnd, LPARAM lParam);
};

#endif // TASKBARGETTER_H
