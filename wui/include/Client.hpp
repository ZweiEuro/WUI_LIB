#pragma once

#include "BufferRenderingHandler.hpp"
#include "include/cef_client.h"

namespace wui
{

    // Client actually handling the html internals
    class Client : public CefClient
    {
    private:
        CefRefPtr<CefRenderHandler> m_renderHandler;

    public:
        Client(CefRefPtr<wui::BufferRenderingHandler> &renderHandler);

        virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;

        IMPLEMENT_REFCOUNTING(Client);
    };
}