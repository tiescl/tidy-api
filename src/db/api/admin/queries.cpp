#include "queries.hpp"

#include <userver/storages/postgres/io/row_types.hpp>

#include <db/db_types.hpp>

#include <tidy_api/sql_queries.hpp>

namespace db::api::admin {

std::vector<defs::admin::AdminUserListItem>
GetUsers(const db::PgCtx& pg, const std::string& search, int64_t limit, int64_t offset) {
    const auto pg_result = pg.MakeRoRequest(tidy_api::sql::kSelectUsersForAdmin, search, limit, offset);

    return pg_result.AsContainer<std::vector<defs::admin::AdminUserListItem>>(storages::postgres::kRowTag);
}

void DeleteUsers(const db::PgCtx& pg, const std::unordered_set<boost::uuids::uuid>& user_ids) {
    pg.MakeRwRequest(tidy_api::sql::kDeleteUsersForAdmin, user_ids);
}

bool UpdateUserRole(const db::PgCtx& pg, const boost::uuids::uuid& user_id, const defs::users::UserRole user_role) {
    const auto pg_result = pg.MakeRwRequest(tidy_api::sql::kUpdateUserRoleForAdmin, user_id, user_role);

    return pg_result.RowsAffected();
}

std::vector<defs::admin::AdminQueueListItem>
GetQueues(const db::PgCtx& pg, const std::string& search, int64_t limit, int64_t offset) {
    const auto pg_result = pg.MakeRoRequest(tidy_api::sql::kSelectQueuesForAdmin, search, limit, offset);

    return pg_result.AsContainer<std::vector<defs::admin::AdminQueueListItem>>(storages::postgres::kRowTag);
}

bool DeleteQueue(const db::PgCtx& pg, const boost::uuids::uuid& queue_id) {
    const auto pg_result = pg.MakeRwRequest(tidy_api::sql::kDeleteQueueForAdmin, queue_id);

    return pg_result.RowsAffected();
}

}  // namespace db::api::admin
