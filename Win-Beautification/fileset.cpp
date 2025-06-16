#include "fileset.h"

FileSet::FileSet()
{
    HideDesktopIcons(true);
}

FileSet::~FileSet()
{
}

bool FileSet::HideDesktopIcons(bool hide) {
    // 获取桌面窗口句柄
    HWND hWndDesktop = FindWindow(L"Progman", L"Program Manager");
    if (hWndDesktop == NULL) {
        return -1;
    }

    // 获取桌面图标的句柄
    HWND hWndFolderView = FindWindowEx(hWndDesktop, NULL, L"SHELLDLL_DefView", NULL);
    if (hWndFolderView == NULL) {
        return -1;
    }

    // 获取桌面图标的子窗口句柄
    HWND hWndIconView = FindWindowEx(hWndFolderView, NULL, L"SysListView32", L"FolderView");
    if (hWndIconView == NULL) {
        return -1;
    }

    if (hide) {
        // 隐藏桌面图标
        ShowWindow(hWndIconView, SW_SHOW);
        return true;
    }
    return false;
}
