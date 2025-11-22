#include "queries.hpp"

#include <userver/storages/postgres/io/row_types.hpp>

#include <db/db_types.hpp>

#include <tidy_api/sql_queries.hpp>

namespace db::api::users {

std::optional<db::dto::users::CurrentUser> GetCurrentUser(const db::PgCtx& pg, const boost::uuids::uuid& user_id) {
    const auto pg_result = pg.MakeRoRequest(tidy_api::sql::kSelectUserForMe, user_id);

    return pg_result.AsOptionalSingleRow<db::dto::users::CurrentUser>(storages::postgres::kRowTag);
}

}  // namespace db::api::users
