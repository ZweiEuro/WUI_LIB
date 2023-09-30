#include "Client.hpp"

namespace wui
{

    Client::Client(CefRefPtr<wui::BufferRenderingHandler> &renderHandler)
        : m_renderHandler(renderHandler)
    {
    }

    CefRefPtr<CefRenderHandler> Client::GetRenderHandler()
    {
        return m_renderHandler;
    }

}