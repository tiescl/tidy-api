#pragma once

#include <chrono>
#include <string>

#include <userver/server/http/http_response_cookie.hpp>

namespace auth {

server::http::Cookie
CreateSecureCookie(const std::string& token, const std::chrono::system_clock::time_point token_expires_at);

}  // namespace auth
