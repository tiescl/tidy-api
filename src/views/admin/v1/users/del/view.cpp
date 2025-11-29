#include "view.hpp"

#include <db/api/admin/queries.hpp>

namespace handlers::admin_v1_users::del {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    [[maybe_unused]] server::request::RequestContext& request_context
) const {
    db::api::admin::DeleteUsers(pg_, request.user_ids);

    return Response200{};
}

}  // namespace handlers::admin_v1_users::del
