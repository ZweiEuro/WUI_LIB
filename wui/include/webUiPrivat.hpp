
#pragma once

#include "RenderProcessHandler.hpp"
#include "BrowserProcessHandler.hpp"

namespace wui
{

    extern CefRefPtr<RenderProcessHandler> renderProcessApp;
    extern CefRefPtr<BrowserProcessHandler> browserProcessApp;

    bool browserProcessExists();

}