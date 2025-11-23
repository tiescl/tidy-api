#include "errors.hpp"

#include <userver/server/handlers/exceptions.hpp>

#include <defs/errors.hpp>

namespace utils {

void HandleQueueQueryErrors(const std::string& result) {
    if (result == ToString(defs::errors::ErrorCode::kQueueNotFound)) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kQueueNotFound)}
        );
    }
    if (result == ToString(defs::errors::ErrorCode::kQueueOwnerMismatch)) {
        throw server::handlers::ClientError(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kQueueOwnerMismatch)}
        );
    }
}

}  // namespace utils
