#pragma once
#include "include/cef_app.h"
#include "BrowserProcessHandler.hpp"
#include "RenderProcessHandler.hpp"

namespace wui
{

    // Implementation of CefApp for the renderer process.
    class App : public CefApp
    {

    private:
        CefRefPtr<BrowserProcessHandler> browser_process_handler_;
        CefRefPtr<RenderProcessHandler> render_process_handler_;

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        IMPLEMENT_REFCOUNTING(App);

    public:
        App();
        virtual ~App(); // Necessary for complete VTABLE

        void OnBeforeCommandLineProcessing(
            const CefString &process_type,
            CefRefPtr<CefCommandLine> command_line) override;

        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;
        CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

        // WUI Functions

        CefRefPtr<BrowserProcessHandler> GetWUIBrowserProcessHandler();
        CefRefPtr<RenderProcessHandler> GetWUIRenderProcessHandler();
    };
}