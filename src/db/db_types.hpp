#pragma once

#include <userver/storages/postgres/io/enum_types.hpp>

#include <docs/issues.hpp>
#include <docs/users.hpp>

namespace storages::postgres::io {

template <>
struct CppToUserPg<defs::users::UserRole> : EnumMappingBase<defs::users::UserRole> {
    static constexpr DBTypeName postgres_name{"tidy", "user_role"};
    static constexpr EnumeratorList enumerators{
        {EnumType::kPending, "pending"},
        {EnumType::kAdmin, "admin"},
        {EnumType::kManager, "manager"},
        {EnumType::kRecruiter, "recruiter"},
        {EnumType::kDeveloper, "developer"},
    };
};

template <>
struct CppToUserPg<defs::issues::IssueAction> : EnumMappingBase<defs::issues::IssueAction> {
    static constexpr DBTypeName postgres_name{"tidy", "issue_action"};
    static constexpr EnumeratorList enumerators{
        {EnumType::kView, "view"},
        {EnumType::kCreate, "create"},
        {EnumType::kEditOwn, "edit_own"},
        {EnumType::kEditAny, "edit_any"},
        {EnumType::kDeleteOwn, "delete_own"},
        {EnumType::kDeleteAny, "delete_any"},
    };
};

template <>
struct CppToUserPg<defs::issues::IssueStatus> : EnumMappingBase<defs::issues::IssueStatus> {
    static constexpr DBTypeName postgres_name{"tidy", "issue_status"};
    static constexpr EnumeratorList enumerators{
        {EnumType::kDescription, "description"},
        {EnumType::kBacklog, "backlog"},
        {EnumType::kPlanned, "planned"},
        {EnumType::kInProgress, "in_progress"},
        {EnumType::kNeedInfo, "need_info"},
        {EnumType::kPullRequest, "pull_request"},
        {EnumType::kMerged, "merged"},
        {EnumType::kClosed, "closed"},
    };
};

template <>
struct CppToUserPg<defs::issues::IssuePriority> : EnumMappingBase<defs::issues::IssuePriority> {
    static constexpr DBTypeName postgres_name{"tidy", "issue_priority"};
    static constexpr EnumeratorList enumerators{
        {EnumType::kMinor, "minor"},
        {EnumType::kNormal, "normal"},
        {EnumType::kCritical, "critical"},
        {EnumType::kBlocker, "blocker"},
    };
};

template <>
struct CppToUserPg<defs::issues::IssueType> : EnumMappingBase<defs::issues::IssueType> {
    static constexpr DBTypeName postgres_name{"tidy", "issue_type"};
    static constexpr EnumeratorList enumerators{
        {EnumType::kTask, "task"},
        {EnumType::kBug, "bug"},
        {EnumType::kLinker, "linker"},
        {EnumType::kEpic, "epic"},
    };
};

}  // namespace storages::postgres::io
