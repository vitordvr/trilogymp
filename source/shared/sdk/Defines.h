#if defined _WIN32
    #define TRILOGY_API extern "C" __declspec(dllexport)
#else
    #define TRILOGY_API extern "C" __attribute__ ((visibility ("default")))
#endif

#ifndef NUMELMS     // from DShow.h
#define NUMELMS(aa) (sizeof(aa)/sizeof((aa)[0]))
#endif

#ifdef WIN32
#define PATH_SEPERATOR "\\"
#else
#define PATH_SEPERATOR "/"
#endif

// Use dassert for debug build only assertations
#if defined(TRILOGY_DEBUG) || defined(DEBUG) || defined(_DEBUG)
#define dassert assert
#else
#define dassert(_Expression)     ((void)0)
#ifdef WIN32
// This, along with RedirectedSetUnhandledExceptionFilter means we can get reports from all crashes with the correct crash address in the file name
#undef assert
#define assert(_Expression) (void)( (!!(_Expression)) || ( *((int*)NULL) = 0) )
#endif
#endif
