#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/utils/assert.hpp>

#include <db/api/issues/queries.hpp>

#include <utils/constants.hpp>
#include <utils/errors.hpp>
#include <utils/path_args.hpp>

#include <defs/users.hpp>

namespace handlers::v1_queues_queue_issues_issue::get {

Response View::Handle(
    [[maybe_unused]] Request&& request,
    const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto queue_id = utils::GetQueuePathArg(http_request);
    const auto issue_id = utils::GetIssuePathArg(http_request);
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    auto result = db::api::issues::GetIssue(pg_, user_id, queue_id, issue_id);

    utils::HandleIssueQueryErrors(result.code);
    UINVARIANT(result.issue.has_value(), "issue must have a value at this point");

    return Response200{std::move(result.issue).value()};
}

}  // namespace handlers::v1_queues_queue_issues_issue::get
