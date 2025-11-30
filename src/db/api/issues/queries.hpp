#pragma once

#include <boost/uuid/uuid.hpp>

#include <db/dto/issues/views.hpp>
#include <db/pg_ctx.hpp>

namespace db::api::issues {

dto::issues::CreateIssueResult CreateIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& author_id,
    const dto::issues::Issue& issue
);

dto::issues::GetIssueResult GetIssue(
    const db::PgCtx& pg,
    const boost::uuids::uuid& user_id,
    const boost::uuids::uuid& queue_id,
    const boost::uuids::uuid& issue_id
);

}  // namespace db::api::issues
