#ifndef APPLICATIONRUNTIMEMANAGER_H
#define APPLICATIONRUNTIMEMANAGER_H

#include <QVector>
#include "application.h"

class ApplicationRuntimeManager
{
public:
    ApplicationRuntimeManager();

    QString toString() const;
    void fromString(QString text);

    void analyzeApplications(const QVector<Application> &apps);

protected:
    QVector<ApplicationRuntime> historys; // 所有的开关历史记录
    QVector<ApplicationRuntime> runnings; // 当前正在运行的
};

#endif // APPLICATIONRUNTIMEMANAGER_H
