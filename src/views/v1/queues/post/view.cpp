#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/server/handlers/exceptions.hpp>
#include <userver/storages/postgres/exceptions.hpp>
#include <userver/utils/assert.hpp>

#include <db/api/queues/queries.hpp>
#include <db/dto/queues/views.hpp>

#include <utils/constants.hpp>

#include <docs/common.hpp>

namespace handlers::v1_queues::post {

Response View::Handle(
    Request&& queue,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    if (queue.owner_id != user_id) {
        throw server::handlers::ClientError(
            server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kQueueOwnerMismatch)}
        );
    }

    std::optional<defs::queues::Queue> created_queue_opt;
    try {
        created_queue_opt = db::api::queues::CreateQueue(pg_, queue);
    } catch (const storages::postgres::UniqueViolation& exc) {
        if (exc.GetConstraint() == utils::constants::kQueuesUniqueKeyConstraint) {
            throw server::handlers::ClientError(
                server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kQueueAlreadyExists)}
            );
        }
    }

    if (!created_queue_opt.has_value()) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kUserNotFound)}
        );
    }

    return Response200{std::move(created_queue_opt).value()};
}

}  // namespace handlers::v1_queues::post
