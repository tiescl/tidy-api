#pragma once

#include <string_view>

#include <userver/server/handlers/auth/auth_checker_factory.hpp>

#include <caches/user_tokens.hpp>

namespace auth {

/// --- checkers --- ///

class AuthCheckerCookieRequired final : public server::handlers::auth::AuthCheckerBase {
public:
    using AuthCheckResult = server::handlers::auth::AuthCheckResult;

    AuthCheckerCookieRequired(
        const caches::UserTokensCache& tokens_cache,
        std::vector<server::auth::UserScope> required_scopes
    )
        : tokens_cache_(tokens_cache), required_scopes_(std::move(required_scopes)) {}

    [[nodiscard]] AuthCheckResult CheckAuth(
        const server::http::HttpRequest& request,
        server::request::RequestContext& request_context
    ) const override;

    [[nodiscard]] bool SupportsUserAuth() const noexcept override { return true; }

private:
    const caches::UserTokensCache& tokens_cache_;
    const std::vector<server::auth::UserScope> required_scopes_;
};

class AuthCheckerCookieOptional final : public server::handlers::auth::AuthCheckerBase {
public:
    using AuthCheckResult = server::handlers::auth::AuthCheckResult;

    AuthCheckerCookieOptional(
        const caches::UserTokensCache& tokens_cache,
        std::vector<server::auth::UserScope> required_scopes
    )
        : tokens_cache_(tokens_cache), required_scopes_(std::move(required_scopes)) {}

    [[nodiscard]] AuthCheckResult CheckAuth(
        const server::http::HttpRequest& request,
        server::request::RequestContext& request_context
    ) const override;

    [[nodiscard]] bool SupportsUserAuth() const noexcept override { return true; }

private:
    const caches::UserTokensCache& tokens_cache_;
    const std::vector<server::auth::UserScope> required_scopes_;
};

/// --- factories --- ///

class CheckerFactoryCookieRequired final : public server::handlers::auth::AuthCheckerFactoryBase {
public:
    static constexpr const std::string_view kAuthType = "cookie-required";

    explicit CheckerFactoryCookieRequired(const components::ComponentContext& context);

    server::handlers::auth::AuthCheckerBasePtr MakeAuthChecker(
        const server::handlers::auth::HandlerAuthConfig& auth_config
    ) const override;

private:
    caches::UserTokensCache& tokens_cache_;
};

class CheckerFactoryCookieOptional final : public server::handlers::auth::AuthCheckerFactoryBase {
public:
    static constexpr const std::string_view kAuthType = "cookie-optional";

    explicit CheckerFactoryCookieOptional(const components::ComponentContext& context);

    server::handlers::auth::AuthCheckerBasePtr MakeAuthChecker(
        const server::handlers::auth::HandlerAuthConfig& auth_config
    ) const override;

private:
    caches::UserTokensCache& tokens_cache_;
};

}  // namespace auth
