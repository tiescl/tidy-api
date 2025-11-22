#pragma once

#include <string>
#include <vector>

#include <boost/uuid/uuid.hpp>

#include <db/pg_ctx.hpp>

#include <defs/admin.hpp>

namespace db::api::admin {

std::vector<defs::admin::AdminUserListItem>
GetUsers(const db::PgCtx& pg, const std::string& search, int64_t limit, int64_t offset);

bool DeleteUser(const db::PgCtx& pg, const boost::uuids::uuid& user_id);

}  // namespace db::api::admin
