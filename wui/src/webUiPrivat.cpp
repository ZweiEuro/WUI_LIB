#include "webUiPrivat.hpp"

namespace wui
{

    CefRefPtr<RenderProcessHandler> renderProcessApp = nullptr;
    CefRefPtr<BrowserProcessHandler> browserProcessApp = nullptr;

    bool browserProcessExists()
    {
        return browserProcessApp.get() != nullptr;
    }

}