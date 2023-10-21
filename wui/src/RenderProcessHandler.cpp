

#include "RenderProcessHandler.hpp"
namespace wui
{

    RenderProcessHandler::RenderProcessHandler()
    {
    }

    void RenderProcessHandler::OnWebKitInitialized()
    {
        // Create the renderer-side router for query handling.
        CefMessageRouterConfig config;
        message_router_ = CefMessageRouterRendererSide::Create(config);
    }

    void RenderProcessHandler::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                                CefRefPtr<CefFrame> frame,
                                                CefRefPtr<CefV8Context> context)
    {
        message_router_->OnContextCreated(browser, frame, context);
    }

    void RenderProcessHandler::OnContextReleased(CefRefPtr<CefBrowser> browser,
                                                 CefRefPtr<CefFrame> frame,
                                                 CefRefPtr<CefV8Context> context)
    {
        message_router_->OnContextReleased(browser, frame, context);
    }

    bool RenderProcessHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                                        CefRefPtr<CefFrame> frame,
                                                        CefProcessId source_process,
                                                        CefRefPtr<CefProcessMessage> message)
    {
        return message_router_->OnProcessMessageReceived(browser, frame,
                                                         source_process, message);
    }
}