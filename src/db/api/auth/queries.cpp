#include "queries.hpp"

#include <db/db_types.hpp>

#include <tidy_api/sql_queries.hpp>

namespace db::api::auth {

std::optional<boost::uuids::uuid> RegisterUser(const db::PgCtx& pg, const dto::auth::User& user) {
    const auto pg_result =
        pg.MakeRwRequest(tidy_api::sql::kInsertUser, user.username, user.email, user.password_hash, user.role);

    return pg_result.AsOptionalSingleRow<boost::uuids::uuid>();
}

void SaveUserToken(const db::PgCtx& pg, const dto::auth::UserToken& user_token) {
    pg.MakeRwRequest(
        tidy_api::sql::kInsertUserToken,
        user_token.token,
        user_token.user_id,
        user_token.user_role,
        user_token.expires_at,
        user_token.updated_at
    );
}

}  // namespace db::api::auth
