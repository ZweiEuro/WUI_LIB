#pragma once

#include "RenderHandler.hpp"
#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"
#include "messageRouter/SingleEventMessageHandler.hpp"
#include "messageRouter/MessageHandlers.hpp"

namespace wui
{

    // Client actually handling the html internals
    // the client also holds the renderHandler
    class Client : public CefClient, public CefLifeSpanHandler, public CefRequestHandler
    {
    private:
        CefRefPtr<RenderHandler> render_handler_;

        CefRefPtr<CefMessageRouterBrowserSide> message_router_;
        std::shared_ptr<MessageHandlers> message_handlers_;

        // Track the number of browsers using this Client.
        int browser_ct_;

        IMPLEMENT_REFCOUNTING(Client);

    public:
        Client(std::shared_ptr<MessageHandlers> message_handler, void **destinationPixelBuffer, const size_t height, const size_t width);

        CefRefPtr<CefRenderHandler> GetRenderHandler() override;
        CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }

        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefProcessId source_process,
                                      CefRefPtr<CefProcessMessage> message) override;

        // CefLifeSpanHandler methods:
        void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
        void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

        // CefRequestHandler methods:
        bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                            CefRefPtr<CefFrame> frame,
                            CefRefPtr<CefRequest> request,
                            bool user_gesture,
                            bool is_redirect) override;

        void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                       TerminationStatus status) override;

        // WUI
        CefRefPtr<RenderHandler> GetWUIRenderHandler();
    };
}