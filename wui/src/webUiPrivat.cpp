#include "webUiPrivat.hpp"

namespace wui
{
    CefRefPtr<wui::App> app;

    bool appExists()
    {
        return app.get() != nullptr;
    }

}