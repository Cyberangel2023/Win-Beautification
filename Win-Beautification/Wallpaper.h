#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <iostream>
#include <windows.h>
#include <string>

enum class WallpaperStyle {
    Tile = 0,     // 平铺
    Center = 1,   // 居中
    Stretch = 2,  // 拉伸
    Fit = 3,      // 适应
    Fill = 4      // 填充
};


class Wallpaper {
public:
	Wallpaper(WallpaperStyle style, const std::wstring wallpaperPath);
	~Wallpaper();

public:
    bool virtual SetWallpaperStyle();
    bool virtual SetWallpaper();

private:
    WallpaperStyle style;
    std::wstring wallpaperPath;
};

#endif // WALLPAPER_H
