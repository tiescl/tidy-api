#pragma once

#include <string>
#include <string_view>

#include <userver/server/handlers/http_handler_base.hpp>

#include <caches/user_tokens.hpp>

#include <db/pg_ctx.hpp>

namespace handlers::v1_auth_logout::post {

using Response = std::string;

class View final : public server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-v1_auth_logout-post";

    View(const components::ComponentConfig& config, const components::ComponentContext& context)
        : HttpHandlerBase(config, context),
          pg_(context),
          tokens_cache_(context.FindComponent<caches::UserTokensCache>()) {}

    Response HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const override;

private:
    const db::PgCtx pg_;
    caches::UserTokensCache& tokens_cache_;
};

}  // namespace handlers::v1_auth_logout::post
