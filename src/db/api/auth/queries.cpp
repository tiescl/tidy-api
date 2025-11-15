#include "queries.hpp"

#include <db/db_types.hpp>

#include <tidy_api/sql_queries.hpp>

namespace db::api::auth {

std::optional<boost::uuids::uuid> RegisterUser(const db::PgCtx& pg, const dto::auth::User user) {
    const auto pg_result =
        pg.MakeRwRequest(tidy_api::sql::kInsertUser, user.username, user.email, user.password_hash, user.role);

    return pg_result.AsOptionalSingleRow<boost::uuids::uuid>();
}

}  // namespace db::api::auth
