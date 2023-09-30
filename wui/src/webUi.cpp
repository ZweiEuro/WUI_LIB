
#include "webUi.hpp"
#include "RendererHandler.hpp"
#include "BrowserClient.hpp"
#include "webUiPrivat.hpp"

#include "include/cef_browser.h"

#include <filesystem>

namespace wui
{

    CefRefPtr<wui::RendererHandler> rendererHandler;
    CefRefPtr<wui::BrowserClient> browserClient;
    CefRefPtr<CefBrowser> browser;

    bool CEFInit(int argc, char **argv)
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
        DLOG(INFO) << "Starting Web UI";

        if (rendererHandler.get() != nullptr)
        {
            DLOG(ERROR) << "Web UI already started";
            return false;
        }

        CefWindowInfo window_info;
        window_info.SetAsWindowless(0); // false means no transparency (site background colour)

        DLOG(INFO) << "Renderer Init";
        rendererHandler = new wui::RendererHandler(pixelBuffer, initialHeight, initialWidth);

        DLOG(INFO) << "Browser Init";
        browserClient = new wui::BrowserClient(rendererHandler);

        CefBrowserSettings browserSettings;
        browserSettings.windowless_frame_rate = 60; // 30 is default

        // If path does not start with '/', it is relative to the current working directory.
        if (path.front() != '/')
        {
            path = std::filesystem::current_path().string() + "/" + path;
        }

        // if path does not end with '/', add it
        if (path.back() != '/')
        {
            path += "/";
        }

        path += "index.html";

        DLOG(INFO)
            << "Open File: " << path
            << " in Browser, with URL: "
            << "file://" + path;

        browser = CefBrowserHost::CreateBrowserSync(window_info, browserClient.get(), "file://" + path, browserSettings, nullptr, nullptr);

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

        if (rendererHandler->resize(newWidth, newHeight, newDestinationPixelBuffer))
        {
            browser->GetHost()->WasResized();
        }
    }

}
