#pragma once

#include <boost/uuid/uuid.hpp>

#include <userver/server/http/http_request.hpp>

namespace utils {

boost::uuids::uuid GetUserPathArg(const server::http::HttpRequest& http_request);

boost::uuids::uuid GetQueuePathArg(const server::http::HttpRequest& http_request);

boost::uuids::uuid GetIssuePathArg(const server::http::HttpRequest& http_request);

}  // namespace utils
