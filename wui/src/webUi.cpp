
#include "webUi.hpp"
#include "RenderHandler.hpp"
#include "Client.hpp"
#include "webUiPrivat.hpp"
#include "App.hpp"

#include "include/cef_browser.h"

namespace wui
{

    CefMainArgs args;

    bool CEFInit(int argc, char **argv)
    {

        if (app.get() != nullptr)
        {
            DLOG(ERROR) << "Web UI already initialized";
            return false;
        }

        args = CefMainArgs(argc, argv);

        app = new wui::App();

        int pid = CefExecuteProcess(args, app, nullptr);

        // checkout CefApp, derive it and set it as second parameter, for more control on
        // command args and resources.
        if (pid >= 0)
        {
            // child process has nothin more to do
            exit(pid);
        }

        return true;
    }

    bool startWui(void **pixelBuffer, const size_t initialHeight, const size_t initialWidth, std::string path)
    {
        DLOG(INFO) << "Starting Web UI";

        app->GetWUIBrowserProcessHandler()->setClientViewParams(pixelBuffer, initialHeight, initialWidth, path);

        CefSettings settings;

        // checkout detailed settings options http://magpcss.org/ceforum/apidocs/projects/%28default%29/_cef_settings_t.html
        // nearly all the settings can be set via args too.
        // settings.multi_threaded_message_loop = true; // not supported, except windows
        // CefString(&settings.browser_subprocess_path).FromASCII("sub_proccess path, by default uses and starts this executeable as child");
        // CefString(&settings.cache_path).FromASCII("");
        // CefString(&settings.log_file).FromASCII("");
        // settings.log_severity = LOGSEVERITY_DEFAULT;
        // CefString(&settings.resources_dir_path).FromASCII("");
        // CefString(&settings.locales_dir_path).FromASCII("");

        settings.log_severity = LOGSEVERITY_INFO;
        settings.windowless_rendering_enabled = true;

#if !defined(CEF_USE_SANDBOX)
        settings.no_sandbox = true;
#endif

        // init custom scheme for local files

        bool init_res = CefInitialize(args, settings, app, nullptr);

        // CefInitialize creates a sub-proccess and executes the same executeable, as calling CefInitialize, if not set different in settings.browser_subprocess_path
        // if you create an extra program just for the childproccess you only have to call CefExecuteProcess(...) in it.
        if (!init_res)
        {
            // handle error
            DLOG(ERROR) << "CefInitialize failed";
            exit(-2);
        }

        return true;
    }

    void CEFRunMessageLoop()
    {
        CefRunMessageLoop();
    }

    void CEFShutdown()
    {
        DLOG(INFO) << "Shutting down CEF";
        CefShutdown();
    }

    void resizeUi(const size_t newWidth, const size_t newHeight, void **newDestinationPixelBuffer)
    {

        if (app->GetWUIBrowserProcessHandler()->GetClient()->GetWUIRenderHandler()->resize(newWidth, newHeight, newDestinationPixelBuffer))
        {
            app->GetWUIBrowserProcessHandler()->GetBrowser()->GetHost()->WasResized();
        }
    }

}
