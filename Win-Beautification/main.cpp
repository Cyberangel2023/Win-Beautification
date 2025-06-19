#include "MainScene.h"
#include "topmainscene.h"
#include "WallpaperStyle.h"
#include "wallpaper.h"
#include "fileset.h"

#include <windows.h>
#include <shlobj.h>  // 包含SHGetKnownFolderPath所需的头文件
#include <wrl/client.h> // 用于COM接口的智能指针（可选，但推荐）
#include <QString>
#include <QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 设置壁纸样式
    WallpaperStyle style = WallpaperStyle::Fill;
    // 设置壁纸路径
    QString wallpaperPath = "C:\\Users\\25444\\Desktop\\imgs\\Kiana1.jpg";
    // 使用QScopedPointer管理Wallpaper和FileSet的生命周期
    QScopedPointer<Wallpaper> wallpaper(new Wallpaper(style, wallpaperPath));
    FileSet::instance()->setHide(false);

    MainScene w;
    w.show();

    // 创建一个新窗口
    TopMainScene* topWindow = new TopMainScene(&w);
    topWindow->show();

    return a.exec();
}
