

#include "BufferRenderingHandler.hpp"
#include "Client.hpp"

#include "include/cef_browser.h"

namespace wui
{

    extern CefRefPtr<wui::BufferRenderingHandler> rendererHandler;
    extern CefRefPtr<wui::Client> browserClient;
    extern CefRefPtr<CefBrowser> browser;

}