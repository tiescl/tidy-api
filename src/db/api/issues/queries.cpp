#include "queries.hpp"

#include <optional>
#include <string>

#include <userver/formats/parse/to.hpp>

#include <db/db_types.hpp>

#include <tidy_api/sql_queries.hpp>

#include <utils/constants.hpp>

#include <defs/errors.hpp>
#include <defs/issues.hpp>

namespace db::api::issues {

dto::issues::CreateIssueResult CreateIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& author_id,
    const dto::issues::Issue& issue
) {
    const auto pg_result = pg.MakeRwRequest(
        tidy_api::sql::kInsertIssue,
        queue_id,
        author_id,
        issue.assignee_id,
        issue.title,
        issue.description,
        issue.type,
        issue.status,
        issue.priority,
        issue.component,
        issue.story_points
    );

    return dto::issues::CreateIssueResult{
        .code = FromString(
            pg_result.Front()[utils::constants::kCode].As<std::string>(), formats::parse::To<defs::errors::ErrorCode>()
        ),
        .issue = pg_result.Front()[utils::constants::kIssue].As<std::optional<defs::issues::Issue>>()
    };
}

}  // namespace db::api::issues
