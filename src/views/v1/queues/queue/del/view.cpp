#include "view.hpp"

#include <userver/server/handlers/exceptions.hpp>

#include <db/api/queues/queries.hpp>

#include <utils/constants.hpp>
#include <utils/errors.hpp>
#include <utils/path_args.hpp>

#include <defs/errors.hpp>

namespace handlers::v1_queues_queue::del {

Response View::Handle(
    [[maybe_unused]] Request&& request,
    const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    const auto result = db::api::queues::DeleteQueue(pg_, utils::GetQueuePathArg(http_request), user_id);

    utils::HandleQueueQueryErrors(result);

    return Response200{};
}

}  // namespace handlers::v1_queues_queue::del
