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

        CefRefPtr<CefV8Value> globalObject = context->GetGlobal();
        v8Handler_ = new V8Handler();
        // Create an object "wui", attach both functions to it, attach it to the global object
        CefRefPtr<CefV8Value> wuiObject = CefV8Value::CreateObject(nullptr, nullptr);

        wuiObject->SetValue("sendEvent", CefV8Value::CreateFunction("sendEvent", v8Handler_), V8_PROPERTY_ATTRIBUTE_READONLY);
        wuiObject->SetValue("registerEventListener", CefV8Value::CreateFunction("sendEvent", v8Handler_), V8_PROPERTY_ATTRIBUTE_READONLY);

        globalObject->SetValue("wui", wuiObject, V8_PROPERTY_ATTRIBUTE_READONLY);
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

    bool RenderProcessHandler::addEventListener(const char *eventName, eventListenerFunction_t function)
    {
        return v8Handler_->addEventListener(eventName, function);
    }

    bool RenderProcessHandler::removeEventListener(const char *eventName)
    {
        return v8Handler_->removeEventListener(eventName);
    }

}