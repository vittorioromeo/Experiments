// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common/typedefs.hpp>

namespace vrm
{
    namespace sdl
    {
        enum class kkey : sz_t
        {
            unknown = 0,

            a = 4,
            b = 5,
            c = 6,
            d = 7,
            e = 8,
            f = 9,
            g = 10,
            h = 11,
            i = 12,
            j = 13,
            k = 14,
            l = 15,
            m = 16,
            n = 17,
            o = 18,
            p = 19,
            q = 20,
            r = 21,
            s = 22,
            t = 23,
            u = 24,
            v = 25,
            w = 26,
            x = 27,
            y = 28,
            z = 29,

            n1 = 30,
            n2 = 31,
            n3 = 32,
            n4 = 33,
            n5 = 34,
            n6 = 35,
            n7 = 36,
            n8 = 37,
            n9 = 38,
            n0 = 39,

            enter = 40,
            escape = 41,
            backspace = 42,
            tab = 43,
            space = 44,

            minus = 45,
            equals = 46,
            leftbracket = 47,
            rightbracket = 48,
            backslash = 49,
            nonushash = 50,
            semicolon = 51,
            apostrophe = 52,
            grave = 53,
            comma = 54,
            period = 55,
            slash = 56,
            capslock = 57,

            f1 = 58,
            f2 = 59,
            f3 = 60,
            f4 = 61,
            f5 = 62,
            f6 = 63,
            f7 = 64,
            f8 = 65,
            f9 = 66,
            f10 = 67,
            f11 = 68,
            f12 = 69,

            printscreen = 70,
            scrolllock = 71,
            pause = 72,
            insert = 73,

            home = 74,
            pageup = 75,
            del = 76,
            end = 77,
            pagedown = 78,
            right = 79,
            left = 80,
            down = 81,
            up = 82,

            numlockclear = 83,

            kp_divide = 84,
            kp_multiply = 85,
            kp_minus = 86,
            kp_plus = 87,
            kp_enter = 88,
            kp_1 = 89,
            kp_2 = 90,
            kp_3 = 91,
            kp_4 = 92,
            kp_5 = 93,
            kp_6 = 94,
            kp_7 = 95,
            kp_8 = 96,
            kp_9 = 97,
            kp_0 = 98,
            kp_period = 99,
            kp_equals = 103,

            lctrl = 224,
            lshift = 225,
            lalt = 226,
            lgui = 227,
            rctrl = 228,
            rshift = 229,
            ralt = 230,
            rgui = 231
        };

        enum class mbtn : sz_t
        {
            left = 1,
            middle = 2,
            right = 3,
            wheelup = 4,
            wheeldown = 5,
            x1 = 6,
            x2 = 7
        };

        namespace impl
        {
            constexpr sz_t kkey_count{512};
            constexpr sz_t mbtn_count{8};
        }

        using key_event_handler = std::function<void(kkey)>;
        using btn_event_handler = std::function<void(mbtn)>;
    }
}