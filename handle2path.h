#ifndef HANDLE2PATH_H
#define HANDLE2PATH_H
#pragma once

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi")

#ifndef _countof
#define _countof(array) (sizeof(array)/sizeof((array)[0]))
#endif

EXTERN_C BOOL GetFilePathFromHandleW(HANDLE hFile, LPWSTR lpszPath, UINT cchMax);
EXTERN_C BOOL GetFilePathFromHandleA(HANDLE hFile, LPSTR  lpszPath, UINT cchMax);

#ifdef UNICODE
#define GetFilePathFromHandle GetFilePathFromHandleW
#else
#define GetFilePathFromHandle GetFilePathFromHandleA
#endif

#endif // HANDLE2PATH_H
