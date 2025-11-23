#include "view.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>

#include <boost/uuid/uuid.hpp>

#include <userver/logging/log.hpp>
#include <userver/server/handlers/exceptions.hpp>

#include <db/api/queues/queries.hpp>

#include <utils/constants.hpp>
#include <utils/text.hpp>

#include <defs/errors.hpp>

namespace handlers::v1_queues::get {

Response View::Handle(
    [[maybe_unused]] Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const try {
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    const std::string search = utils::WrapWithWildcards(http_request.GetArg(utils::constants::kSearch));
    if (search.length() > 256) {
        throw std::out_of_range("search term is too long");
    }

    const std::string page_str = http_request.GetArg(utils::constants::kPage);
    const int64_t page = std::max(page_str.empty() ? 1LL : std::stoll(page_str), 1LL);

    const std::string limit_str = http_request.GetArg(utils::constants::kLimit);
    int64_t limit = limit_str.empty() ? utils::constants::kDefaultLimit : std::stoll(limit_str);
    limit = std::min(limit, utils::constants::kMaxLimit);

    return Response200{db::api::queues::GetAvailableQueues(pg_, user_id, search, limit, (page - 1) * limit)};
} catch (const std::out_of_range& exc) {
    LOG_ERROR("invalid query params received from client: {}", exc.what());
    throw server::handlers::ClientError(
        server::handlers::ExternalBody{ToString(defs::errors::ErrorCode::kInvalidQueryParams)}
    );
}

}  // namespace handlers::v1_queues::get
