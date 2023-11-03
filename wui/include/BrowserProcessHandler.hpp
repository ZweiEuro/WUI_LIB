#pragma once
#include "Client.hpp"
#include "webUiBinding.hpp"

namespace wui
{

    // This manages all the clients and their views and lifespans
    class BrowserProcessHandler : public CefApp, public CefBrowserProcessHandler
    {

    private:
        // browser itself
        CefRefPtr<CefBrowser> browser_;

        // client parameters, inside the browser
        void **client_para_pixelBuffer_ = nullptr;
        size_t client_para_initialHeight = 0;
        size_t client_para_initialWidth = 0;
        std::string client_para_path = "";

        CefRefPtr<Client> client_;

        // Query handler

        std::shared_ptr<wui::MessageHandler> message_handler_ = std::make_shared<MessageHandler>(); // used by the router inside client

        // CEF ref
        IMPLEMENT_REFCOUNTING(BrowserProcessHandler);

        BrowserProcessHandler(const BrowserProcessHandler &) = delete;
        BrowserProcessHandler &operator=(const BrowserProcessHandler &) = delete;

    public:
        BrowserProcessHandler() {}

        void OnBeforeCommandLineProcessing(
            const CefString &process_type,
            CefRefPtr<CefCommandLine> command_line) override;

        void OnContextInitialized() override;

        // WUI
        void setClientViewParams(void **pixelBuffer, const size_t initialHeight, const size_t initialWidth, std::string path);

        CefRefPtr<Client> GetClient();
        CefRefPtr<CefBrowser> GetBrowser();

        CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
        CefRefPtr<BrowserProcessHandler> GetWUIBrowserProcessHandler();

        // WUI Functions

        bool addEventListener(const char *eventName, eventListenerFunction_t function);
        bool removeEventListener(const char *eventName);
    };
}
