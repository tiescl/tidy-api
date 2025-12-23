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

#include <views/admin/v1/issues/delete/post/view.hpp>
#include <views/admin/v1/issues/get/view.hpp>
#include <views/admin/v1/queues/delete/post/view.hpp>
#include <views/admin/v1/queues/get/view.hpp>
#include <views/admin/v1/users/delete/post/view.hpp>
#include <views/admin/v1/users/get/view.hpp>
#include <views/admin/v1/users/user/role/post/view.hpp>
#include <views/v1/auth/login/post/view.hpp>
#include <views/v1/auth/logout/post/view.hpp>
#include <views/v1/auth/register/post/view.hpp>
#include <views/v1/queues/get/view.hpp>
#include <views/v1/queues/post/view.hpp>
#include <views/v1/queues/queue/delete/post/view.hpp>
#include <views/v1/queues/queue/issues/get/view.hpp>
#include <views/v1/queues/queue/issues/issue/delete/post/view.hpp>
#include <views/v1/queues/queue/issues/issue/get/view.hpp>
#include <views/v1/queues/queue/issues/issue/put/view.hpp>
#include <views/v1/queues/queue/issues/post/view.hpp>
#include <views/v1/queues/queue/permissions/delete/post/view.hpp>
#include <views/v1/queues/queue/permissions/post/view.hpp>
#include <views/v1/users/me/delete/post/view.hpp>
#include <views/v1/users/me/get/view.hpp>
#include <views/v1/users/me/password/put/view.hpp>
#include <views/v1/users/me/put/view.hpp>

namespace {

components::ComponentList GenerateServiceHandlersList() {
    return components::ComponentList()
        .Append<handlers::admin_v1_issues_delete::post::View>()
        .Append<handlers::admin_v1_issues::get::View>()
        .Append<handlers::admin_v1_queues_delete::post::View>()
        .Append<handlers::admin_v1_queues::get::View>()
        .Append<handlers::admin_v1_users_delete::post::View>()
        .Append<handlers::admin_v1_users::get::View>()
        .Append<handlers::admin_v1_users_user_role::post::View>()
        .Append<handlers::v1_auth_login::post::View>()
        .Append<handlers::v1_auth_logout::post::View>()
        .Append<handlers::v1_auth_register::post::View>()
        .Append<handlers::v1_queues::get::View>()
        .Append<handlers::v1_queues::post::View>()
        .Append<handlers::v1_queues_queue_delete::post::View>()
        .Append<handlers::v1_queues_queue_issues::get::View>()
        .Append<handlers::v1_queues_queue_issues_issue_delete::post::View>()
        .Append<handlers::v1_queues_queue_issues_issue::get::View>()
        .Append<handlers::v1_queues_queue_issues_issue::put::View>()
        .Append<handlers::v1_queues_queue_issues::post::View>()
        .Append<handlers::v1_queues_queue_permissions_delete::post::View>()
        .Append<handlers::v1_queues_queue_permissions::post::View>()
        .Append<handlers::v1_users_me_delete::post::View>()
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
