#include "view.hpp"

#include <db/api/admin/queries.hpp>

namespace handlers::admin_v1_queues_delete::post {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    [[maybe_unused]] server::request::RequestContext& request_context
) const {
    db::api::admin::DeleteQueues(pg_, request.queue_ids);

    return Response200{};
}

}  // namespace handlers::admin_v1_queues_delete::post
