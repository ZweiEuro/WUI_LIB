#pragma once
#include "include/wrapper/cef_message_router.h"

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
            DLOG(INFO) << "Create renderer side router";
            // Create the renderer-side router for query handling.
            CefMessageRouterConfig config;
            message_router_ = CefMessageRouterRendererSide::Create(config);
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