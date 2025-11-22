#pragma once

#include <string_view>
#include <variant>

#include <userver/formats/json.hpp>

#include <db/pg_ctx.hpp>

#include <utils/typed_json_handler.hpp>

#include <defs/users.hpp>

namespace handlers::v1_users_me_password::put {

using Request = defs::users::V1UsersMePasswordRequest;

using Response200 = defs::users::V1UsersMePasswordResponse;
using Response = std::variant<Response200>;

class View final
    : public utils::TypedJsonHandler<View, Request, Response, Response200, defs::users::Parse, defs::users::Serialize> {
public:
    static constexpr std::string_view kName = "handler-v1_users_me_password-put";

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

}  // namespace handlers::v1_users_me_password::put
