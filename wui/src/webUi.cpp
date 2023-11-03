
#include "webUi.hpp"
#include "RenderHandler.hpp"
#include "Client.hpp"
#include "webUiPrivat.hpp"

#include "include/cef_browser.h"
#include "cefUtil.hpp"

namespace wui
{

    CefMainArgs args;

    /**
     * Dev Notes:
     * separate the processes maybe? https://bitbucket.org/chromiumembedded/cef/wiki/GeneralUsage.md#markdown-header-processes
     * search "Application Structure"
     * (separate processes would enable mac support... i think)
     * */

    bool CEFInit(int argc, char **argv)
    {

        if (browserProcessExists())
        {
            DLOG(ERROR) << "Web UI already initialized";
            return false;
        }

        args = CefMainArgs(argc, argv);
        auto commandLine = CreateCommandLine(args);

        CefRefPtr<CefApp> app = nullptr;

        switch (GetProcessType(commandLine))
        {
        case PROCESS_TYPE_BROWSER:
            DLOG(INFO) << "Initializing Web UI browser process " << GetProcessType(commandLine);
            browserProcessApp = new BrowserProcessHandler();
            app = browserProcessApp;
            break;
        case PROCESS_TYPE_RENDERER:
            DLOG(INFO) << "Initializing Web UI renderer process " << GetProcessType(commandLine);
            renderProcessApp = new RenderProcessHandler();
            app = renderProcessApp;
            break;
        case PROCESS_TYPE_LINUX_ZYGOTE:
            DLOG(INFO) << "Initializing Web UI linux zygote process " << GetProcessType(commandLine);
            app = new RenderProcessHandler();
            break;
        case PROCESS_TYPE_OTHER:
            DLOG(INFO) << "Initializing Web UI other process " << GetProcessType(commandLine);
            app = new RenderProcessHandler();
            break;

        default:
            break;
        }

        int exit_code = CefExecuteProcess(args, app.get(), nullptr);

        // checkout CefApp, derive it and set it as second parameter, for more control on
        // command args and resources.
        if (exit_code >= 0)
        {
            // child process has nothin more to do
            exit(exit_code);
        }
        else
        {
            DLOG(INFO) << "Web UI initialized " << exit_code << " (-1 is good)";
        }

        return true;
    }

    bool startWui(void **pixelBuffer, const size_t initialHeight, const size_t initialWidth, std::string path)
    {
        DLOG(INFO) << "Starting Web UI";

        browserProcessApp->GetWUIBrowserProcessHandler()->setClientViewParams(pixelBuffer, initialHeight, initialWidth, path);

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

        bool init_res = CefInitialize(args, settings, browserProcessApp.get(), nullptr);

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

        if (browserProcessApp->GetWUIBrowserProcessHandler()->GetClient()->GetWUIRenderHandler()->resize(newWidth, newHeight, newDestinationPixelBuffer))
        {
            browserProcessApp->GetWUIBrowserProcessHandler()->GetBrowser()->GetHost()->WasResized();
        }
    }

}
