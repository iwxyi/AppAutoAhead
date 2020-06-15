#include "handle2path.h"


EXTERN_C BOOL GetVolumeNameByHandle(HANDLE hFile, LPWSTR szVolumeName, UINT cchMax)
{
    BOOL bResult = FALSE;
    WCHAR szBuf[500] = { 0 };
    WCHAR * pIter = szBuf;
    int i = 0;
    BY_HANDLE_FILE_INFORMATION stFileInfo = { 0 };

    do
    {
        if(FALSE == GetFileInformationByHandle(hFile, &stFileInfo)) {
            break;
        }

        if(0 == GetLogicalDriveStringsW(_countof(szBuf), szBuf)) {
            break;
        }

        for(; pIter; pIter+=4)
        {
            DWORD dwVolumeSerialNumber = 0;

            if(GetVolumeInformationW(pIter, NULL, 0, &dwVolumeSerialNumber,
                NULL, NULL, NULL, 0))
            {
                if(dwVolumeSerialNumber == stFileInfo.dwVolumeSerialNumber)
                {
                    lstrcpynW(szVolumeName, pIter, cchMax);
                    bResult = TRUE;
                    break;
                }
            }
        }

    } while (FALSE);

    return bResult;
}



typedef struct _IO_STATUS_BLOCK {
    LONG Status;
    LONG Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _FILE_NAME_INFORMATION {
    ULONG FileNameLength;
    WCHAR FileName[MAX_PATH];
} FILE_NAME_INFORMATION;

__declspec(dllimport) LONG __stdcall ZwQueryInformationFile (
                                                             IN HANDLE FileHandle,
                                                             OUT PIO_STATUS_BLOCK IoStatusBlock,
                                                             OUT PVOID FileInformation,
                                                             IN ULONG FileInformationLength,
                                                             IN ULONG FileInformationClass
                                                             );

typedef LONG (__stdcall * PFN_ZwQueryInformationFile) (
                                                       IN HANDLE FileHandle,
                                                       OUT PIO_STATUS_BLOCK IoStatusBlock,
                                                       OUT PVOID FileInformation,
                                                       IN ULONG FileInformationLength,
                                                       IN ULONG FileInformationClass
                                                       );


EXTERN_C BOOL GetFilePathFromHandleW(HANDLE hFile, LPWSTR lpszPath, UINT cchMax)
{
    BOOL bResult = FALSE;
    WCHAR szValue[MAX_PATH] = { 0 };

    IO_STATUS_BLOCK    isb = { 0 };
    FILE_NAME_INFORMATION fni = { 0 };
    HANDLE hNtDll = NULL;
    PFN_ZwQueryInformationFile pfn_ZwQueryInformationFile = NULL;

    do
    {
        if (INVALID_HANDLE_VALUE==hFile || NULL==lpszPath || 0==cchMax) {
            break;
        }

        hNtDll = GetModuleHandle(_T("ntdll.dll"));
        if (NULL == hNtDll) {
            break;
        }

        pfn_ZwQueryInformationFile = (PFN_ZwQueryInformationFile)
            GetProcAddress((HMODULE)hNtDll, "ZwQueryInformationFile");
        if (NULL == pfn_ZwQueryInformationFile) {
            break;
        }

        // 9 == FileNameInformation
        if (0 != pfn_ZwQueryInformationFile(hFile, &isb, &fni, sizeof(fni), 9)) {
            break;
        }

        if (FALSE == GetVolumeNameByHandle(hFile, szValue, _countof(szValue))) {
            break;
        }

        PathAppendW(szValue, fni.FileName);

        lstrcpynW(lpszPath, szValue, cchMax);

        bResult = TRUE;
    } while (FALSE);
    return bResult;
}

EXTERN_C BOOL GetFilePathFromHandleA(HANDLE hFile, LPSTR  lpszPath, UINT cchMax)
{
    BOOL bResult = FALSE;
    WCHAR szTmep[MAX_PATH] = { 0 };

    do
    {
        if (INVALID_HANDLE_VALUE==hFile || NULL==lpszPath || 0==cchMax) {
            break;
        }

        if (FALSE == GetFilePathFromHandleW(hFile, szTmep, _countof(szTmep))) {
            break;
        }

        if (0 == WideCharToMultiByte(CP_ACP, 0,
            szTmep, lstrlenW(szTmep),
            lpszPath, cchMax, NULL, NULL))
        {
            break;
        }

        bResult = TRUE;
    } while (FALSE);
    return bResult;
}
