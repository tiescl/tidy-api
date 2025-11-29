#pragma once

#include <userver/server/handlers/exceptions.hpp>

#include <defs/errors.hpp>

namespace server::handlers {

class Forbidden : public ExceptionWithCode<HandlerErrorCode::kForbidden> {
public:
    using BaseType::BaseType;
};

}  // namespace server::handlers

namespace utils {

void HandleQueueQueryErrors(const defs::errors::ErrorCode& code);

void HandleIssueQueryErrors(const defs::errors::ErrorCode& code);

}  // namespace utils
