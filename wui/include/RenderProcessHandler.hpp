#pragma once
#include "include/wrapper/cef_message_router.h"
#include "messageRouter/messageRouterConfig.hpp"
namespace wui
{

    // Implementation of CefApp for the renderer process.
    class RenderProcessHandler : public CefRenderProcessHandler
    {
    public:
        RenderProcessHandler() {}

        // CefRenderProcessHandler methods:
        void OnWebKitInitialized() override
        {
            // Create the renderer-side router for query handling.
            message_router_ = CefMessageRouterRendererSide::Create(getMessageRouterConfig());
        }

        void OnContextCreated(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefV8Context> context) override
        {
            message_router_->OnContextCreated(browser, frame, context);
        }

        void OnContextReleased(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               CefRefPtr<CefV8Context> context) override
        {
            message_router_->OnContextReleased(browser, frame, context);
        }

        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefProcessId source_process,
                                      CefRefPtr<CefProcessMessage> message) override
        {
            return message_router_->OnProcessMessageReceived(browser, frame,
                                                             source_process, message);
        }

    private:
        // Handles the renderer side of query routing.
        CefRefPtr<CefMessageRouterRendererSide> message_router_;

        IMPLEMENT_REFCOUNTING(RenderProcessHandler);
        DISALLOW_COPY_AND_ASSIGN(RenderProcessHandler);
    };

}