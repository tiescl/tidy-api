#include "view.hpp"

#include <string>

#include <openssl/evp.h>
#include <openssl/rand.h>

#include <userver/formats/json.hpp>
#include <userver/utils/log.hpp>

#include <db/api/time/queries.hpp>

#include <utils/greeting.hpp>

#include <defs/admin_api.hpp>

namespace handlers::hello::get {

Handler::Handler(const components::ComponentConfig& config, const components::ComponentContext& context)
    : HttpHandlerBase(config, context), pg_(context) {}

std::string Handler::HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const {
    unsigned char salt[16];
    RAND_bytes(salt, 16);
    std::string salt_str(reinterpret_cast<const char*>(salt), 16);
    LOG_INFO("tiescl(salt): {}", salt_str);

    return utils::SayHelloTo(request.GetArg("name"), utils::UserType::kFirstTime, db::api::SelectNow(pg_));
}

}  // namespace handlers::hello::get
