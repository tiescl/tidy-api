#pragma once

#include <string_view>
#include <variant>

#include <userver/formats/json.hpp>

#include <db/pg_ctx.hpp>

#include <utils/typed_json_handler.hpp>

#include <defs/admin.hpp>

namespace handlers::admin_v1_users::get {

using Request = defs::admin::AdminV1UsersRequest;

using Response200 = defs::admin::AdminV1UsersResponse;
using Response = std::variant<Response200>;

class View final
    : public utils::TypedJsonHandler<View, Request, Response, Response200, defs::admin::Parse, defs::admin::Serialize> {
public:
    static constexpr std::string_view kName = "handler-admin_v1_users-post";

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

}  // namespace handlers::admin_v1_users::get
