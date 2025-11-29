#pragma once

#include <optional>

#include <boost/uuid/uuid.hpp>

#include <defs/errors.hpp>
#include <defs/queues.hpp>

namespace db::dto::issues {

struct CreateIssueResult {
    defs::errors::ErrorCode code;
    std::optional<boost::uuids::uuid> issue_id;
};

using Issue = defs::queues::V1QueuesQueueIssuesPostRequest;

}  // namespace db::dto::issues
