#pragma once

#include <optional>

#include <boost/uuid/uuid.hpp>

#include <defs/errors.hpp>
#include <defs/issues.hpp>
#include <defs/queues.hpp>

namespace db::dto::issues {

struct IssueResult {
    defs::errors::ErrorCode code;
    std::optional<defs::issues::Issue> issue;
};

using CreateIssueResult = IssueResult;

using UpdateIssueResult = IssueResult;

using GetIssueResult = IssueResult;

using CreateIssueData = defs::queues::V1QueuesQueueIssuesPostRequest;

using UpdateIssueData = defs::queues::V1QueuesQueueIssuesIssuePutRequest;

}  // namespace db::dto::issues
