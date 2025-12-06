#pragma once

#include <string_view>
#include <variant>

#include <db/pg_ctx.hpp>

#include <utils/typed_json_handler.hpp>

#include <docs/admin.hpp>

namespace handlers::v1_users_me_delete::post {

using namespace defs::users;

using Request = V1UsersMeDeleteRequest;
using Response200 = V1UsersMeDeleteResponse;
using Response = std::variant<Response200>;

class View final : public utils::TypedJsonHandler<View, Request, Response, Response200, Parse, Serialize> {
public:
    static constexpr std::string_view kName = "handler-v1_users_me_delete-post";

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

}  // namespace handlers::v1_users_me_delete::post
