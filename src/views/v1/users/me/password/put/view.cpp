#include "view.hpp"

#include <boost/uuid/uuid.hpp>

#include <userver/server/handlers/exceptions.hpp>
#include <userver/utils/assert.hpp>

#include <db/api/users/queries.hpp>

#include <auth/password_hasher.hpp>
#include <utils/constants.hpp>

#include <docs/common.hpp>

namespace handlers::v1_users_me_password::put {

Response View::Handle(
    Request&& request,
    [[maybe_unused]] const server::http::HttpRequest& http_request,
    server::request::RequestContext& request_context
) const {
    const auto user_id = request_context.GetData<boost::uuids::uuid>(utils::constants::kUserId);

    const auto stored_hash = db::api::users::GetUserPasswordHash(pg_, user_id);
    UINVARIANT(stored_hash, "missing password_hash while user_id is valid");

    if (!auth::PasswordHasher::Verify(request.old_password, stored_hash.value())) {
        throw server::handlers::ClientError(
            server::handlers::ExternalBody{ToString(defs::common::ErrorCode::kInvalidPassword)}
        );
    }

    db::api::users::SetNewUserPasswordHash(pg_, user_id, auth::PasswordHasher::Hash(request.new_password));

    return Response200{};
}

}  // namespace handlers::v1_users_me_password::put
