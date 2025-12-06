#pragma once

#include <boost/uuid/uuid.hpp>

#include <db/dto/issues/views.hpp>
#include <db/pg_ctx.hpp>

namespace db::api::issues {

dto::issues::CreateIssueResult CreateIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& author_id,
    const dto::issues::CreateIssueData& issue
);

dto::issues::UpdateIssueResult UpdateIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& issue_id,
    const dto::issues::UpdateIssueData& issue
);

std::optional<dto::issues::GetQueueIssuesResult> GetQueueIssues(
    const db::PgCtx& pg,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& queue_id,
    const std::string& search,
    int64_t limit,
    int64_t offset
);

dto::issues::GetIssueResult GetIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& issue_id
);

defs::common::ErrorCode DeleteIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& issue_id
);

}  // namespace db::api::issues
