//
// winLAME - a frontend for the LAME encoding engine
// Copyright (c) 2000-2016 Michael Fink
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
/// \file CDRipTitleFormatManager.hpp
/// \brief CD rip title format manager
//
#pragma once

// includes

// forward references
struct CDRipDiscInfo;
struct CDRipTrackInfo;
struct UISettings;

/// Manager to help formatting titles while CD ripping
class CDRipTitleFormatManager
{
public:
   static CString FormatTitle(UISettings& settings,
      const CDRipDiscInfo& discInfo, const CDRipTrackInfo& trackInfo);

   /// formats a track title for CD ripping
   static CString FormatTitle(const CString& format,
      const CDRipDiscInfo& discInfo, const CDRipTrackInfo& trackInfo);

   /// returns a filename generated by title, with only valid characters for filenames
   static CString GetFilenameByTitle(const CString& title);

   /// returns all available tags
   static std::vector<CString> GetAllTags();

private:
   /// ctor; unused
   CDRipTitleFormatManager() = delete;
   /// dtor; unused
   ~CDRipTitleFormatManager() = delete;
};
