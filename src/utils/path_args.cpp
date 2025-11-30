#include "path_args.hpp"

#include <userver/server/handlers/exceptions.hpp>
#include <userver/utils/boost_uuid4.hpp>

#include <utils/constants.hpp>

#include <defs/errors.hpp>

namespace utils {

boost::uuids::uuid GetUserPathArg(const server::http::HttpRequest& http_request) try {
    return utils::BoostUuidFromString(http_request.GetPathArg(utils::constants::kUser));
} catch (const std::exception& exc) {
    throw server::handlers::ClientError(server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kInvalidUserId)
    });
}

boost::uuids::uuid GetQueuePathArg(const server::http::HttpRequest& http_request) try {
    return utils::BoostUuidFromString(http_request.GetPathArg(utils::constants::kQueue));
} catch (const std::exception& exc) {
    throw server::handlers::ClientError(server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kInvalidQueueId
    )});
}

boost::uuids::uuid GetIssuePathArg(const server::http::HttpRequest& http_request) try {
    return utils::BoostUuidFromString(http_request.GetPathArg(utils::constants::kIssue));
} catch (const std::exception& exc) {
    throw server::handlers::ClientError(server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kInvalidIssueId
    )});
}

}  // namespace utils
