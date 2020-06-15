#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <QDateTime>

class TimeUtil
{
public:
    /**
     * 获取当前时间戳
     */
    static qint64 getTimestamp()
    {
        return QDateTime::currentMSecsSinceEpoch();
    }

    /**
     * 获取当前的刻在一天中的索引
     */
    static int getKe()
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
    static int getKe(qint64 timestamp)
    {
        auto time = QDateTime::fromMSecsSinceEpoch(timestamp).time();
        auto hour = time.hour();
        auto minute = time.minute();
        auto ke = hour * 4 + (minute+14) / 15;
        return ke;
    }
};

#endif // TIMEUTIL_H
