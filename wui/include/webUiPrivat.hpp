

#include "RenderHandler.hpp"
#include "Client.hpp"

#include "include/cef_browser.h"

namespace wui
{

    extern CefRefPtr<wui::RenderHandler> rendererHandler;
    extern CefRefPtr<wui::Client> browserClient;
    extern CefRefPtr<CefBrowser> browser;

}