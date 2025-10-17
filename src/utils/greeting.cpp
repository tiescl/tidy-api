#include "greeting.hpp"

#include <format>

#include <userver/utils/assert.hpp>

namespace utils {

std::string SayHelloTo(std::string_view name, UserType type) {
    if (name.empty()) {
        name = "unknown user";
    }

    switch (type) {
        case UserType::kFirstTime:
            return std::format("Hello, {}!\n", name);
        case UserType::kKnown:
            return std::format("Hi again, {}!\n", name);
    }

    UASSERT(false);
}

}  // namespace utils
