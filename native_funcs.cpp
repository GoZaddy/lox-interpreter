// #include "native_funcs.h"

#include "declr.h"


typedef std::chrono::milliseconds milliseconds;

int Clock::arity() { return 0; }

rv Clock::call(
    Interpreter* interpreter,
    std::vector<rv> arguments
) {
    milliseconds ms = std::chrono::duration_cast< milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()
    );
    return new String(std::to_string(ms.count()));
}

std::string Clock::toString() { return "<native fn>"; }