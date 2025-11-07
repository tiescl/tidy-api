#include "view.hpp"

#include <db/api/time/queries.hpp>

#include <utils/greeting.hpp>

namespace handlers::hello::get {

Handler::Handler(const components::ComponentConfig& config, const components::ComponentContext& context)
    : HttpHandlerBase(config, context), pg_(context) {}

std::string Handler::HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const {
    return utils::SayHelloTo(request.GetArg("name"), utils::UserType::kFirstTime, db::api::SelectNow(pg_));
}

}  // namespace handlers::hello::get
