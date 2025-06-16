#include "wallpaper.h"

Wallpaper::Wallpaper(WallpaperStyle style, const QString& wallpaperPath) :
    style(style), wallpaperPath(wallpaperPath)
{
    SetWallpaperStyle();
    SetWallpaper();
}

Wallpaper::~Wallpaper()
{
}

bool Wallpaper::SetWallpaperStyle()
{
    HKEY hKey;
    const QString subKey = QString::fromWCharArray(L"Control Panel\\Desktop");

    // 打开注册表键
    if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey.toStdWString().c_str(), 0, KEY_WRITE, &hKey) != ERROR_SUCCESS) {
        return false;
    }

    QString wallpaperStyle = "0";
    QString tileWallpaper = "0";

    switch (style) {
    case WallpaperStyle::Tile:
        wallpaperStyle = "0";
        tileWallpaper = "1";
        break;
    case WallpaperStyle::Center:
        wallpaperStyle = "0";
        tileWallpaper = "0";
        break;
    case WallpaperStyle::Stretch:
        wallpaperStyle = "2";
        tileWallpaper = "0";
        break;
    case WallpaperStyle::Fit:
        wallpaperStyle = "6";
        tileWallpaper = "0";
        break;
    case WallpaperStyle::Fill:
        wallpaperStyle = "10";
        tileWallpaper = "0";
        break;
    }

    // 写入注册表
    if (RegSetValueEx(hKey, L"WallpaperStyle", 0, REG_SZ, (const BYTE*)wallpaperStyle.toStdWString().c_str(), (wallpaperStyle.toStdWString().size() + 1) * sizeof(wchar_t)) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return false;
    }

    if (RegSetValueEx(hKey, L"TileWallpaper", 0, REG_SZ, (const BYTE*)tileWallpaper.toStdWString().c_str(), (tileWallpaper.toStdWString().size() + 1) * sizeof(wchar_t)) != ERROR_SUCCESS)
    {
        RegCloseKey(hKey);
        return false;
    }

    // 关闭注册表键
    RegCloseKey(hKey);
    return true;
}

bool Wallpaper::SetWallpaper()
{
    // 使用 SystemParametersInfo 函数设置壁纸
    if (!SystemParametersInfoW(
            SPI_SETDESKWALLPAPER, // 设置桌面壁纸
            0,
            (PVOID)wallpaperPath.utf16(), // 壁纸路径
            SPIF_UPDATEINIFILE | SPIF_SENDCHANGE // 立即生效
            )) {
        return false;
    }
    return true;
}
