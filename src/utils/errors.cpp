#include "errors.hpp"

#include <userver/server/handlers/exceptions.hpp>

namespace utils {

void HandleQueueQueryErrors(const defs::common::ErrorCode& code) {
    switch (code) {
        case defs::common::ErrorCode::kOk:
            break;
        case defs::common::ErrorCode::kQueueNotFound:
            throw server::handlers::ResourceNotFound(
                server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kQueueNotFound)}
            );
            break;
        case defs::common::ErrorCode::kQueueOwnerMismatch:
            throw server::handlers::ClientError(
                server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kQueueOwnerMismatch)}
            );
            break;
        default:
            throw server::handlers::InternalServerError();
    }
}

void HandleIssueQueryErrors(const defs::common::ErrorCode& code) {
    switch (code) {
        case defs::common::ErrorCode::kOk:
            break;
        case defs::common::ErrorCode::kUserNotFound:
            throw server::handlers::ResourceNotFound(
                server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kUserNotFound)}
            );
            break;
        case defs::common::ErrorCode::kQueueNotFound:
            throw server::handlers::ResourceNotFound(
                server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kQueueNotFound)}
            );
            break;
        case defs::common::ErrorCode::kForbidden:
            throw server::handlers::Forbidden(
                server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kForbidden)}
            );
            break;
        case defs::common::ErrorCode::kIssueNotFound:
            throw server::handlers::ResourceNotFound(
                server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kIssueNotFound)}
            );
            break;
        default:
            throw server::handlers::InternalServerError();
    }
}

}  // namespace utils
