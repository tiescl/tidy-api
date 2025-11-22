#include "view.hpp"

#include <chrono>
#include <string>

#include <boost/uuid/uuid.hpp>

#include <userver/logging/log.hpp>
#include <userver/utils/datetime.hpp>
#include <userver/utils/uuid4.hpp>

#include <db/api/auth/queries.hpp>
#include <db/dto/auth/views.hpp>

#include <auth/cookie.hpp>
#include <auth/password_hasher.hpp>
#include <utils/constants.hpp>

#include <defs/error.hpp>
#include <defs/users.hpp>

namespace handlers::v1_auth_register::post {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    [[maybe_unused]] server::request::RequestContext& request_context
) const {
    server::http::HttpResponse& response = http_request.GetHttpResponse();
    if (request_context.GetDataOptional<boost::uuids::uuid>(utils::constants::kUserId)) {
        LOG_INFO("valid user token in context, redirecting");
        response.SetStatus(server::http::HttpStatus::kFound);
        response.SetHeader(utils::constants::kLocationHeader, utils::constants::kLandingPageUrl);

        return Response302();
    }

    std::optional<boost::uuids::uuid> created_user_id_opt;
    try {
        created_user_id_opt = db::api::auth::RegisterUser(
            pg_,
            db::dto::auth::User{
                .username = request.username,
                .email = request.email,
                .password_hash = auth::PasswordHasher::Hash(request.password),
                .role = defs::users::UserRole::kPending,
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

    if (!created_user_id_opt.has_value()) {
        throw server::handlers::InternalServerError();
    }

    std::string token = utils::generators::GenerateUuid();
    boost::uuids::uuid created_user_id = created_user_id_opt.value();
    const auto now = utils::datetime::Now();
    const auto token_expires_at = now + std::chrono::days(1);

    db::api::auth::SaveUserToken(
        pg_,
        db::dto::auth::UserToken{
            .token{token},
            .user_id = created_user_id,
            .user_role = defs::users::UserRole::kPending,
            .expires_at{token_expires_at}
        }
    );

    response.SetCookie(auth::CreateSecureCookie(std::move(token), token_expires_at));

    return Response200{std::move(created_user_id)};
}

}  // namespace handlers::v1_auth_register::post
