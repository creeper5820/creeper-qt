#pragma once

#include <qfont.h>

namespace creeper {
namespace material {

    constexpr auto kFontSizeExtraSmall = int { 15 };
    constexpr auto kFontSizeSmall      = int { 18 };
    constexpr auto kFontSizeMedium     = int { 18 };
    constexpr auto kFontSizeLarge      = int { 24 };
    constexpr auto kFontSizeExtraLarge = int { 32 };

    constexpr auto kSharpFontName          = "Material Symbols Sharp";
    inline const auto kSharpExtraSmallFont = QFont { kSharpFontName, kFontSizeExtraSmall };
    inline const auto kSharpSmallFont      = QFont { kSharpFontName, kFontSizeSmall };
    inline const auto kSharpMediumFont     = QFont { kSharpFontName, kFontSizeMedium };
    inline const auto kSharpLargeFont      = QFont { kSharpFontName, kFontSizeLarge };
    inline const auto kSharpExtraLargeFont = QFont { kSharpFontName, kFontSizeExtraLarge };

    constexpr auto kRoundedFontName          = "Material Symbols Rounded";
    inline const auto kRoundedExtraSmallFont = QFont { kRoundedFontName, kFontSizeExtraSmall };
    inline const auto kRoundedSmallFont      = QFont { kRoundedFontName, kFontSizeSmall };
    inline const auto kRoundedMediumFont     = QFont { kRoundedFontName, kFontSizeMedium };
    inline const auto kRoundedLargeFont      = QFont { kRoundedFontName, kFontSizeLarge };
    inline const auto kRoundedExtraLargeFont = QFont { kRoundedFontName, kFontSizeExtraLarge };

    constexpr auto kOutlinedFontName          = "Material Symbols Outlined";
    inline const auto kOutlinedExtraSmallFont = QFont { kOutlinedFontName, kFontSizeExtraSmall };
    inline const auto kOutlinedSmallFont      = QFont { kOutlinedFontName, kFontSizeSmall };
    inline const auto kOutlinedMediumFont     = QFont { kOutlinedFontName, kFontSizeMedium };
    inline const auto kOutlinedLargeFont      = QFont { kOutlinedFontName, kFontSizeLarge };
    inline const auto kOutlinedExtraLargeFont = QFont { kOutlinedFontName, kFontSizeExtraLarge };

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
