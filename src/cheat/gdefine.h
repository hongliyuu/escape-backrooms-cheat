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