#include "errors.hpp"

#include <userver/server/handlers/exceptions.hpp>

namespace utils {

void HandleQueueQueryErrors(const defs::errors::ErrorCode& code) {
    switch (code) {
        case defs::errors::ErrorCode::kOk:
            break;
        case defs::errors::ErrorCode::kQueueNotFound:
            throw server::handlers::ResourceNotFound(
                server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kQueueNotFound)}
            );
            break;
        case defs::errors::ErrorCode::kQueueOwnerMismatch:
            throw server::handlers::ClientError(
                server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kQueueOwnerMismatch)}
            );
            break;
        default:
            throw server::handlers::InternalServerError();
    }
}

void HandleIssueQueryErrors(const defs::errors::ErrorCode& code) {
    switch (code) {
        case defs::errors::ErrorCode::kOk:
            break;
        case defs::errors::ErrorCode::kUserNotFound:
            throw server::handlers::ResourceNotFound(
                server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kUserNotFound)}
            );
            break;
        case defs::errors::ErrorCode::kQueueNotFound:
            throw server::handlers::ResourceNotFound(
                server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kQueueNotFound)}
            );
            break;
        case defs::errors::ErrorCode::kForbidden:
            throw server::handlers::Forbidden(
                server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kForbidden)}
            );
            break;
        case defs::errors::ErrorCode::kIssueNotFound:
            throw server::handlers::ResourceNotFound(
                server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kIssueNotFound)}
            );
            break;
        default:
            throw server::handlers::InternalServerError();
    }
}

}  // namespace utils
