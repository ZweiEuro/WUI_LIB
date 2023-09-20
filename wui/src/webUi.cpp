
#include "webUi.hpp"
#include "RendererHandler.hpp"
#include "BrowserClient.hpp"

namespace wui
{

    CefRefPtr<wui::RendererHandler> rendererHandler;
    CefRefPtr<wui::BrowserClient> browserClient;
    CefRefPtr<CefBrowser> browser;

    bool initCEF(int argc, char **argv)
    {
        CefMainArgs args(argc, argv);

        int pid = CefExecuteProcess(args, nullptr, nullptr);

        // checkout CefApp, derive it and set it as second parameter, for more control on
        // command args and resources.
        if (pid >= 0)
        {
            // child process has nothin more to do
            exit(pid);
        }

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

        bool init_res = CefInitialize(args, settings, nullptr, nullptr);

        // CefInitialize creates a sub-proccess and executes the same executeable, as calling CefInitialize, if not set different in settings.browser_subprocess_path
        // if you create an extra program just for the childproccess you only have to call CefExecuteProcess(...) in it.
        if (!init_res)
        {
            // handle error
            DLOG(ERROR) << "CefInitialize failed";
            exit(-2);
        }

        return init_res;
    }

    bool startWui(void **pixelBuffer, const size_t initialHeight, const size_t initialWidth, std::string path)
    {
        if (rendererHandler.get() != nullptr)
        {
            DLOG(ERROR) << "Web UI already started";
            return false;
        }

        CefWindowInfo window_info;
        window_info.SetAsWindowless(0); // false means no transparency (site background colour)

        rendererHandler = new wui::RendererHandler(pixelBuffer, initialHeight, initialWidth);
        browserClient = new wui::BrowserClient(rendererHandler);

        CefBrowserSettings browserSettings;
        browserSettings.windowless_frame_rate = 60; // 30 is default

        browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), path, browserSettings, nullptr, nullptr);

        return true;
    }

    bool resizeUi(const size_t newHeight, const size_t newWidth, void **pixelbuffer)
    {
        return false;
    }
}
