#include "metalog.h"

struct new_log_struct {
    enum type : int {
        VERBOSE = 0,
        BASIC = 1,
        INTERMEDIATE = 2,
        ADVANCED = 3
    };
    std::string ToString(type tp) {
        switch (tp)
        {
        case VERBOSE:
            return "verbose";
        case BASIC:
            return "basic";
        case INTERMEDIATE:
            return "intermediate";
        case ADVANCED:
            return "advanced";
        }
    }
};
int main() {
    auto& metalog_instance = MetaLog<deflog>::GetInstance();
    metalog_instance.Init(deflog::ERROR, output::out | output::logfile, "/home/usr/test_log");
    metalog_instance.UniqueLog(deflog::ERROR, "the state is %\n", "A"); // prints
    metalog_instance.UniqueLog(deflog::ERROR, "the state is %\n", "A"); // nope
    metalog_instance.UniqueLog(deflog::ERROR, "the state is %\n", "A"); // nope
    metalog_instance.UniqueLog(deflog::ERROR, "the state is %\n", "B"); // prints
    metalog_instance.UniqueLog(deflog::ERROR, "the state is %\n", "A"); // nope
    for (size_t i = 0; i < 50000000; ++i) {
        metalog_instance.TimedLog(deflog::ERROR, 0, 200, ">>>>>>>>>>>> timed log %\n", i);
        metalog_instance.Log(deflog::ERROR, "<<<<<<<<<<<< normal log %\n", i);
    }
    metalog_instance.FlushStreams();
    return 0;
}