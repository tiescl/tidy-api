#pragma once

#include <optional>

#include <boost/uuid/uuid.hpp>

#include <defs/errors.hpp>
#include <defs/issues.hpp>
#include <defs/queues.hpp>

namespace db::dto::issues {

struct CreateIssueResult {
    defs::errors::ErrorCode code;
    std::optional<defs::issues::Issue> issue;
};

using Issue = defs::queues::V1QueuesQueueIssuesPostRequest;

}  // namespace db::dto::issues
