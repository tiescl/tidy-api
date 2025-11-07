#include "greeting.hpp"

#include <format>

#include <userver/utils/assert.hpp>
#include <userver/utils/datetime.hpp>

namespace utils {

std::string SayHelloTo(std::string_view name, const UserType type, const storages::postgres::TimePointTz now) {
    if (name.empty()) {
        name = "unknown user";
    }

    std::string pretty_now = utils::datetime::Timestring(now);
    switch (type) {
        case UserType::kFirstTime:
            return std::format("Hello from {}, {}!\n", std::move(pretty_now), name);
        case UserType::kKnown:
            return std::format("Hi again from {}, {}!\n", std::move(pretty_now), name);
    }

    UASSERT(false);
}

}  // namespace utils
