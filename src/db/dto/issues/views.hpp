#pragma once

#include <optional>

#include <boost/uuid/uuid.hpp>

#include <docs/common.hpp>
#include <docs/issues.hpp>
#include <docs/queues.hpp>

namespace db::dto::issues {

struct IssueResult {
    defs::common::ErrorCode code;
    std::optional<defs::issues::Issue> issue;
};

using CreateIssueResult = IssueResult;

using UpdateIssueResult = IssueResult;

using GetIssueResult = IssueResult;

using CreateIssueData = defs::queues::V1QueuesQueueIssuesPostRequest;

using UpdateIssueData = defs::queues::V1QueuesQueueIssuesIssuePutRequest;

}  // namespace db::dto::issues
