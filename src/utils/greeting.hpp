#pragma once

#include <string>
#include <string_view>

#include <userver/storages/postgres/io/chrono.hpp>

namespace utils {

enum class UserType { kFirstTime, kKnown };

std::string SayHelloTo(std::string_view name, const UserType type, const storages::postgres::TimePointTz now);

}  // namespace utils
