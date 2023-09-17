#include <include/cef_client.h>
#include <include/cef_app.h>

#include <pthread.h>

namespace wui
{

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

}
