#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/server/handlers/exceptions.hpp>

#include <db/api/queues/queries.hpp>

#include <utils/constants.hpp>
#include <utils/path_args.hpp>

#include <docs/common.hpp>

namespace handlers::v1_queues_queue::get {

Response View::Handle(
    [[maybe_unused]] Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    auto queue = db::api::queues::GetQueue(pg_, user_id, utils::GetQueuePathArg(http_request));
    if (!queue.has_value()) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kQueueNotFound)}
        );
    }

    return Response200{std::move(queue).value()};
}

}  // namespace handlers::v1_queues_queue::get
