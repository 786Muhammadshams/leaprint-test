#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

#include "cairo/cairo.h"
#include "cairo/cairo-ft.h"
#include "cairo/cairo-pdf.h"

#include <ft2build.h>
#include FT_FREETYPE_H

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

#include "DrivePrintHandler.h"

FT_Library library;

class OKHandler : public Poco::Net::HTTPRequestHandler
{
public:
  OKHandler()
  {
  }

  void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response)
  {
    std::ostream& respout = response.send();
    respout << "LEA Print is up\n" << std::flush;
  }
};

class DrivePrintHandlerFactory : public HTTPRequestHandlerFactory
{
public:
  virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &req)
  {
    if (req.getMethod() == "GET")
      return new OKHandler();

    std::cout << "Making DrivePrintHandler" << std::endl;
    return new DrivePrintHandler;
  }
};

class LEAPrintServer : public ServerApplication
{
protected:
  int main(const vector<string> &)
  {
    HTTPServer s(new DrivePrintHandlerFactory, ServerSocket(9090), new HTTPServerParams);

    s.start();
    cout << endl << "Server started" << endl;

    waitForTerminationRequest();  // wait for CTRL-C or kill

    cout << endl << "Shutting down..." << endl;
    s.stop();

    return Application::EXIT_OK;
  }
};

int main(int argc, char** argv)
{
  std::cout << "LEAPrint server running from " << fs::current_path() << std::endl;

  auto error = FT_Init_FreeType(&library);
  if (error)
  {
    std::cout << "Failed to init FT library" << std::endl;
  }

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
  fonts.push_back("RADN-Regular.ttf");
  //fonts.push_back("RADN 15 v02.ttf");

  std::string fontPath = "./fonts/";
  if (!fs::exists(fontPath))
  {
    fontPath = "/usr/share/LEAPrintFonts/";
  }
  std::cout << "Reading fonts from " << fontPath << std::endl;


  for (auto it = fonts.begin(); it != fonts.end(); it++)
  {
    std::string file = *it;
    std::string name = file.substr(0, file.find('.'));
    std::string path = fontPath + *it;
    //std::cout << "Loading font [" << name << "]" << std::endl;
    std::cout << "FontLoader loading " << name << " " << path << std::endl;

    FT_Face face;
    error = FT_New_Face(library, path.c_str(), 0, &face);
    if (error == FT_Err_Unknown_File_Format)
    {
      std::cout << "Font format unsupported " << std::endl;
    }
    else if (error)
    {
      std::cout << "Failed to read font" << std::endl;
    }

    // Create cairo font.
    cairo_font_face_t* ff = cairo_ft_font_face_create_for_ft_face(face, 0);

    // Hand it off to DriveRenderer.
    DriveRendererClientFont(name, ff);
  }

  LEAPrintServer app;
  return app.run(argc, argv);
}
