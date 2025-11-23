#include "queries.hpp"

#include <userver/storages/postgres/io/row_types.hpp>

#include <db/db_types.hpp>

#include <tidy_api/sql_queries.hpp>

namespace db::api::queues {

std::vector<defs::queues::Queue> GetAvailableQueues(
    const db::PgCtx& pg,
    const boost::uuids::uuid& user_id,
    const std::string& search,
    const int64_t limit,
    const int64_t offset
) {
    const auto pg_result = pg.MakeRoRequest(tidy_api::sql::kSelectQueues, user_id, search, limit, offset);

    return pg_result.AsContainer<std::vector<defs::queues::Queue>>(storages::postgres::kRowTag);
}

std::optional<boost::uuids::uuid> CreateQueue(const db::PgCtx& pg, const dto::queues::Queue& queue) {
    const auto pg_result = pg.MakeRwRequest(tidy_api::sql::kInsertQueue, queue.key, queue.name, queue.owner_id);

    return pg_result.AsOptionalSingleRow<boost::uuids::uuid>();
}

std::string DeleteQueue(const db::PgCtx& pg, const boost::uuids::uuid& queue_id, const boost::uuids::uuid& owner_id) {
    const auto pg_result = pg.MakeRwRequest(tidy_api::sql::kDeleteQueue, queue_id, owner_id);

    return pg_result.AsSingleRow<std::string>();
}

std::string CreateQueueRolePermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const defs::users::UserRole role,
    const std::vector<defs::issues::IssueAction>& actions,
    const boost::uuids::uuid& owner_id
) {
    const auto pg_result =
        pg.MakeRwRequest(tidy_api::sql::kInsertQueueRolePermission, queue_id, role, actions, owner_id);

    return pg_result.AsSingleRow<std::string>();
}

std::string CreateQueueUserPermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& user_id,
    const std::vector<defs::issues::IssueAction>& actions,
    const boost::uuids::uuid& owner_id
) {
    const auto pg_result =
        pg.MakeRwRequest(tidy_api::sql::kInsertQueueUserPermission, queue_id, user_id, actions, owner_id);

    return pg_result.AsSingleRow<std::string>();
}

std::string DeleteQueueRolePermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const defs::users::UserRole role,
    const boost::uuids::uuid& owner_id
) {
    const auto pg_result = pg.MakeRwRequest(tidy_api::sql::kDeleteQueueRolePermission, queue_id, role, owner_id);

    return pg_result.AsSingleRow<std::string>();
}

std::string DeleteQueueUserPermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& owner_id
) {
    const auto pg_result = pg.MakeRwRequest(tidy_api::sql::kDeleteQueueUserPermission, queue_id, user_id, owner_id);

    return pg_result.AsSingleRow<std::string>();
}

}  // namespace db::api::queues
