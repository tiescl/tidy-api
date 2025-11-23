#include "view.hpp"

#include <userver/server/handlers/exceptions.hpp>

#include <db/api/users/queries.hpp>

#include <utils/constants.hpp>

#include <defs/error.hpp>

namespace handlers::v1_users_me::del {

Response View::Handle(
    [[maybe_unused]] Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    if (!db::api::users::DeleteUser(pg_, user_id)) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::error::ErrorCode::kUserNotFound)}
        );
    }

    return Response200{};
}

}  // namespace handlers::v1_users_me::del
