#include "view.hpp"

#include <userver/server/handlers/exceptions.hpp>

#include <db/api/admin/queries.hpp>

#include <defs/errors.hpp>

namespace handlers::admin_v1_queues::del {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    [[maybe_unused]] server::request::RequestContext& request_context
) const {
    if (!db::api::admin::DeleteQueue(pg_, request.queue_id)) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kQueueNotFound)}
        );
    }

    return Response200{};
}

}  // namespace handlers::admin_v1_queues::del
