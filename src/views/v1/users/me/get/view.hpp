#pragma once

#include <string_view>
#include <variant>

#include <db/pg_ctx.hpp>

#include <utils/typed_json_handler.hpp>

#include <docs/users.hpp>

namespace handlers::v1_users_me::get {

using namespace defs::users;

using Request = V1UsersMeRequest;
using Response200 = V1UsersMeResponse;
using Response = std::variant<Response200>;

class View final : public utils::TypedJsonHandler<View, Request, Response, Response200, Parse, Serialize> {
public:
    static constexpr std::string_view kName = "handler-v1_users_me-get";

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

}  // namespace handlers::v1_users_me::get
