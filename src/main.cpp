#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
#include <userver/components/component_list.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/congestion_control/component.hpp>
#include <userver/server/handlers/auth/auth_checker_factory.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include <auth/auth_checker.hpp>
#include <caches/user_tokens.hpp>

#include <views/admin/v1/queues/del/view.hpp>
#include <views/admin/v1/users/del/view.hpp>
#include <views/admin/v1/users/get/view.hpp>
#include <views/admin/v1/users/role/post/view.hpp>
#include <views/v1/auth/login/post/view.hpp>
#include <views/v1/auth/logout/post/view.hpp>
#include <views/v1/auth/register/post/view.hpp>
#include <views/v1/queues/del/view.hpp>
#include <views/v1/queues/get/view.hpp>
#include <views/v1/queues/permissions/del/view.hpp>
#include <views/v1/queues/permissions/post/view.hpp>
#include <views/v1/queues/post/view.hpp>
#include <views/v1/users/me/del/view.hpp>
#include <views/v1/users/me/get/view.hpp>
#include <views/v1/users/me/password/put/view.hpp>
#include <views/v1/users/me/put/view.hpp>

namespace {

components::ComponentList GenerateServiceHandlersList() {
    return components::ComponentList()
        .Append<handlers::admin_v1_queues::del::View>()
        .Append<handlers::admin_v1_users::del::View>()
        .Append<handlers::admin_v1_users::get::View>()
        .Append<handlers::admin_v1_users_role::post::View>()
        .Append<handlers::v1_auth_login::post::View>()
        .Append<handlers::v1_auth_logout::post::View>()
        .Append<handlers::v1_auth_register::post::View>()
        .Append<handlers::v1_queues::del::View>()
        .Append<handlers::v1_queues::get::View>()
        .Append<handlers::v1_queues_permissions::del::View>()
        .Append<handlers::v1_queues_permissions::post::View>()
        .Append<handlers::v1_queues::post::View>()
        .Append<handlers::v1_users_me::del::View>()
        .Append<handlers::v1_users_me::get::View>()
        .Append<handlers::v1_users_me_password::put::View>()
        .Append<handlers::v1_users_me::put::View>();
}

}  // namespace

int main(int argc, char* argv[]) {
    server::handlers::auth::RegisterAuthCheckerFactory<auth::CheckerFactoryCookieRequired>();
    server::handlers::auth::RegisterAuthCheckerFactory<auth::CheckerFactoryCookieOptional>();

    auto component_list = components::MinimalServerComponentList()
                              .Append<server::handlers::Ping>()
                              .Append<components::HttpClientCore>()
                              .Append<components::HttpClient>()
                              .Append<clients::dns::Component>()
                              .Append<components::TestsuiteSupport>()
                              .Append<congestion_control::Component>()
                              .Append<server::handlers::TestsControl>()
                              .Append<components::Postgres>("tidy_pg")
                              .Append<caches::UserTokensCache>()
                              .AppendComponentList(GenerateServiceHandlersList());

    return utils::DaemonMain(argc, argv, component_list);
}
