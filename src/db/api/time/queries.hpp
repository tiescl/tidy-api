#pragma once

#include <userver/storages/postgres/io/chrono.hpp>

#include <db/pg_ctx.hpp>

namespace db::api {

storages::postgres::TimePointTz SelectNow(const db::PgCtx& pg);

}  // namespace db::api
