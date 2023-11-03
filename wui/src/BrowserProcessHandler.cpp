
#include "BrowserProcessHandler.hpp"
#include "Client.hpp"
#include "include/cef_browser.h"
#include <filesystem>

namespace wui
{

    void BrowserProcessHandler::OnBeforeCommandLineProcessing(
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

    void BrowserProcessHandler::setClientViewParams(void **pixelBuffer, const size_t initialHeight, const size_t initialWidth, std::string path)
    {
        this->client_para_pixelBuffer_ = pixelBuffer;
        this->client_para_initialHeight = initialHeight;
        this->client_para_initialWidth = initialWidth;
        this->client_para_path = path;
    }

    // Startup our client
    void BrowserProcessHandler::OnContextInitialized()
    {

        // create the client

        this->client_ = new Client(
            message_handler_,
            client_para_pixelBuffer_,
            client_para_initialHeight,
            client_para_initialWidth);

        // initial setup

        CefWindowInfo window_info;
        window_info.SetAsWindowless(0); // false means no transparency (site background colour)

        DLOG(INFO) << "Browser Init";

        CefBrowserSettings browserSettings;
        browserSettings.windowless_frame_rate = 60; // 30 is default

        auto path = client_para_path;

        // If path does not start with '/', it is relative to the current working directory.
        if (path.front() != '/')
        {
            path = std::filesystem::current_path().string() + "/" + path;
        }

        // if path does not end with '/', add it
        if (path.back() != '/')
        {
            path += "/";
        }

        path += "index.html";

        DLOG(INFO)
            << "Open File: " << path
            << " in Browser, with URL: "
            << "file://" + path;

        this->browser_ = CefBrowserHost::CreateBrowserSync(window_info, this->client_, "file://" + path, browserSettings, nullptr, nullptr);
    }

    CefRefPtr<Client> BrowserProcessHandler::GetClient() { return this->client_; }
    CefRefPtr<CefBrowser> BrowserProcessHandler::GetBrowser() { return this->browser_; }

    CefRefPtr<CefBrowserProcessHandler> BrowserProcessHandler::GetBrowserProcessHandler()
    {
        return this;
    }

    CefRefPtr<BrowserProcessHandler> BrowserProcessHandler::GetWUIBrowserProcessHandler()
    {
        return this;
    }

    bool BrowserProcessHandler::addEventListener(const char *eventName, eventListenerFunction_t function)
    {
        return this->message_handler_->addEventListener(eventName, function);
    }

    bool BrowserProcessHandler::removeEventListener(const char *eventName)
    {
        return this->message_handler_->removeEventListener(eventName);
    }

}