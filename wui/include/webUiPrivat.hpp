
#pragma once

#include "RenderHandler.hpp"
#include "Client.hpp"
#include "App.hpp"

#include "include/cef_browser.h"

namespace wui
{

    extern CefRefPtr<wui::App> app;

    bool appExists();

}