#include "view.hpp"

#include <utils/greeting.hpp>

namespace handlers::hello::get {

std::string Handler::HandleRequest(
    userver::server::http::HttpRequest& request,
    [[maybe_unused]] userver::server::request::RequestContext& context
) const {
    return utils::SayHelloTo(request.GetArg("name"), utils::UserType::kFirstTime);
}

}  // namespace handlers::hello::get
