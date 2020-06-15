#ifndef APPLICATION_H
#define APPLICATION_H

#include "windows.h"
#include <QIcon>

struct Application
{
    HWND hwdn;
    QString name;
    HICON icon;
};

struct ApplicationRuntime
{
    qint64 begin;
    qint64 end;
    int keb; // 开始的刻在一天中的索引
    int kee; // 结束的刻在一天中的索引
    Application app;
};

#endif // APPLICATION_H
