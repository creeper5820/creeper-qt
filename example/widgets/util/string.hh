#pragma once

namespace str {

constexpr auto kGithubBaseUrl = "https://www.github.com";
constexpr auto kFontUrlPath {
    "/google/material-design-icons/raw/master/font/MaterialIconsRound-Regular.otf",
};
constexpr auto kFontFileName = "MaterialIconsRound-Regular.otf";

constexpr auto kFontDescription = R"(
    <div style="line-height: 1.5; font-size: 14px;">
        <p>示例中的所有 Icon 均为 Google Material Icons 字体提供，<br>
        需要下载并安装相应的字体文件才能正常显示。</p>

        <p>这里贴心做了下载按钮，点击即送。<br>
        如果无法下载，请设置环境变量 <code>http_proxy</code> 或 <code>https_proxy</code>。</p>

        <p>字体链接：<a href="https://github.com//google/material-design-icons/raw/master/font/MaterialIconsRound-Regular.otf" target="_blank">MaterialIconsRound-Regular.otf</a></p>
    </div>
)";

}
