#pragma once

#include <optional>
#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>

#include <db/dto/queues/views.hpp>
#include <db/pg_ctx.hpp>

#include <defs/issues.hpp>
#include <defs/queues.hpp>
#include <defs/users.hpp>

namespace db::api::queues {

std::vector<defs::queues::Queue> GetAvailableQueues(
    const db::PgCtx& pg,
    const boost::uuids::uuid& user_id,
    const std::string& search,
    const int64_t limit,
    const int64_t offset
);

std::optional<boost::uuids::uuid> CreateQueue(const db::PgCtx& pg, const dto::queues::Queue& queue);

std::string DeleteQueue(const db::PgCtx& pg, const boost::uuids::uuid& queue_id, const boost::uuids::uuid& owner_id);

std::string CreateQueueRolePermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const defs::users::UserRole role,
    const std::vector<defs::issues::IssueAction>& actions,
    const boost::uuids::uuid& owner_id
);

std::string CreateQueueUserPermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& user_id,
    const std::vector<defs::issues::IssueAction>& actions,
    const boost::uuids::uuid& owner_id
);

std::string DeleteQueueRolePermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const defs::users::UserRole role,
    const boost::uuids::uuid& owner_id
);

std::string DeleteQueueUserPermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& owner_id
);

}  // namespace db::api::queues
