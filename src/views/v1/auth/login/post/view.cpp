#include "view.hpp"

#include <string>

#include <boost/uuid/uuid.hpp>

#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/utils/datetime.hpp>
#include <userver/utils/uuid4.hpp>

#include <db/api/auth/queries.hpp>
#include <db/dto/auth/views.hpp>

#include <auth/cookie.hpp>
#include <auth/password_hasher.hpp>
#include <utils/constants.hpp>

#include <defs/errors.hpp>
#include <defs/users.hpp>

namespace handlers::v1_auth_login::post {

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

    const auto stored_user_data = db::api::auth::GetUserAuthData(pg_, request.email);
    if (!stored_user_data.has_value()) {
        throw server::handlers::Unauthorized(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kEmailNotFound)}
        );
    }

    if (!auth::PasswordHasher::Verify(request.password, stored_user_data->password_hash)) {
        throw server::handlers::Unauthorized(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kInvalidPassword)}
        );
    }

    std::string token = utils::generators::GenerateUuid();
    const auto now = utils::datetime::Now();
    const auto token_expires_at = now + std::chrono::days(1);

    db::api::auth::SaveUserToken(
        pg_,
        db::dto::auth::UserToken{
            .token{token},
            .user_id = stored_user_data.value().user_id,
            .user_role = stored_user_data.value().role,
            .expires_at{token_expires_at}
        }
    );

    response.SetCookie(auth::CreateSecureCookie(std::move(token), token_expires_at));

    return Response200{};
}

}  // namespace handlers::v1_auth_login::post
