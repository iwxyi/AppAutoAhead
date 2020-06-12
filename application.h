#ifndef APPLICATION_H
#define APPLICATION_H

#include "windows.h"
#include <QIcon>
#include <QDateTime>

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

/**
 * 获取当前时间戳
 */
qint64 getTimestamp()
{
    return QDateTime::currentMSecsSinceEpoch();
}

/**
 * 获取当前的刻在一天中的索引
 */
int getKe()
{
    auto time = QTime::currentTime();
    auto hour = time.hour();
    auto minute = time.minute();
    auto ke = hour * 4 + (minute+14) / 15;
    return ke;
}

/**
 * 获取时间戳的刻的索引
 */
int getKe(qint64 timestamp)
{
    auto time = QDateTime::fromMSecsSinceEpoch(timestamp).time();
    auto hour = time.hour();
    auto minute = time.minute();
    auto ke = hour * 4 + (minute+14) / 15;
    return ke;
}

#endif // APPLICATION_H
