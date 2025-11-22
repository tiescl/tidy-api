#pragma once

#include <optional>
#include <string>

#include <boost/uuid/uuid.hpp>

#include <db/dto/users/views.hpp>
#include <db/pg_ctx.hpp>

namespace db::api::users {

std::optional<std::string> GetUserPasswordHash(const db::PgCtx& pg, const boost::uuids::uuid& user_id);

void SetNewUserPasswordHash(const db::PgCtx& pg, const boost::uuids::uuid& user_id, const std::string& new_hash);

std::optional<db::dto::users::UpdatedUser> UpdateUserData(
    const db::PgCtx& pg,
    const boost::uuids::uuid user_id,
    const std::optional<std::string> full_name,
    const std::optional<std::string> email,
    const std::optional<std::string> username
);

std::optional<db::dto::users::CurrentUser> GetCurrentUser(const db::PgCtx& pg, const boost::uuids::uuid& user_id);

}  // namespace db::api::users
