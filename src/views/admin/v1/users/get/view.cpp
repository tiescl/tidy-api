#include "view.hpp"

#include <algorithm>
#include <format>
#include <string>

#include <db/api/admin/queries.hpp>

namespace handlers::admin_v1_users::get {

namespace {

static constexpr long long kMaxLimit = 100;
static constexpr std::string_view kPage = "page";
static constexpr std::string_view kLimit = "limit";
static constexpr std::string_view kSearch = "search";

std::string WrapWithWildcards(const std::string& search_term) { return std::format("%{}%", search_term); }

}  // namespace

Response View::Handle(
    [[maybe_unused]] Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    [[maybe_unused]] server::request::RequestContext& request_context
) const {
    const std::string search = WrapWithWildcards(http_request.GetArg(kSearch));
    const int64_t page = std::max(std::atoll(http_request.GetArg(kPage).c_str()), 1LL);
    const int64_t limit = std::min(std::atoll(http_request.GetArg(kLimit).c_str()), kMaxLimit);

    return Response200{db::api::admin::GetUsers(pg_, search, limit, (page - 1) * limit)};
}

}  // namespace handlers::admin_v1_users::get
