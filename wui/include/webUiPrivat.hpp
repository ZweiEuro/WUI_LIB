

#include "RendererHandler.hpp"
#include "BrowserClient.hpp"

#include "include/cef_browser.h"

namespace wui
{

    extern CefRefPtr<wui::RendererHandler> rendererHandler;
    extern CefRefPtr<wui::BrowserClient> browserClient;
    extern CefRefPtr<CefBrowser> browser;

}