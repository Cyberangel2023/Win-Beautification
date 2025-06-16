#include "Wallpaper.h"



Wallpaper::Wallpaper(WallpaperStyle style, const std::wstring wallpaperPath) :
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
    const wchar_t* subKey = L"Control Panel\\Desktop";

    // ´ò¿ª×¢²á±í¼ü
    if (RegOpenKeyEx(HKEY_CURRENT_USER, subKey, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS) {
        std::cerr << "´ò¿ª×¢²á±í¼üÊ§°Ü" << std::endl;
        return false;
    }

    // ÉèÖÃ±ÚÖ½ÑùÊ½
    const wchar_t* wallpaperStyle = L"0";
    const wchar_t* tileWallpaper = L"0";

    switch (style) {
    case WallpaperStyle::Tile:
        wallpaperStyle = L"0";
        tileWallpaper = L"1";
        break;
    case WallpaperStyle::Center:
        wallpaperStyle = L"0";
        tileWallpaper = L"0";
        break;
    case WallpaperStyle::Stretch:
        wallpaperStyle = L"2";
        tileWallpaper = L"0";
        break;
    case WallpaperStyle::Fit:
        wallpaperStyle = L"6";
        tileWallpaper = L"0";
        break;
    case WallpaperStyle::Fill:
        wallpaperStyle = L"10";
        tileWallpaper = L"0";
        break;
    }

    // Ð´Èë×¢²á±í
    if (RegSetValueEx(hKey, L"WallpaperStyle", 0, REG_SZ, (const BYTE*)wallpaperStyle, (wcslen(wallpaperStyle) + 1) * sizeof(wchar_t)) != ERROR_SUCCESS) {
        std::cerr << "ÉèÖÃ WallpaperStyle Ê§°Ü" << std::endl;
        RegCloseKey(hKey);
        return false;
    }

    if (RegSetValueEx(hKey, L"TileWallpaper", 0, REG_SZ, (const BYTE*)tileWallpaper, (wcslen(tileWallpaper) + 1) * sizeof(wchar_t)) != ERROR_SUCCESS) {
        std::cerr << "ÉèÖÃ TileWallpaper Ê§°Ü" << std::endl;
        RegCloseKey(hKey);
        return false;
    }

    // ¹Ø±Õ×¢²á±í¼ü
    RegCloseKey(hKey);

    return true;
}

bool Wallpaper::SetWallpaper()
{
    // Ê¹ÓÃ SystemParametersInfo º¯ÊýÉèÖÃ±ÚÖ½
    if (!SystemParametersInfoW(
        SPI_SETDESKWALLPAPER, // ÉèÖÃ×ÀÃæ±ÚÖ½
        0,
        (PVOID)wallpaperPath.c_str(), // ±ÚÖ½Â·¾¶
        SPIF_UPDATEINIFILE | SPIF_SENDCHANGE // Á¢¼´ÉúÐ§
    )) {
        std::cerr << "±ÚÖ½ÉèÖÃÊ§°Ü" << std::endl;
        return false;
    }
    return true;
}