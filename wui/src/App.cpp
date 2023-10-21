
#include "App.hpp"

namespace wui
{

    App::App()
    {
        browser_process_handler_ = new BrowserProcessHandler();
    }

    App::~App()
    {
    }

    // CefApp methods:
    CefRefPtr<CefRenderProcessHandler> App::GetRenderProcessHandler()
    {
        // not needed yet
        return nullptr;
    }

    CefRefPtr<CefBrowserProcessHandler> App::GetBrowserProcessHandler()
    {
        return this->browser_process_handler_;
    }

    CefRefPtr<BrowserProcessHandler> App::GetWUIBrowserProcessHandler()
    {
        return this->browser_process_handler_;
    }

    void App::OnBeforeCommandLineProcessing(
        const CefString &process_type,
        CefRefPtr<CefCommandLine> command_line)
    {
        // Command-line flags can be modified in this callback.
        // |process_type| is empty for the browser process.
        if (process_type.empty())
        {
#if defined(OS_MACOSX)
            // Disable the macOS keychain prompt. Cookies will not be encrypted.
            command_line->AppendSwitch("use-mock-keychain");
#endif
        }
    }

}