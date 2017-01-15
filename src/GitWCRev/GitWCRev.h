// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2017 - TortoiseGit
// Copyright (C) 2003-2015 - TortoiseSVN

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
#pragma once

/**
 * \ingroup GitWCRev
 * This structure is used as the status baton for WC crawling
 * and contains all the information we are collecting.
 */
struct GitWCRev_t
{
	GitWCRev_t()
		: HasMods(FALSE)
		, HasUnversioned(FALSE)
		, bExternals(TRUE)
		, bIsGitItem(FALSE)
		, bHasSubmoduleNewCommits(FALSE)
		, bHasSubmoduleMods(FALSE)
		, bHasSubmoduleUnversioned(FALSE)
		, bIsTagged(FALSE)
		, bIsUnborn(FALSE)
		, HeadTime(0)
	{
		HeadHash[0] = '\0';
		HeadHashReadable[0] = '\0';
	}

	char HeadHash[GIT_OID_RAWSZ];
	char HeadHashReadable[GIT_OID_HEXSZ + 1];
	std::string HeadAuthor;
	std::string HeadEmails;
	time_t HeadTime;
	BOOL HasMods;           // True if local modifications found
	BOOL HasUnversioned;    // True if unversioned items found
	BOOL bExternals;        // If TRUE, status of externals is included
	BOOL bIsGitItem;           // True if the item is under Git version control
	BOOL bHasSubmoduleNewCommits;
	BOOL bHasSubmoduleMods;
	BOOL bHasSubmoduleUnversioned;
	BOOL bIsTagged;
	BOOL bIsUnborn;
	std::set<std::tuple<std::string, size_t>> ignorepatterns;   // a list of file patterns to ignore
};
