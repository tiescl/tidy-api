#pragma once

#include <string>

#include <defs/auth.hpp>

namespace db::dto::auth {

struct User {
    std::string username;
    std::string email;
    std::string password_hash;
    defs::auth::UserRole role;
};

}  // namespace db::dto::auth
