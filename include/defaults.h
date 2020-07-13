#ifndef __METALOG_DEFAULTs_H__
#define __METALOG_DEFAULTs_H__

#include <string>

#ifdef __unix__
#define DEFAULT_LOGFILE "~/metalog"
#elif defined(_WIN32)
#define DEFAULT_LOGFILE "c:\\metalog";
#endif

struct deflog {
    enum type : int {
        VERBOSE = 0,
        WARNING = 1,
        ERROR = 2
    };
    static std::string ToString(deflog::type _tp) {
        switch (_tp)
        {
        case VERBOSE:
            return "verbose";
        case WARNING:
            return "warning";
        case ERROR:
            return "error";
        }
    }

};

#endif