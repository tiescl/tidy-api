#include "view.hpp"

#include <chrono>

#include <userver/server/auth/user_auth_info.hpp>
#include <userver/storages/postgres/io/chrono.hpp>
#include <userver/utils/datetime.hpp>

#include <auth/cookie.hpp>
#include <db/api/auth/queries.hpp>

#include <utils/constants.hpp>

namespace handlers::v1_auth_logout::post {

std::string View::HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const {
    server::http::HttpResponse& response = request.GetHttpResponse();

    const auto token = request.GetCookie(utils::constants::kUserTokenCookieName);
    const auto expire_time = utils::datetime::Now() - std::chrono::seconds(1);
    db::api::auth::MarkTokenAsExpired(pg_, token, storages::postgres::TimePointTz{expire_time});

    tokens_cache_.InvalidateAsync(cache::UpdateType::kIncremental);

    response.SetCookie(auth::CreateSecureCookie(token, expire_time));

    return {};
}

}  // namespace handlers::v1_auth_logout::post
