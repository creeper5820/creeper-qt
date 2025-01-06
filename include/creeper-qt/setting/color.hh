#pragma once
#include <cstdint>

namespace creeper {

struct color {
    static constexpr uint32_t transparent = 0xffffffff;
    static constexpr uint32_t black = 0x000000;
    static constexpr uint32_t white = 0xffffff;

    static constexpr uint32_t red050 = 0xFFEBEE;
    static constexpr uint32_t red100 = 0xFFCDD2;
    static constexpr uint32_t red200 = 0xEF9A9A;
    static constexpr uint32_t red300 = 0xE57373;
    static constexpr uint32_t red400 = 0xEF5350;
    static constexpr uint32_t red500 = 0xF44336;
    static constexpr uint32_t red600 = 0xE53935;
    static constexpr uint32_t red700 = 0xD32F2F;
    static constexpr uint32_t red800 = 0xC62828;
    static constexpr uint32_t red900 = 0xB71C1C;

    static constexpr uint32_t indigo050 = 0xE8EAF6;
    static constexpr uint32_t indigo100 = 0xC5CAE9;
    static constexpr uint32_t indigo200 = 0x9FA8DA;
    static constexpr uint32_t indigo300 = 0x7986CB;
    static constexpr uint32_t indigo400 = 0x5C6BC0;
    static constexpr uint32_t indigo500 = 0x3F51B5;
    static constexpr uint32_t indigo600 = 0x3949AB;
    static constexpr uint32_t indigo700 = 0x303F9F;
    static constexpr uint32_t indigo800 = 0x283593;
    static constexpr uint32_t indigo900 = 0x1A237E;

    static constexpr uint32_t blue050 = 0xE3F2FD;
    static constexpr uint32_t blue100 = 0xBBDEFB;
    static constexpr uint32_t blue200 = 0x90CAF9;
    static constexpr uint32_t blue300 = 0x64B5F6;
    static constexpr uint32_t blue400 = 0x42A5F5;
    static constexpr uint32_t blue500 = 0x2196F3;
    static constexpr uint32_t blue600 = 0x1E88E5;
    static constexpr uint32_t blue700 = 0x1976D2;
    static constexpr uint32_t blue800 = 0x1565C0;
    static constexpr uint32_t blue900 = 0x0D47A1;

    static constexpr uint32_t teal050 = 0xE0F2F1;
    static constexpr uint32_t teal100 = 0xB2DFDB;
    static constexpr uint32_t teal200 = 0x80CBC4;
    static constexpr uint32_t teal300 = 0x4DB6AC;
    static constexpr uint32_t teal400 = 0x26A69A;
    static constexpr uint32_t teal500 = 0x009688;
    static constexpr uint32_t teal600 = 0x00897B;
    static constexpr uint32_t teal700 = 0x00796B;
    static constexpr uint32_t teal800 = 0x00695C;
    static constexpr uint32_t teal900 = 0x004D40;

    static constexpr uint32_t green050 = 0xE8F5E9;
    static constexpr uint32_t green100 = 0xC8E6C9;
    static constexpr uint32_t green200 = 0xA5D6A7;
    static constexpr uint32_t green300 = 0x81C784;
    static constexpr uint32_t green400 = 0x66BB6A;
    static constexpr uint32_t green500 = 0x4CAF50;
    static constexpr uint32_t green600 = 0x43A047;
    static constexpr uint32_t green700 = 0x388E3C;
    static constexpr uint32_t green800 = 0x2E7D32;
    static constexpr uint32_t green900 = 0x1B5E20;

    static constexpr uint32_t orange050 = 0xFFF3E0;
    static constexpr uint32_t orange100 = 0xFFE0B2;
    static constexpr uint32_t orange200 = 0xFFCC80;
    static constexpr uint32_t orange300 = 0xFFB74D;
    static constexpr uint32_t orange400 = 0xFFA726;
    static constexpr uint32_t orange500 = 0xFF9800;
    static constexpr uint32_t orange600 = 0xFB8C00;
    static constexpr uint32_t orange700 = 0xF57C00;
    static constexpr uint32_t orange800 = 0xEF6C00;
    static constexpr uint32_t orange900 = 0xE65100;

    static constexpr uint32_t brown050 = 0xEFEBE9;
    static constexpr uint32_t brown100 = 0xD7CCC8;
    static constexpr uint32_t brown200 = 0xBCAAA4;
    static constexpr uint32_t brown300 = 0xA1887F;
    static constexpr uint32_t brown400 = 0x8D6E63;
    static constexpr uint32_t brown500 = 0x795548;
    static constexpr uint32_t brown600 = 0x6D4C41;
    static constexpr uint32_t brown700 = 0x5D4037;
    static constexpr uint32_t brown800 = 0x4E342E;
    static constexpr uint32_t brown900 = 0x3E2723;

    static constexpr uint32_t grey050 = 0xFAFAFA;
    static constexpr uint32_t grey100 = 0xF5F5F5;
    static constexpr uint32_t grey200 = 0xEEEEEE;
    static constexpr uint32_t grey300 = 0xE0E0E0;
    static constexpr uint32_t grey400 = 0xBDBDBD;
    static constexpr uint32_t grey500 = 0x9E9E9E;
    static constexpr uint32_t grey600 = 0x757575;
    static constexpr uint32_t grey700 = 0x616161;
    static constexpr uint32_t grey800 = 0x424242;
    static constexpr uint32_t grey900 = 0x212121;
};

}