#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/server/handlers/exceptions.hpp>
#include <userver/utils/boost_uuid4.hpp>

#include <db/api/admin/queries.hpp>

#include <utils/constants.hpp>

#include <defs/errors.hpp>

namespace handlers::admin_v1_users_user_role::post {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    [[maybe_unused]] server::request::RequestContext& request_context
) const {
    boost::uuids::uuid user_id;
    try {
        user_id = utils::BoostUuidFromString(http_request.GetPathArg(utils::constants::kUser));
    } catch (const std::exception& exc) {
        throw server::handlers::ClientError(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kInvalidUserId)}
        );
    }

    if (!db::api::admin::UpdateUserRole(pg_, user_id, request.role)) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kUserNotFound)}
        );
    }

    return Response200{};
}

}  // namespace handlers::admin_v1_users_user_role::post
