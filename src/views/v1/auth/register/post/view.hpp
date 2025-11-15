#pragma once

#include <db/pg_ctx.hpp>

#include <utils/typed_json_handler.hpp>

#include <defs/auth.hpp>

namespace handlers::v1_auth_register::post {

using Request = defs::auth::V1AuthRegisterRequest;
using Response = defs::auth::V1AuthRegisterResponse;

class View final : public utils::TypedJsonHandler<View, Request, Response, defs::auth::Parse, defs::auth::Serialize> {
public:
    static constexpr std::string_view kName = "handler-v1_auth_register-post";

    View(const components::ComponentConfig& config, const components::ComponentContext& context)
        : TypedJsonHandler(config, context), pg_(context) {}

    Response Handle(
        Request&& request,
        const server::http::HttpRequest& http_request,
        server::request::RequestContext& request_context
    ) const;

private:
    const db::PgCtx pg_;
};

}  // namespace handlers::v1_auth_register::post
