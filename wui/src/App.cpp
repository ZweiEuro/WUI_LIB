
#include "App.hpp"
#include "RenderProcessHandler.hpp"

namespace wui
{

    App::App()
    {
        this->render_process_handler_ = new RenderProcessHandler();
    }

    // CefApp methods:
    CefRefPtr<CefRenderProcessHandler> App::GetRenderProcessHandler()
    {
        return this->render_process_handler_;
    }

}