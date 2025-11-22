#pragma once

#include <optional>
#include <string>

#include <boost/uuid/uuid.hpp>

#include <userver/storages/postgres/io/chrono.hpp>

#include <db/dto/auth/views.hpp>
#include <db/pg_ctx.hpp>

namespace db::api::auth {

std::optional<dto::auth::UserAuthData> GetUserAuthData(const db::PgCtx& pg, const std::string& email);

std::optional<boost::uuids::uuid> RegisterUser(const db::PgCtx& pg, const dto::auth::User& user);

void SaveUserToken(const db::PgCtx& pg, const dto::auth::UserToken& user_token);

void MarkTokenAsExpired(
    const db::PgCtx& pg,
    const std::string& token,
    const storages::postgres::TimePointTz expire_time
);

}  // namespace db::api::auth
