
#include "webUi.hpp"
#include "webUiInput.hpp"
#include "RendererHandler.hpp"
#include "BrowserClient.hpp"

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

    bool resizeUi(const size_t newHeight, const size_t newWidth, void **pixelbuffer)
    {
        return false;
    }

    bool CEFSendMouseClickEvent(const wui_mouse_event_t &event, wui_mouse_button_type_t type, bool mouseUp,
                                int clickCount, bool force)
    {

        DLOG(INFO) << "Send Mouse Click Event: "
                   << " X: " << event.x
                   << " Y: " << event.y
                   << " Type: " << type
                   << " MouseUp: " << mouseUp
                   << " ClickCount: " << clickCount;

        if (force == false && rendererHandler->coordinateEmpty(event.x,
                                                               event.y))
        {
            DLOG(INFO) << "Coordinate Empty";
            return false;
        }

        cef_mouse_event_t cefEvent = {
            .x = event.x,
            .y = event.y,
            .modifiers = event.modifiers,
        };

        browser->GetHost()->SendMouseClickEvent(CefMouseEvent(cefEvent), (CefBrowserHost::MouseButtonType)type, mouseUp, clickCount);

        return true;
    }

    void CEFSendMouseMoveEvent(const wui_mouse_event_t &event, bool mouseLeave)
    {
        /*      DLOG(INFO) << "Send Mouse Move Event: "
                         << " X: " << event.x
                         << " Y: " << event.y
                         << " MouseLeave: " << mouseLeave;*/

        cef_mouse_event_t cefEvent = {
            .x = event.x,
            .y = event.y,
            .modifiers = event.modifiers,
        };

        browser->GetHost()->SendMouseMoveEvent(CefMouseEvent(cefEvent), mouseLeave);
    }

    bool CEFSendMouseWheelEvent(const wui_mouse_event_t &event, int deltaX, int deltaY, bool force)
    {

        DLOG(INFO) << "Send Mouse Wheel Event: "
                   << " X: " << event.x
                   << " Y: " << event.y
                   << " DeltaX: " << deltaX
                   << " DeltaY: " << deltaY;

        if (force == false && rendererHandler->coordinateEmpty(event.x,
                                                               event.y))
        {
            DLOG(INFO) << "Coordinate Empty";
            return false;
        }

        cef_mouse_event_t cefEvent = {
            .x = event.x,
            .y = event.y,
            .modifiers = event.modifiers,
        };

        browser->GetHost()->SendMouseWheelEvent(CefMouseEvent(cefEvent), deltaX, deltaY);
        return true;
    }

    void CEFSendKeyEvent(const wui_key_event_t &event)
    {
        DLOG(INFO) << "Send Key Event: "
                   << " Type: " << event.type
                   << " Modifiers: " << event.modifiers
                   << " WindowsKeyCode: " << event.windows_key_code
                   << " NativeKeyCode: " << event.native_key_code
                   << " IsSystemKey: " << event.is_system_key
                   << " FocusOnEditableField: " << event.focus_on_editable_field;

        cef_key_event_t cefEvent = {
            .type = (cef_key_event_type_t)event.type,
            .modifiers = event.modifiers,
            .windows_key_code = event.windows_key_code,
            .native_key_code = event.native_key_code,
            .is_system_key = event.is_system_key,
            .character = event.character,
            .unmodified_character = event.unmodified_character,
            .focus_on_editable_field = event.focus_on_editable_field,
        };

        browser->GetHost()->SendKeyEvent(CefKeyEvent(cefEvent));
    }

}
