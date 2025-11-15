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

static constexpr int kSaltLength{16};
static constexpr int kHashLength{32};
static constexpr int kIterations{100000};

Handler::Handler(const components::ComponentConfig& config, const components::ComponentContext& context)
    : HttpHandlerBase(config, context), pg_(context) {}

std::string Handler::HandleRequest(server::http::HttpRequest& request, server::request::RequestContext&) const {
    unsigned char salt[kSaltLength];
    RAND_bytes(salt, kSaltLength);

    unsigned char hash[kHashLength];
    const auto& name = request.GetArg("name");
    PKCS5_PBKDF2_HMAC(name.c_str(), name.length(), salt, kSaltLength, kIterations, EVP_sha256(), kHashLength, hash);

    std::string result;
    result.reserve(kSaltLength + 1 + kHashLength);
    for (int i = 0; i < kSaltLength; i++) {
        result += std::format("{:02x}", salt[i]);
    }
    result += "$";
    for (int i = 0; i < kHashLength; i++) {
        result += std::format("{:02x}", hash[i]);
    }

    LOG_INFO("tiescl(pswd_hash): {}", result);

    return utils::SayHelloTo(name, utils::UserType::kFirstTime, db::api::SelectNow(pg_));
}

}  // namespace handlers::hello::get
