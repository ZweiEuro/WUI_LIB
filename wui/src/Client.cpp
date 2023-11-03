#include "Client.hpp"
#include "RenderHandler.hpp"
#include "include/wrapper/cef_helpers.h"
#include "messageRouter/MessageHandler.hpp"
#include "messageRouter/messageRouterConfig.hpp"

namespace wui
{

    Client::Client(std::shared_ptr<wui::MessageHandler> message_handler, void **destinationPixelBuffer, const size_t height, const size_t width)

    {
        this->render_handler_ = new RenderHandler(destinationPixelBuffer, height, width);
        this->message_handler_ = message_handler;
    }

    CefRefPtr<CefRenderHandler> Client::GetRenderHandler()
    {
        return render_handler_;
    }

    CefRefPtr<RenderHandler> Client::GetWUIRenderHandler()
    {
        return render_handler_;
    }

    bool Client::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message)
    {
        CEF_REQUIRE_UI_THREAD();

        return message_router_->OnProcessMessageReceived(browser, frame,
                                                         source_process, message);
    }

    void Client::OnAfterCreated(CefRefPtr<CefBrowser> browser)
    {
        CEF_REQUIRE_UI_THREAD();

        if (!message_router_)
        {
            DLOG(INFO) << "Creating message router";
            // Create the browser-side router for query handling.
            message_router_ = CefMessageRouterBrowserSide::Create(getMessageRouterConfig());

            // Register handlers with the router.
            message_router_->AddHandler(message_handler_.get(), false);
        }

        browser_ct_++;
    }

    void Client::OnBeforeClose(CefRefPtr<CefBrowser> browser)
    {
        CEF_REQUIRE_UI_THREAD();

        if (--browser_ct_ == 0)
        {

            DLOG(INFO) << "Destroying message router";

            // Free the router when the last browser is closed.
            message_router_->RemoveHandler(message_handler_.get());
            message_handler_.reset();
            message_router_ = nullptr;
        }
    }

    bool Client::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefRequest> request,
                                bool user_gesture,
                                bool is_redirect)
    {
        CEF_REQUIRE_UI_THREAD();

        message_router_->OnBeforeBrowse(browser, frame);
        return false;
    }

    void Client::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                           TerminationStatus status)
    {
        CEF_REQUIRE_UI_THREAD();

        message_router_->OnRenderProcessTerminated(browser);
    }

}