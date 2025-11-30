#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <db/api/issues/queries.hpp>

#include <utils/constants.hpp>
#include <utils/errors.hpp>
#include <utils/path_args.hpp>

namespace handlers::v1_queues_queue_issues_issue_delete::post {

Response View::Handle(
    [[maybe_unused]] Request&& request,
    const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto queue_id = utils::GetQueuePathArg(http_request);
    const auto issue_id = utils::GetIssuePathArg(http_request);
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    utils::HandleIssueQueryErrors(db::api::issues::DeleteIssue(pg_, user_id, queue_id, issue_id));

    return Response200{};
}

}  // namespace handlers::v1_queues_queue_issues_issue_delete::post
