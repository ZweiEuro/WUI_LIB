#pragma once

#include "RenderHandler.hpp"
#include "include/cef_client.h"

namespace wui
{

    // Client actually handling the html internals
    class Client : public CefClient
    {
    private:
        CefRefPtr<CefRenderHandler> m_renderHandler;

    public:
        Client(CefRefPtr<wui::RenderHandler> &renderHandler);

        virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;

        IMPLEMENT_REFCOUNTING(Client);
    };
}