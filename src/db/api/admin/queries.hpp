#pragma once

#include <string>
#include <vector>

#include <db/pg_ctx.hpp>

#include <defs/admin.hpp>

namespace db::api::admin {

std::vector<defs::admin::AdminUserListItem>
GetUsers(const db::PgCtx& pg, const std::string& search, int64_t limit, int64_t offset);

}  // namespace db::api::admin
