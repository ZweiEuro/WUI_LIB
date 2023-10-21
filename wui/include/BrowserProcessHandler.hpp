#pragma once
#include "Client.hpp"

namespace wui
{

    // This manages all the clients and their views and lifespans
    class BrowserProcessHandler : public CefBrowserProcessHandler
    {

    private:
        // client parameters
        void **client_para_pixelBuffer_ = nullptr;
        size_t client_para_initialHeight = 0;
        size_t client_para_initialWidth = 0;
        std::string client_para_path = "";

        CefRefPtr<Client> client_;

        // browser itself
        CefRefPtr<CefBrowser> browser_;

        // CEF ref
        IMPLEMENT_REFCOUNTING(BrowserProcessHandler);

        BrowserProcessHandler(const BrowserProcessHandler &) = delete;
        BrowserProcessHandler &operator=(const BrowserProcessHandler &) = delete;

    public:
        BrowserProcessHandler() {}

        void OnContextInitialized() override;

        // WUI
        void setClientViewParams(void **pixelBuffer, const size_t initialHeight, const size_t initialWidth, std::string path);

        CefRefPtr<Client> GetClient();
        CefRefPtr<CefBrowser> GetBrowser();
    };
}
