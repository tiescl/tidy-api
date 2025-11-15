#include "view.hpp"

#include <userver/logging/log.hpp>
#include <userver/utils/boost_uuid4.hpp>

#include <db/api/auth/queries.hpp>
#include <db/dto/auth/views.hpp>

#include <utils/constants.hpp>
#include <utils/password_hasher.hpp>

#include <defs/error.hpp>

namespace handlers::v1_auth_register::post {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    [[maybe_unused]] server::request::RequestContext& request_context
) const {
    std::optional<boost::uuids::uuid> created_user_id;
    try {
        created_user_id = db::api::auth::RegisterUser(
            pg_,
            db::dto::auth::User{
                .username = request.username,
                .email = request.email,
                .password_hash = utils::PasswordHasher::Hash(request.password),
                .role = defs::auth::UserRole::kPending,
            }
        );
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

    if (!created_user_id.has_value()) {
        throw server::handlers::InternalServerError();
    }

    return Response{created_user_id.value()};
}

}  // namespace handlers::v1_auth_register::post
