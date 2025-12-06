#pragma once

#include <userver/server/handlers/exceptions.hpp>

#include <docs/common.hpp>

namespace server::handlers {

class Forbidden : public ExceptionWithCode<HandlerErrorCode::kForbidden> {
public:
    using BaseType::BaseType;
};

}  // namespace server::handlers

namespace utils {

void HandleQueueQueryErrors(const defs::common::ErrorCode& code);

void HandleIssueQueryErrors(const defs::common::ErrorCode& code);

}  // namespace utils
