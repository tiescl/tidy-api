#pragma once

#include <string_view>
#include <variant>

#include <db/pg_ctx.hpp>

#include <utils/typed_json_handler.hpp>

#include <docs/issues.hpp>
#include <docs/queues.hpp>

namespace handlers::v1_queues_queue_issues::post {

using Request = defs::queues::V1QueuesQueueIssuesPostRequest;
using Response200 = defs::queues::V1QueuesQueueIssuesPostResponse;
using Response = std::variant<Response200>;

class View final
    : public utils::
          TypedJsonHandler<View, Request, Response, Response200, defs::queues::Parse, defs::issues::Serialize> {
public:
    static constexpr std::string_view kName = "handler-v1_queues_queue_issues-post";

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

}  // namespace handlers::v1_queues_queue_issues::post
