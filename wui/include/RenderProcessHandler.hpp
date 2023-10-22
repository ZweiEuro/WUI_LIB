#pragma once
#include "include/wrapper/cef_helpers.h"
#include "include/cef_render_process_handler.h"
#include "include/wrapper/cef_message_router.h"
#include "jsBinding/V8Handler.hpp"
#include "webUiJsBindings.hpp"

namespace wui
{

    // Implementation of RenderProcess, this handles the local context.
    // Necessary for: message routing, js binding
    class RenderProcessHandler : public CefRenderProcessHandler
    {
    private:
        // Handles the renderer side of query routing.
        CefRefPtr<CefMessageRouterRendererSide> message_router_;
        CefRefPtr<V8Handler> v8Handler_;

        IMPLEMENT_REFCOUNTING(RenderProcessHandler);
        RenderProcessHandler(const RenderProcessHandler &) = delete;
        RenderProcessHandler &operator=(const RenderProcessHandler &) = delete;

    public:
        RenderProcessHandler() {}

        // CefRenderProcessHandler methods:
        void OnWebKitInitialized() override;

        void OnContextCreated(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              CefRefPtr<CefV8Context> context) override;
        void OnContextReleased(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               CefRefPtr<CefV8Context> context) override;
        bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefProcessId source_process,
                                      CefRefPtr<CefProcessMessage> message) override;

        // WUI Functions

        bool addEventListener(const char *eventName, eventListenerFunction_t function);
        bool removeEventListener(const char *eventName);
    };
}