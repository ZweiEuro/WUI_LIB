#include "RenderProcessHandler.hpp"
#include "messageRouter/messageRouterConfig.hpp"

namespace wui
{

    // CefRenderProcessHandler methods:
    void RenderProcessHandler::OnWebKitInitialized()
    {
        // Create the renderer-side router for query handling.
        message_router_ = CefMessageRouterRendererSide::Create(getMessageRouterConfig());
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

    CefRefPtr<CefRenderProcessHandler> RenderProcessHandler::GetRenderProcessHandler()
    {
        return this;
    }
    CefRefPtr<RenderProcessHandler> RenderProcessHandler::GetWUIRenderProcessHandler()
    {
        return this;
    }
}