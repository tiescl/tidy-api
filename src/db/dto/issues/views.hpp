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

using GetIssueResult = IssueResult;

using Issue = defs::queues::V1QueuesQueueIssuesPostRequest;

}  // namespace db::dto::issues
