#include "auth_checker.hpp"

#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <userver/http/common_headers.hpp>
#include <userver/logging/log.hpp>
#include <userver/server/auth/user_auth_info.hpp>
#include <userver/server/handlers/auth/auth_checker_base.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <userver/storages/postgres/io/chrono.hpp>

#include <db/api/auth/queries.hpp>

#include <utils/constants.hpp>

#include <defs/errors.hpp>

namespace auth {

AuthCheckerCookieRequired::AuthCheckResult AuthCheckerCookieRequired::CheckAuth(
    const server::http::HttpRequest& request,
    server::request::RequestContext& request_context
) const {
    const server::auth::UserAuthInfo::Ticket& token{request.GetCookie(utils::constants::kUserTokenCookieName)};
    if (token.empty()) {
        return AuthCheckResult{
            AuthCheckResult::Status::kTokenNotFound,
            {},
            ToString(defs::errors::ErrorCode::kTokenNotFound),
            server::handlers::HandlerErrorCode::kUnauthorized
        };
    }

    const auto cache_snapshot = tokens_cache_.Get();
    auto info = cache_snapshot->GetUserInfoByToken(token);

    if (!info) {
        if (auto db_result = db::api::auth::GetUserTokenData(pg_, token.GetUnderlying())) {
            info = std::make_shared<const db::dto::auth::UserTokenExt>(std::move(*db_result));
        }
    }

    if (!info || info->expires_at < storages::postgres::Now()) {
        return AuthCheckResult{
            AuthCheckResult::Status::kInvalidToken,
            {},
            ToString(defs::errors::ErrorCode::kInvalidToken),
            server::handlers::HandlerErrorCode::kUnauthorized
        };
    }

    if (const auto& user_role_str = ToString(info->user_role);
        !required_scopes_.empty() &&
        std::find(required_scopes_.begin(), required_scopes_.end(), user_role_str) == required_scopes_.end()) {
        return AuthCheckResult{
            AuthCheckResult::Status::kForbidden,
            {},
            ToString(defs::errors::ErrorCode::kForbidden),
            server::handlers::HandlerErrorCode::kForbidden
        };
    }

    request_context.SetData(utils::constants::kUserId, info->user_id);
    return {};
}

AuthCheckerCookieOptional::AuthCheckResult AuthCheckerCookieOptional::CheckAuth(
    const server::http::HttpRequest& request,
    server::request::RequestContext& request_context
) const {
    const server::auth::UserAuthInfo::Ticket& token{request.GetCookie(utils::constants::kUserTokenCookieName)};
    if (token.empty()) {
        return {};
    }

    const auto cache_snapshot = tokens_cache_.Get();
    const auto info = cache_snapshot->GetUserInfoByToken(token);
    if (!info) {
        LOG_WARNING("invalid token sent to cookie-optional endpoint");
        return {};
    }

    const auto& user_role_str = ToString(info->user_role);
    if (!required_scopes_.empty() &&
        std::find(required_scopes_.begin(), required_scopes_.end(), user_role_str) == required_scopes_.end()) {
        return {};
    }

    request_context.SetData(utils::constants::kUserId, info->user_id);
    return {};
}

CheckerFactoryCookieRequired::CheckerFactoryCookieRequired(const components::ComponentContext& context)
    : tokens_cache_(context.FindComponent<caches::UserTokensCache>()), pg_{context} {}

server::handlers::auth::AuthCheckerBasePtr CheckerFactoryCookieRequired::MakeAuthChecker(
    const server::handlers::auth::HandlerAuthConfig& auth_config
) const {
    auto scopes = auth_config["scopes"].As<server::auth::UserScopes>({});
    return std::make_shared<AuthCheckerCookieRequired>(pg_, tokens_cache_, std::move(scopes));
}

CheckerFactoryCookieOptional::CheckerFactoryCookieOptional(const components::ComponentContext& context)
    : tokens_cache_(context.FindComponent<caches::UserTokensCache>()) {}

server::handlers::auth::AuthCheckerBasePtr CheckerFactoryCookieOptional::MakeAuthChecker(
    const server::handlers::auth::HandlerAuthConfig& auth_config
) const {
    auto scopes = auth_config["scopes"].As<server::auth::UserScopes>({});
    return std::make_shared<AuthCheckerCookieOptional>(tokens_cache_, std::move(scopes));
}

}  // namespace auth
