#pragma once

#include <string_view>

#include <userver/http/predefined_header.hpp>

namespace utils::constants {

static constexpr const char* kCode{"code"};

static constexpr const char* kIssue{"issue"};
static constexpr const char* kUserId{"user_id"};
static constexpr const char* kIssueId{"issue_id"};
static constexpr std::string_view kUser{"user"};
static constexpr std::string_view kQueue{"queue"};

static constexpr std::string_view kQueuesUniqueKeyConstraint{"queues_unique_key"};
static constexpr std::string_view kUsersUniqueEmailConstraint{"users_unique_email"};
static constexpr std::string_view kUsersUniqueUsernameConstraint{"users_unique_username"};

static constexpr int64_t kMaxLimit = 100;
static constexpr int64_t kDefaultLimit = 50;
static constexpr std::string_view kPage = "page";
static constexpr std::string_view kLimit = "limit";
static constexpr std::string_view kSearch = "search";

static constexpr const char* kCookiePath = "/";
static constexpr const char* kSameSiteLax = "Lax";
static constexpr const char* kLandingPageUrl = "/";
static constexpr const char* kServerDomainName = "api.tidyapp.uz";
static constexpr const char* kUserTokenCookieName = "session_token";
static constexpr http::headers::PredefinedHeader kLocationHeader{"Location"};

}  // namespace utils::constants
