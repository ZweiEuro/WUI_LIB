#pragma once

#include "RenderHandler.hpp"
#include "include/cef_client.h"

namespace wui
{

    // Client actually handling the html internals
    // the client also holds the renderHandler
    class Client : public CefClient
    {
    private:
        CefRefPtr<RenderHandler> m_renderHandler;

        IMPLEMENT_REFCOUNTING(Client);

    public:
        Client(void **destinationPixelBuffer, const size_t height, const size_t width);

        virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;

        CefRefPtr<RenderHandler> GetWUIRenderHandler();

        // WUI
    };
}