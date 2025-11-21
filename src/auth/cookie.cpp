#include "cookie.hpp"

#include <utils/constants.hpp>

namespace auth {

server::http::Cookie
CreateSecureCookie(const std::string& token, const std::chrono::system_clock::time_point token_expires_at) {
    server::http::Cookie cookie(utils::constants::kUserTokenCookieName, token);
    cookie.SetSecure();
    cookie.SetHttpOnly();
    cookie.SetExpires(token_expires_at);
    cookie.SetPath(utils::constants::kCookiePath);
    cookie.SetSameSite(utils::constants::kSameSiteLax);
    cookie.SetDomain(utils::constants::kServerDomainName);

    return cookie;
}

}  // namespace auth
