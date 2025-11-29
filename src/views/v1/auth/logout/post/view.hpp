#pragma once

#include <string_view>
#include <variant>

#include <caches/user_tokens.hpp>
#include <db/pg_ctx.hpp>

#include <utils/typed_json_handler.hpp>

#include <defs/auth.hpp>

namespace handlers::v1_auth_logout::post {

using namespace defs::auth;

using Request = V1AuthLogoutRequest;
using Response200 = V1AuthLogoutResponse;
using Response = std::variant<Response200>;

class View final : public utils::TypedJsonHandler<View, Request, Response, Response200, Parse, Serialize> {
public:
    static constexpr std::string_view kName = "handler-v1_auth_logout-post";

    View(const components::ComponentConfig& config, const components::ComponentContext& context)
        : TypedJsonHandler(config, context),
          pg_(context),
          tokens_cache_(context.FindComponent<caches::UserTokensCache>()) {}

    Response Handle(
        Request&& request,
        const server::http::HttpRequest& http_request,
        server::request::RequestContext& request_context
    ) const;

private:
    const db::PgCtx pg_;
    caches::UserTokensCache& tokens_cache_;
};

}  // namespace handlers::v1_auth_logout::post
