#include "metalog.h"

int main() {
    auto& metalog_instace = Log::GetInstance();

    // metalog_instace.UniqueLog("shitfuck", deflog::VERBOSE, "% is", "healthy");
    for (size_t i = 0; i < 2000000; ++i) {
        metalog_instace.TimedLog(deflog::VERBOSE, 0, 200, "counter %", i);
    }
    return 0;
}