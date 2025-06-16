#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <windows.h>
#include <QString>
#include <QSettings>

#include "WallpaperStyle.h"

class Wallpaper
{
public:
    Wallpaper(WallpaperStyle style, const QString& wallpaperPath);
    ~Wallpaper();

private:
    // 设置壁纸样式
    bool SetWallpaperStyle();
    // 设置壁纸图片
    bool SetWallpaper();

public:
    void setStyle(WallpaperStyle style) {
        this->style = style;
    }

private:
    WallpaperStyle style; // 壁纸样式
    QString wallpaperPath; // 壁纸图片路径
};

#endif // WALLPAPER_H
