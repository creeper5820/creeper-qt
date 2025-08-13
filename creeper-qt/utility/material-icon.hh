#pragma once

#include <qfont.h>

namespace creeper {
namespace material {

    namespace size {
        constexpr auto _0 = int { 15 };
        constexpr auto _1 = int { 18 };
        constexpr auto _2 = int { 18 };
        constexpr auto _3 = int { 24 };
        constexpr auto _4 = int { 32 };
    }
    namespace sharp {
        constexpr auto font      = "Material Icons Sharp";
        inline const auto font_0 = QFont { font, size::_0 };
        inline const auto font_1 = QFont { font, size::_1 };
        inline const auto font_2 = QFont { font, size::_2 };
        inline const auto font_3 = QFont { font, size::_3 };
        inline const auto font_4 = QFont { font, size::_4 };
    }
    namespace round {
        constexpr auto font      = "Material Icons Round";
        inline const auto font_0 = QFont { font, size::_0 };
        inline const auto font_1 = QFont { font, size::_1 };
        inline const auto font_2 = QFont { font, size::_2 };
        inline const auto font_3 = QFont { font, size::_3 };
        inline const auto font_4 = QFont { font, size::_4 };
    }
    namespace outlined {
        constexpr auto font      = "Material Icons Outlined";
        inline const auto font_0 = QFont { font, size::_0 };
        inline const auto font_1 = QFont { font, size::_1 };
        inline const auto font_2 = QFont { font, size::_2 };
        inline const auto font_3 = QFont { font, size::_3 };
        inline const auto font_4 = QFont { font, size::_4 };
    }

    constexpr auto kFontSizeExtraSmall = size::_0;
    constexpr auto kFontSizeSmall      = size::_1;
    constexpr auto kFontSizeMedium     = size::_2;
    constexpr auto kFontSizeLarge      = size::_3;
    constexpr auto kFontSizeExtraLarge = size::_4;

    constexpr auto kSharpFontName          = sharp::font;
    inline const auto kSharpExtraSmallFont = sharp::font_0;
    inline const auto kSharpSmallFont      = sharp::font_1;
    inline const auto kSharpMediumFont     = sharp::font_2;
    inline const auto kSharpLargeFont      = sharp::font_3;
    inline const auto kSharpExtraLargeFont = sharp::font_4;

    constexpr auto kRoundFontName          = round::font;
    inline const auto kRoundExtraSmallFont = round::font_0;
    inline const auto kRoundSmallFont      = round::font_1;
    inline const auto kRoundMediumFont     = round::font_2;
    inline const auto kRoundLargeFont      = round::font_3;
    inline const auto kRoundExtraLargeFont = round::font_4;

    constexpr auto kOutlinedFontName          = outlined::font;
    inline const auto kOutlinedExtraSmallFont = outlined::font_0;
    inline const auto kOutlinedSmallFont      = outlined::font_1;
    inline const auto kOutlinedMediumFont     = outlined::font_2;
    inline const auto kOutlinedLargeFont      = outlined::font_3;
    inline const auto kOutlinedExtraLargeFont = outlined::font_4;

    namespace icon {

        // Function
        constexpr auto kSettings      = "settings";
        constexpr auto kSearch        = "search";
        constexpr auto kHome          = "home";
        constexpr auto kMenu          = "menu";
        constexpr auto kInfo          = "info";
        constexpr auto kHelp          = "help";
        constexpr auto kRefresh       = "refresh";
        constexpr auto kMoreVert      = "more_vert";
        constexpr auto kMoreHoriz     = "more_horiz";
        constexpr auto kNotifications = "notifications";
        constexpr auto kDashboard     = "dashboard";

        // Shape
        constexpr auto kFavorite    = "favorite";
        constexpr auto kStar        = "star";
        constexpr auto kHeartBroken = "heart_broken";
        constexpr auto kCheck       = "check";
        constexpr auto kCircle      = "circle";
        constexpr auto kSquare      = "square";
        constexpr auto kArrowUp     = "arrow_upward";
        constexpr auto kArrowDown   = "arrow_downward";
        constexpr auto kArrowLeft   = "arrow_back";
        constexpr auto kArrowRight  = "arrow_forward";

        // Action
        constexpr auto kClose       = "close";
        constexpr auto kAdd         = "add";
        constexpr auto kEdit        = "edit";
        constexpr auto kDelete      = "delete";
        constexpr auto kSave        = "save";
        constexpr auto kShare       = "share";
        constexpr auto kSend        = "send";
        constexpr auto kUpload      = "upload";
        constexpr auto kDownload    = "download";
        constexpr auto kCheckCircle = "check_circle";
        constexpr auto kCancel      = "cancel";
        constexpr auto kOpenInNew   = "open_in_new";
        constexpr auto kLogout      = "logout";
        constexpr auto kRoutine     = "routine";
        constexpr auto kDarkMode    = "dark_mode";

        // File
        constexpr auto kFolder        = "folder";
        constexpr auto kFolderOpen    = "folder_open";
        constexpr auto kInsertDrive   = "insert_drive_file";
        constexpr auto kAttachFile    = "attach_file";
        constexpr auto kCloud         = "cloud";
        constexpr auto kCloudDownload = "cloud_download";
        constexpr auto kCloudUpload   = "cloud_upload";
        constexpr auto kFileCopy      = "file_copy";
        constexpr auto kDescription   = "description";
    }
}
}
