#include "view.hpp"

#include <userver/server/handlers/exceptions.hpp>

#include <db/api/queues/queries.hpp>

#include <utils/constants.hpp>

#include <defs/errors.hpp>

namespace handlers::v1_queues::del {

namespace {

void HandleQueryErrors(const std::string& result) {
    if (result == ToString(defs::errors::ErrorCode::kQueueNotFound)) {
        throw server::handlers::ResourceNotFound(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kQueueNotFound)}
        );
    }
    if (result == ToString(defs::errors::ErrorCode::kQueueOwnerMismatch)) {
        throw server::handlers::ClientError(
            server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kQueueOwnerMismatch)}
        );
    }
}

}  // namespace

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    [[maybe_unused]] server::request::RequestContext& request_context
) const {
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);
    const auto result = db::api::queues::DeleteQueue(pg_, request.queue_id, user_id);

    HandleQueryErrors(result);

    return Response200{};
}

}  // namespace handlers::v1_queues::del
