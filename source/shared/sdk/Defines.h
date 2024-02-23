#if defined _WIN32
    #define TRILOGY_API extern "C" __declspec(dllexport)
#else
    #define TRILOGY_API extern "C" __attribute__ ((visibility ("default")))
#endif
