#pragma once

#include <string_view>
#include <variant>

#include <userver/formats/json.hpp>

#include <db/pg_ctx.hpp>

#include <utils/typed_json_handler.hpp>

#include <defs/queues.hpp>

namespace handlers::v1_queues_single::get {

using namespace defs::queues;

using Request = V1QueuesSingleRequest;
using Response200 = V1QueuesSingleResponse;
using Response = std::variant<Response200>;

class View final : public utils::TypedJsonHandler<View, Request, Response, Response200, Parse, Serialize> {
public:
    static constexpr std::string_view kName = "handler-v1_queues_single-get";

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

}  // namespace handlers::v1_queues_single::get
