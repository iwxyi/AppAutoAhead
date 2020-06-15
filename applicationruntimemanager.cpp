#include "applicationruntimemanager.h"

ApplicationRuntimeManager::ApplicationRuntimeManager()
{

}

QString ApplicationRuntimeManager::toString() const
{
    QString text;

    return text;
}

void ApplicationRuntimeManager::fromString(QString text)
{

}

void ApplicationRuntimeManager::analyzeApplications(const QVector<Application> &apps)
{
    // 初始化
    if (runnings.isEmpty())
    {
        auto timestamp = TimeUtil::getTimestamp();
        auto ke = TimeUtil::getKe();
        foreach (auto app, apps)
        {
            runnings.append(ApplicationRuntime{timestamp, 0, ke, 0, app});
        }
        return ;
    }

    // 传进来新的，跟现有的进行比较

}
