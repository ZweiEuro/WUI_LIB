#include "include/cef_app.h"

namespace wui
{

    // Implementation of CefApp for the renderer process.
    class App : public CefApp
    {

    private:
        // Handles the renderer side of query routing.
        CefRefPtr<CefRenderProcessHandler> render_process_handler_;

        IMPLEMENT_REFCOUNTING(App);

        App(const App &) = delete;
        App &operator=(const App &) = delete;

    public:
        App();

        // CefApp methods:
        CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;
    };
}