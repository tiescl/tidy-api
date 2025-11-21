#pragma once

#include <string>

#include <boost/uuid/uuid.hpp>

#include <userver/server/auth/user_auth_info.hpp>
#include <userver/storages/postgres/io/chrono.hpp>

#include <defs/users.hpp>

namespace db::dto::auth {

struct UserAuthData {
    boost::uuids::uuid user_id;
    std::string email;
    std::string password_hash;
};

struct User {
    std::string username;
    std::string email;
    std::string password_hash;
    defs::users::UserRole role;
};

struct UserToken {
    server::auth::UserAuthInfo::Ticket token;
    boost::uuids::uuid user_id;
    defs::users::UserRole user_role;
    storages::postgres::TimePointTz expires_at;
    storages::postgres::TimePointTz updated_at;
};

}  // namespace db::dto::auth
