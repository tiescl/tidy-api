#pragma once

#include <optional>

#include <boost/uuid/uuid.hpp>

#include <db/dto/auth/views.hpp>
#include <db/pg_ctx.hpp>

namespace db::api::auth {

std::optional<boost::uuids::uuid> RegisterUser(const db::PgCtx& pg, const dto::auth::User user);

}  // namespace db::api::auth
