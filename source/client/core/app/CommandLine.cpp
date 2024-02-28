#include "StdInc.h"
#include "CommandLine.h"

#include <span>
#include <app/Debug.h>
#include <span>

namespace CommandLine {
    bool unhookAll{ false };
    std::vector<std::string_view> unhookSome{};
    std::vector<std::string_view> unhookExcept{};

    bool waitForDebugger{ false };

    void ProcessArgument(const char* arg) {
    }

    void CommandLine::Load(int argc, char** argv) {
    }
} // namespace CommandLine
