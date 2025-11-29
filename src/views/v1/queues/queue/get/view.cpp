#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/server/handlers/exceptions.hpp>
#include <userver/utils/boost_uuid4.hpp>

#include <db/api/queues/queries.hpp>

#include <utils/constants.hpp>

#include <defs/errors.hpp>

namespace handlers::v1_queues_queue::get {

Response View::Handle(
    [[maybe_unused]] Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    boost::uuids::uuid queue_id;
    try {
        queue_id = utils::BoostUuidFromString(http_request.GetPathArg(utils::constants::kQueue));
    } catch (const std::exception& exc) {
        throw server::handlers::ClientError(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kInvalidQueueId)}
        );
    }

    auto queue = db::api::queues::GetQueue(pg_, user_id, queue_id);
    if (!queue.has_value()) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kQueueNotFound)}
        );
    }

    return Response200{std::move(queue).value()};
}

}  // namespace handlers::v1_queues_queue::get
