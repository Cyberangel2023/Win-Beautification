#ifndef FILESET_H
#define FILESET_H

#include <windows.h>
#include <WinUser.h>
#include <shellapi.h>

class FileSet
{
public:
    FileSet();
    ~FileSet();

private:
    bool HideDesktopIcons(bool hide);

private:
    bool isHide = false;
};

#endif // FILESET_H
