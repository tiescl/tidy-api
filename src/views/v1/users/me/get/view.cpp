#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/server/handlers/exceptions.hpp>
#include <userver/utils/assert.hpp>

#include <db/api/users/queries.hpp>

#include <utils/constants.hpp>

#include <defs/error.hpp>

namespace handlers::v1_users_me::get {

Response View::Handle(
    [[maybe_unused]] Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    auto user = db::api::users::GetCurrentUser(pg_, user_id);
    if (!user.has_value()) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::error::ErrorCode::kUserNotFound)}
        );
    }

    return Response200{std::move(user).value()};
}

}  // namespace handlers::v1_users_me::get
