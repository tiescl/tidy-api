#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/server/handlers/exceptions.hpp>

#include <db/api/users/queries.hpp>

#include <utils/constants.hpp>

#include <defs/error.hpp>

namespace handlers::v1_users_me::put {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    std::optional<Response200> updated_data;
    try {
        updated_data = db::api::users::UpdateUserData(pg_, user_id, request.full_name, request.email, request.username);
    } catch (const storages::postgres::UniqueViolation& exc) {
        if (exc.GetConstraint() == utils::constants::kUsersUniqueEmailConstraint) {
            throw server::handlers::ClientError(
                server::handlers::ExternalBody{ToString(defs::error::ErrorCode::kEmailAlreadyExists)}
            );
        }
        if (exc.GetConstraint() == utils::constants::kUsersUniqueUsernameConstraint) {
            throw server::handlers::ClientError(
                server::handlers::ExternalBody{ToString(defs::error::ErrorCode::kUsernameAlreadyExists)}
            );
        }
    }

    if (!updated_data.has_value()) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::error::ErrorCode::kUserNotFound)}
        );
    }

    return Response200{std::move(updated_data).value()};
}

}  // namespace handlers::v1_users_me::put
