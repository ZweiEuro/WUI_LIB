#include "include/cef_render_process_handler.h"
#include "include/wrapper/cef_message_router.h"

namespace wui
{

    // Implements main renderer process callbacks.
    // Needed to create the context for message routing

    class RenderProcessHandler : public CefRenderProcessHandler
    {
    private:
        CefRefPtr<CefMessageRouterRendererSide> message_router_;

        IMPLEMENT_REFCOUNTING(RenderProcessHandler);

        RenderProcessHandler(const RenderProcessHandler &) = delete;
        RenderProcessHandler &operator=(const RenderProcessHandler &) = delete;

    public:
        RenderProcessHandler();

        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
        {
            // This is very important since the default implementation returns "null" and
            // then onContextCreated will never be called
            return this;
        }

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
    };

}