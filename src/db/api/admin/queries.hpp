#pragma once

#include <string>
#include <unordered_set>
#include <vector>

#include <boost/uuid/uuid.hpp>

#include <db/pg_ctx.hpp>

#include <defs/admin.hpp>
#include <defs/users.hpp>

namespace db::api::admin {

std::vector<defs::admin::AdminUserListItem>
GetUsers(const db::PgCtx& pg, const std::string& search, int64_t limit, int64_t offset);

void DeleteUsers(const db::PgCtx& pg, const std::unordered_set<boost::uuids::uuid>& user_ids);

bool UpdateUserRole(const db::PgCtx& pg, const boost::uuids::uuid& user_id, const defs::users::UserRole user_role);

std::vector<defs::admin::AdminQueueListItem>
GetQueues(const db::PgCtx& pg, const std::string& search, int64_t limit, int64_t offset);

void DeleteQueues(const db::PgCtx& pg, const std::unordered_set<boost::uuids::uuid>& queue_ids);

}  // namespace db::api::admin
