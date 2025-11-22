#include "queries.hpp"

#include <userver/storages/postgres/io/row_types.hpp>

#include <db/db_types.hpp>

#include <tidy_api/sql_queries.hpp>

namespace db::api::users {

std::optional<std::string> GetUserPasswordHash(const db::PgCtx& pg, const boost::uuids::uuid& user_id) {
    const auto pg_result = pg.MakeRoRequest(tidy_api::sql::kSelectUserPasswordHash, user_id);

    return pg_result.AsOptionalSingleRow<std::string>();
}

void SetNewUserPasswordHash(const db::PgCtx& pg, const boost::uuids::uuid& user_id, const std::string& new_hash) {
    pg.MakeRwRequest(tidy_api::sql::kUpdateUserPasswordHash, user_id, new_hash);
}

std::optional<db::dto::users::UpdatedUser> UpdateUserData(
    const db::PgCtx& pg,
    const boost::uuids::uuid user_id,
    const std::optional<std::string> full_name,
    const std::optional<std::string> email,
    const std::optional<std::string> username
) {
    const auto pg_result = pg.MakeRwRequest(tidy_api::sql::kUpsertUserData, user_id, full_name, email, username);

    return pg_result.AsOptionalSingleRow<db::dto::users::UpdatedUser>(storages::postgres::kRowTag);
}

std::optional<db::dto::users::CurrentUser> GetCurrentUser(const db::PgCtx& pg, const boost::uuids::uuid& user_id) {
    const auto pg_result = pg.MakeRoRequest(tidy_api::sql::kSelectUserForMe, user_id);

    return pg_result.AsOptionalSingleRow<db::dto::users::CurrentUser>(storages::postgres::kRowTag);
}

}  // namespace db::api::users
