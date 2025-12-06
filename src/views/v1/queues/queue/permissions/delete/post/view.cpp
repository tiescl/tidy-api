#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/server/handlers/exceptions.hpp>

#include <db/api/queues/queries.hpp>

#include <utils/constants.hpp>
#include <utils/errors.hpp>
#include <utils/path_args.hpp>

#include <docs/common.hpp>
#include <docs/queues.hpp>

namespace handlers::v1_queues_queue_permissions_delete::post {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto owner_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);
    const auto queue_id = utils::GetQueuePathArg(http_request);

    defs::common::ErrorCode result;
    switch (request.permission_type) {
        case defs::queues::PermissionType::kRole:
            if (!request.role.has_value()) {
                throw server::handlers::ClientError(
                    server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kMissingRolePermission)}
                );
            }

            result = db::api::queues::DeleteQueueRolePermission(pg_, queue_id, request.role.value(), owner_id);

            break;
        case defs::queues::PermissionType::kUser:
            if (!request.user_id.has_value()) {
                throw server::handlers::ClientError(
                    server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kMissingUserPermission)}
                );
            }

            result = db::api::queues::DeleteQueueUserPermission(pg_, queue_id, request.user_id.value(), owner_id);

            break;
    };

    utils::HandleQueueQueryErrors(result);

    return Response200{};
}

}  // namespace handlers::v1_queues_queue_permissions_delete::post
