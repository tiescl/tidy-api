#pragma once

#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

#include <boost/uuid/uuid.hpp>

#include <db/dto/queues/views.hpp>
#include <db/pg_ctx.hpp>

#include <defs/errors.hpp>
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

std::optional<defs::queues::Queue>
GetQueue(const db::PgCtx& pg, const boost::uuids::uuid& user_id, const boost::uuids::uuid& queue_id);

std::optional<defs::queues::Queue> CreateQueue(const db::PgCtx& pg, const dto::queues::Queue& queue);

defs::errors::ErrorCode
DeleteQueue(const db::PgCtx& pg, const boost::uuids::uuid& queue_id, const boost::uuids::uuid& owner_id);

defs::errors::ErrorCode CreateQueueRolePermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const defs::users::UserRole role,
    const std::unordered_set<defs::issues::IssueAction>& actions,
    const boost::uuids::uuid& owner_id
);

defs::errors::ErrorCode CreateQueueUserPermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& user_id,
    const std::unordered_set<defs::issues::IssueAction>& actions,
    const boost::uuids::uuid& owner_id
);

defs::errors::ErrorCode DeleteQueueRolePermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const defs::users::UserRole role,
    const boost::uuids::uuid& owner_id
);

defs::errors::ErrorCode DeleteQueueUserPermission(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& owner_id
);

}  // namespace db::api::queues
