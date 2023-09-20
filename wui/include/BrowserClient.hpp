#pragma once

#include "RendererHandler.hpp"
#include "include/cef_client.h"

namespace wui
{

    // Client actually handling the html internals
    class BrowserClient : public CefClient
    {
    public:
        CefRefPtr<CefRenderHandler> m_renderHandler;

    public:
        BrowserClient(CefRefPtr<wui::RendererHandler> &renderHandler)
            : m_renderHandler(renderHandler)
        {
        }

        virtual CefRefPtr<CefRenderHandler> GetRenderHandler()
        {
            return m_renderHandler;
        }

        IMPLEMENT_REFCOUNTING(BrowserClient);
    };
}