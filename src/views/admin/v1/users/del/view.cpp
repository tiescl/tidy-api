#include "view.hpp"

#include <userver/server/handlers/exceptions.hpp>

#include <db/api/admin/queries.hpp>

#include <defs/error.hpp>

namespace handlers::admin_v1_users::del {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    [[maybe_unused]] server::request::RequestContext& request_context
) const {
    if (!db::api::admin::DeleteUser(pg_, request.user_id)) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::error::ErrorCode::kUserNotFound)}
        );
    }

    return Response200{};
}

}  // namespace handlers::admin_v1_users::del
