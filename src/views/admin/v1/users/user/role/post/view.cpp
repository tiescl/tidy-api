#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/server/handlers/exceptions.hpp>

#include <db/api/admin/queries.hpp>

#include <utils/constants.hpp>
#include <utils/path_args.hpp>

#include <docs/common.hpp>

namespace handlers::admin_v1_users_user_role::post {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    [[maybe_unused]] server::request::RequestContext& request_context
) const {
    if (!db::api::admin::UpdateUserRole(pg_, utils::GetUserPathArg(http_request), request.role)) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kUserNotFound)}
        );
    }

    return Response200{};
}

}  // namespace handlers::admin_v1_users_user_role::post
