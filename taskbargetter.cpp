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

    // 获取程序名字
    CHAR caption[256];
    memset(caption, 0, sizeof(caption));
    ::GetWindowTextA(hwnd, caption, 255);

    // 获取程序路径
//    std::wstring szFilePath;
//    GetProcessFilePath(hwnd, szFilePath);
//    QString path = QString::fromStdWString(szFilePath);

    LPWSTR szFilePath = NULL;
    GetFullPathByWindow(hwnd, szFilePath, MAX_PATH);
//    GetFilePathFromHandleW(hwnd, szFilePath, MAX_PATH);
    QString path = QString::fromStdWString(szFilePath);
    qDebug() << path;

    if (strcmp(caption, ""))
    {
        if (GetClassLong(hwnd, GCL_HICON))
        {
            HICON hIcon =(HICON)GetClassLong(hwnd, GCL_HICON);
            if(hIcon)
            {
                Application app{hwnd, QString::fromLocal8Bit(caption), hIcon};
                applications.append(app);
            }
        }
    }

    return TRUE;
}

/// @brief      获取指定进程所对应的可执行（EXE）文件全路径
/// @param[in]  hProcess : 进程句柄。
/// @param[out] szFilePath : 进程句柄hProcess所对应的可执行文件路径
/// @remark     hProcess必须具有PROCESS_QUERY_INFORMATION 或 PROCESS_QUERY_LIMITED_INFORMATION 权限
/// @return     获取成功返回true，其余false
bool TaskBarGetter::GetProcessFilePath(IN HANDLE hProcess, OUT std::wstring& szFilePath)
{
    szFilePath = _T("");
    TCHAR tsFileDosPath[MAX_PATH + 1];
    ZeroMemory(tsFileDosPath, sizeof(TCHAR)*(MAX_PATH + 1));
    if (0 == GetProcessImageFileNameW(hProcess, tsFileDosPath, MAX_PATH + 1)) // TODO
    {
        qDebug() << "直接return";
        return false;
    }

    // 获取Logic Drive String长度
    UINT uiLen = GetLogicalDriveStrings(0, NULL);
    if (0 == uiLen)
    {
        return false;
    }

    PTSTR pLogicDriveString = new TCHAR[uiLen + 1];
    ZeroMemory(pLogicDriveString, uiLen + 1);
    uiLen = GetLogicalDriveStrings(uiLen, pLogicDriveString);
    if (0 == uiLen)
    {
        delete[]pLogicDriveString;
        return false;
    }

    TCHAR szDrive[3] = TEXT(" :");
    PTSTR pDosDriveName = new TCHAR[MAX_PATH];
    PTSTR pLogicIndex = pLogicDriveString;

    do
    {
        szDrive[0] = *pLogicIndex;
        uiLen = QueryDosDevice(szDrive, pDosDriveName, MAX_PATH);
        if (0 == uiLen)
        {
            if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
            {
                break;
            }

            delete[]pDosDriveName;
            pDosDriveName = new TCHAR[uiLen + 1];
            uiLen = QueryDosDevice(szDrive, pDosDriveName, uiLen + 1);
            if (0 == uiLen)
            {
                break;
            }
        }

        uiLen = _tcslen(pDosDriveName);
        if (0 == _tcsnicmp(tsFileDosPath, pDosDriveName, uiLen))
        {
            wchar_t buf[1024];
            swprintf_s(buf, 1024, L"%s%s", szDrive, tsFileDosPath + uiLen);
            wchar_t *pstr = buf;
            szFilePath = std::wstring(pstr);

            break;
        }

        while (*pLogicIndex++);
    } while (*pLogicIndex);

    delete[]pLogicDriveString;
    delete[]pDosDriveName;

    return true;
}

int TaskBarGetter::GetFullPathByWindow(HWND hWnd, LPWSTR lpFilePathBuf, int nBufSize)
{
    HANDLE hProcess = NULL;
    DWORD  dwProcessId;
    int    nFilePathLen = 0;
    char*  p = NULL;

    GetWindowThreadProcessId(hWnd, &dwProcessId);
    if (dwProcessId == 0)
        return 0;

    hProcess = OpenProcess(PROCESS_VM_READ|PROCESS_QUERY_INFORMATION, TRUE, dwProcessId);
    if (hProcess == NULL)
        return 0;

    if ((nFilePathLen = GetModuleFileNameEx(hProcess, NULL, lpFilePathBuf, nBufSize)) == 0)
    {
        CloseHandle(hProcess);
        return 0;
    }

    /*p = strrchr(lpFilePathBuf, '\\');
    if (p != NULL)
    {
        p[0] = '\0';
    }

    CloseHandle(hProcess);

    return strlen(lpFilePathBuf);*/
    return  0;
}
