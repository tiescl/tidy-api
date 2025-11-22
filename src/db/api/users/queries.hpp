#pragma once

#include <optional>

#include <boost/uuid/uuid.hpp>

#include <db/dto/users/views.hpp>
#include <db/pg_ctx.hpp>

namespace db::api::users {

std::optional<db::dto::users::CurrentUser> GetCurrentUser(const db::PgCtx& pg, const boost::uuids::uuid& user_id);

}  // namespace db::api::users
