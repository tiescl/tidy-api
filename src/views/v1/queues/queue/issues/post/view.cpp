#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/utils/assert.hpp>

#include <db/api/issues/queries.hpp>

#include <utils/constants.hpp>
#include <utils/errors.hpp>
#include <utils/path_args.hpp>

#include <defs/users.hpp>

namespace handlers::v1_queues_queue_issues::post {

Response View::Handle(
    Request&& issue,
    const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto queue_id = utils::GetQueuePathArg(http_request);
    const auto author_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    auto result = db::api::issues::CreateIssue(pg_, queue_id, author_id, issue);

    utils::HandleIssueQueryErrors(result.code);
    UINVARIANT(result.issue_id.has_value(), "issue_id must have a value at this point");

    return Response200{std::move(result.issue_id).value()};
}

}  // namespace handlers::v1_queues_queue_issues::post
