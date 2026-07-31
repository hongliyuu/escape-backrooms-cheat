#pragma once

#include <string>

namespace paths {
    inline std::string data_dir() {
        return "C:\\LHY1339\\escape_the_backrooms";
    }
    inline std::string data_file(const std::string& name) {
        return data_dir() + "\\" + name;
    }
    inline std::wstring data_file_w(const std::string& name) {
        std::string s = data_file(name);
        return std::wstring(s.begin(), s.end());
    }
}

//#define CHEAT_DEBUG

#define VERSION "26.2.13"
#define VERSION_W L"版本：26.2.13\n"
#define GAME_VERSION L"支持游戏版本：1.20(2841)\n"
#define THANKS \
L"感谢 雾染 的爱发电喵~\n" \
L"感谢 爱发电用户_6560b 的爱发电喵~\n" \
L"感谢 凡星玄尘 的爱发电喵~\n" \
L"感谢 爱发电用户_90b57 的爱发电喵~\n" \
L"感谢 爱发电用户_tYuQ 的爱发电喵~\n" \
L"感谢 夏尘 的爱发电喵~\n" \
L"感谢 Alans1gn 的爱发电喵~\n" \
L"感谢 何必执着那片海 的爱发电喵~\n" \
L"感谢 爱发电用户_UyQx 的爱发电喵~\n" \
L"感谢 zou 的爱发电喵~\n" \
L"感谢 YY 的爱发电喵~\n" \
L"感谢 爱发电用户_V6Cf 的爱发电喵~\n"