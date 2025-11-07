#include "queries.hpp"

#include <tidy_api/sql_queries.hpp>

namespace db::api {

storages::postgres::TimePointTz SelectNow(const db::PgCtx& pg) {
    const auto pg_result = pg.MakeRoRequest(tidy_api::sql::kSelectNow);

    return pg_result.AsSingleRow<storages::postgres::TimePointTz>();
}

}  // namespace db::api
