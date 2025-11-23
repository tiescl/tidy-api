#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/server/handlers/exceptions.hpp>
#include <userver/utils/boost_uuid4.hpp>

#include <db/api/queues/queries.hpp>

#include <utils/constants.hpp>

#include <defs/errors.hpp>
#include <defs/queues.hpp>

namespace handlers::v1_queues_permissions::post {

namespace {

void HandleQueryErrors(const std::string& result) {
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

}  // namespace

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto owner_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    boost::uuids::uuid queue_id;
    try {
        queue_id = utils::BoostUuidFromString(http_request.GetPathArg(utils::constants::kQueueId));
    } catch (const std::exception& exc) {
        throw server::handlers::ClientError(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kInvalidQueueId)}
        );
    }

    std::string result;
    switch (request.permission_type) {
        case defs::queues::PermissionType::kRole:
            if (!request.role.has_value()) {
                throw server::handlers::ClientError(
                    server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kMissingRolePermission)}
                );
            }

            result = db::api::queues::CreateQueueRolePermission(
                pg_, queue_id, request.role.value(), request.actions, owner_id
            );

            break;
        case defs::queues::PermissionType::kUser:
            if (!request.user_id.has_value()) {
                throw server::handlers::ClientError(
                    server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kMissingUserPermission)}
                );
            }

            result = db::api::queues::CreateQueueUserPermission(
                pg_, queue_id, request.user_id.value(), request.actions, owner_id
            );

            break;
    };

    HandleQueryErrors(result);

    return Response200{};
}

}  // namespace handlers::v1_queues_permissions::post
