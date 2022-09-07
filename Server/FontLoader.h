#ifndef include_HyperServer_LEAPrint_FontLoader_h
#define include_HyperServer_LEAPrint_FontLoader_h

#include <vector>

#include "cairo/cairo.h"

#include "lea/util/Singleton.h"
#include "lea/util/Logger.h"

#include "dr/API2.h"

class FontLoader : public Singleton<FontLoader>
{
    friend class Singleton<FontLoader>;
private:
    FontLoader() {}
    ~FontLoader() {}

public:
    void LoadFonts(void)
    {
        std::vector<std::string> fonts;
        fonts.push_back("ArialBlackLEA.ttf");
        fonts.push_back("ArialBoldItalLEA.ttf");
        fonts.push_back("ArialBoldLEA.ttf");
        fonts.push_back("ArialItalLEA.ttf");
        fonts.push_back("ArialKursivLEA.ttf");
        fonts.push_back("ArialLEA.ttf");
        fonts.push_back("SegoeWP-Black.ttf");
        fonts.push_back("SegoeWP-Bold.ttf");
        fonts.push_back("SegoeWP-Semibold.ttf");
        fonts.push_back("SegoeWP.ttf");
        fonts.push_back("RADN 15 v02.ttf");

        for (auto it = fonts.begin(); it != fonts.end(); it++)
        {
            std::string file = *it;
            std::string name = file.substr(0, file.find('.'));
            std::string path = "./fonts/" + *it;
            //std::cout << "Loading font [" << name << "]" << std::endl;
            Logger::Info(boost::format("FontLoader: loading %1% from %2%") % name % path);

            if (name.size() >= 32)
                Logger::Warning("FontLoader: font-face name is too long (32 chars max)");

            // To make sure we are causing errors.
            SetLastError(0);

            // http://msdn.microsoft.com/en-us/library/windows/desktop/dd183327(v=vs.85).aspx
            // Add font from file.
            // FR_PRIVATE => Specifies that only the process that called the AddFontResourceEx function can use this font. When the font name matches a public font, the private font will be chosen. When the process terminates, the system will remove all fonts installed by the process with the AddFontResourceEx function.
            AddFontResourceEx(path.c_str(), FR_PRIVATE, 0);

            DWORD addError = GetLastError();
            if (addError)
                Logger::Error(boost::format("Error while adding font resource: %1%") % addError);

            // http://msdn.microsoft.com/en-us/library/windows/desktop/dd145037(v=vs.85).aspx
            // A font description.
            LOGFONT logfont;
            logfont.lfHeight = 0; // use default height
            logfont.lfWidth = 0; // find closest match based on height and device aspect ratio
            logfont.lfEscapement = 0; // string angle
            logfont.lfOrientation = 0; // character angle
            logfont.lfWeight = FW_NORMAL; // normal font weight=400
            logfont.lfItalic = false;
            logfont.lfUnderline = false;
            logfont.lfStrikeOut = false;
            logfont.lfCharSet = DEFAULT_CHARSET;
            logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
            logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
            logfont.lfQuality = ANTIALIASED_QUALITY;
            logfont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
            std::strncpy(logfont.lfFaceName, name.c_str(), 32);

            // http://msdn.microsoft.com/en-us/library/windows/desktop/dd183500(v=vs.85).aspx
            // LOGFONT => HFONT
            HFONT hfont = CreateFontIndirect(&logfont);

            DWORD createError = GetLastError();
            if (createError)
                Logger::Error(boost::format("Error while creating hfont: %1%") % createError);

            // Create cairo font.
            cairo_font_face_t* ff = cairo_win32_font_face_create_for_hfont(hfont);

            // Hand it off to DriveRenderer.
            DriveRendererClientFont(name, ff);
        }
    }
};

#endif
