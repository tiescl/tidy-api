#include "queries.hpp"

#include <optional>
#include <string>

#include <userver/formats/parse/to.hpp>

#include <db/db_types.hpp>

#include <tidy_api/sql_queries.hpp>

#include <utils/constants.hpp>

#include <docs/common.hpp>
#include <docs/issues.hpp>

namespace db::api::issues {

dto::issues::CreateIssueResult CreateIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& author_id,
    const dto::issues::CreateIssueData& issue
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
            pg_result.Front()[utils::constants::kCode].As<std::string>(), formats::parse::To<defs::common::ErrorCode>()
        ),
        .issue = pg_result.Front()[utils::constants::kIssue].As<std::optional<defs::issues::Issue>>()
    };
}

dto::issues::UpdateIssueResult UpdateIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& issue_id,
    const dto::issues::UpdateIssueData& issue
) {
    const auto pg_result = pg.MakeRwRequest(
        tidy_api::sql::kUpdateIssue,
        user_id,
        queue_id,
        issue_id,
        issue.title,
        issue.description,
        issue.type,
        issue.status,
        issue.priority,
        issue.component,
        issue.story_points,
        issue.assignee_id
    );

    return dto::issues::UpdateIssueResult{
        .code = FromString(
            pg_result.Front()[utils::constants::kCode].As<std::string>(), formats::parse::To<defs::common::ErrorCode>()
        ),
        .issue = pg_result.Front()[utils::constants::kIssue].As<std::optional<defs::issues::Issue>>()
    };
}

dto::issues::GetIssueResult GetIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& issue_id
) {
    const auto pg_result = pg.MakeRoRequest(tidy_api::sql::kSelectIssue, user_id, queue_id, issue_id);

    return dto::issues::GetIssueResult{
        .code = FromString(
            pg_result.Front()[utils::constants::kCode].As<std::string>(), formats::parse::To<defs::common::ErrorCode>()
        ),
        .issue = pg_result.Front()[utils::constants::kIssue].As<std::optional<defs::issues::Issue>>()
    };
}

defs::common::ErrorCode DeleteIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& issue_id
) {
    const auto pg_result = pg.MakeRwRequest(tidy_api::sql::kDeleteIssue, user_id, queue_id, issue_id);

    return FromString(pg_result.AsSingleRow<std::string>(), formats::parse::To<defs::common::ErrorCode>());
}

}  // namespace db::api::issues
