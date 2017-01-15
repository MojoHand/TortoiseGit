// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2017 - TortoiseGit
// Copyright (C) 2003-2016 - TortoiseSVN

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "stdafx.h"
#include "GitWCRev.h"
#include "status.h"
#include "registry.h"
#include "StringUtils.h"
#include "UnicodeUtils.h"
#include <fstream>

void LoadIgnorePatterns(const char* wc, GitWCRev_t* SubStat)
{
	std::string path = wc;
	std::string ignorepath = path + "/.GitWCRevignore";

	std::ifstream infile;
	infile.open(ignorepath);
	if (infile.good())
	{
		std::string line;
		while (std::getline(infile, line))
		{
			if (!line.empty())
				SubStat->ignorepatterns.emplace(line, path.size());
		}
	}
}

tstring Tokenize(const _TCHAR* str, const _TCHAR* delim, tstring::size_type& iStart)
{
	const _TCHAR* pstr = str + iStart;
	const _TCHAR* r = wcsstr(pstr, delim);
	tstring::size_type dlen = wcslen(delim);

	while (r)
	{
		if (r > pstr)
		{
			iStart = tstring::size_type(r - str) + dlen;
			return tstring(pstr, tstring::size_type(r - pstr));
		}
		pstr = r + dlen;
		r = wcsstr(pstr, delim);
	}

	if (pstr[0])
	{
		iStart = tstring::size_type(wcslen(str));
		return tstring(pstr);
	}
	return tstring();
}

int GetStatus(const TCHAR* wc, GitWCRev_t& SubStat)
{
	std::string wcA = CUnicodeUtils::StdGetUTF8(wc);
	CAutoBuf wcroot;
	if (git_repository_discover(wcroot, wcA.c_str(), 0, nullptr) < 0)
		return ERR_NOWC;

	CAutoRepository repo;
	if (git_repository_open(repo.GetPointer(), wcroot->ptr))
		return ERR_NOWC;

	/*CAutoConfig config(true);
	git_config_add_file_ondisk(config, CGit::GetGitPathStringA(projectConfig), GIT_CONFIG_LEVEL_LOCAL, FALSE);
	git_config_add_file_ondisk(config, CGit::GetGitPathStringA(globalConfig), GIT_CONFIG_LEVEL_GLOBAL, FALSE);
	git_config_add_file_ondisk(config, CGit::GetGitPathStringA(globalXDGConfig), GIT_CONFIG_LEVEL_XDG, FALSE);
	if (!systemConfig.IsEmpty())
	git_config_add_file_ondisk(config, CGit::GetGitPathStringA(systemConfig), GIT_CONFIG_LEVEL_SYSTEM, FALSE);
	if (!programDataConfig.IsEmpty())
	git_config_add_file_ondisk(config, CGit::GetGitPathStringA(programDataConfig), GIT_CONFIG_LEVEL_PROGRAMDATA, FALSE);
	git_repository_set_config(repository, config);*/

	if (git_repository_head_unborn(repo)) // TODO!
		return ERR_NOWC;

	CAutoReference head;
	if (git_repository_head(head.GetPointer(), repo) < 0)
		return ERR_GIT_ERR;

	CAutoObject object;
	git_reference_peel(object.GetPointer(), head, GIT_OBJ_COMMIT);

	const git_oid* oid = git_object_id(object);
	git_oid_cpy((git_oid*)SubStat.HeadHash, oid);
	git_oid_tostr(SubStat.HeadHashReadable, sizeof(SubStat.HeadHashReadable), oid);

	CAutoCommit commit;
	if (git_commit_lookup(commit.GetPointer(), repo, oid) < 0)
		return ERR_GIT_ERR;

	const git_signature* sig = git_commit_author(commit);
	SubStat.HeadAuthor = sig->name;
	SubStat.HeadEmails = sig->email;
	SubStat.HeadTime = sig->when.time;

	struct TagPayload { git_repository* repo; GitWCRev_t& SubStat; } tagpayload = {repo, SubStat};

	if (git_tag_foreach(repo, [](const char*, git_oid* tagoid, void* payload)
	{
		auto pl = reinterpret_cast<struct TagPayload*>(payload);
		if (git_oid_cmp(tagoid, (git_oid*)pl->SubStat.HeadHash) == 0)
		{
			pl->SubStat.bIsTagged = TRUE;
			return 0;
		}

		CAutoTag tag;
		if (git_tag_lookup(tag.GetPointer(), pl->repo, tagoid))
			return -1;
		CAutoObject tagObject;
		if (git_tag_peel(tagObject.GetPointer(), tag))
			return -1;
		if (git_oid_cmp(git_object_id(tagObject), (git_oid*)pl->SubStat.HeadHash) == 0)
			pl->SubStat.bIsTagged = TRUE;

		return 0;
	}, &tagpayload))
		return ERR_GIT_ERR;

	git_status_options git_status_options = GIT_STATUS_OPTIONS_INIT;
	git_status_options.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED | GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;
	if (!SubStat.bExternals)
		git_status_options.flags |= GIT_STATUS_OPT_EXCLUDE_SUBMODULES;
	CAutoStatusList status;
	if (git_status_list_new(status.GetPointer(), repo, &git_status_options) < 0)
		return ERR_GIT_ERR;

	for (size_t i = 0, maxi = git_status_list_entrycount(status); i < maxi; ++i)
	{
		const git_status_entry* s = git_status_byindex(status, i);
		if (s->status == GIT_STATUS_WT_NEW)
			SubStat.HasUnversioned = TRUE;
		if (s->index_to_workdir && s->index_to_workdir->new_file.mode == GIT_FILEMODE_COMMIT)
		{
			unsigned int smstatus = 0;
			if (!git_submodule_status(&smstatus, repo, s->index_to_workdir->new_file.path, GIT_SUBMODULE_IGNORE_UNSPECIFIED))
			{
				if (smstatus & GIT_SUBMODULE_STATUS_WD_MODIFIED)
					SubStat.bHasSubmoduleNewCommits = TRUE;
				else if ((smstatus & GIT_SUBMODULE_STATUS_WD_INDEX_MODIFIED) || (smstatus & GIT_SUBMODULE_STATUS_WD_WD_MODIFIED))
					SubStat.bHasSubmoduleMods = TRUE;
				else if (smstatus & GIT_SUBMODULE_STATUS_WD_UNTRACKED)
					SubStat.bHasSubmoduleUnversioned = TRUE;
			}
		}
		else if (s->status != GIT_STATUS_CURRENT && s->status != GIT_STATUS_WT_NEW)
			SubStat.HasMods = TRUE;
	}

	return 0;
}
