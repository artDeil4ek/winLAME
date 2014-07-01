//
// winLAME - a frontend for the LAME encoding engine
// Copyright (c) 2000-2014 Michael Fink
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
/// \file FreeDbDiscListDlg.cpp
/// \brief FreeDB disc list dialog
//

// includes
#include "stdafx.h"
#include "resource.h"
#include "FreeDbDiscListDlg.hpp"

using UI::FreeDbDiscListDlg;

LRESULT FreeDbDiscListDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   // center the dialog on the screen
   CenterWindow();

   // set icons
   HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON_WINLAME),
      IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
   SetIcon(hIcon, TRUE);
   HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON_WINLAME),
      IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
   SetIcon(hIconSmall, FALSE);

   m_list.SubclassWindow(GetDlgItem(IDC_FREEDB_LIST_TRACKS));

   m_list.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

   m_list.InsertColumn(0, _T("Album Name"), LVCFMT_LEFT, 200, 0);
   m_list.InsertColumn(1, _T("Genre"), LVCFMT_LEFT, 50, 0);

   m_list.SelectItem(0);

   UpdateList();

   return TRUE;
}

void FreeDbDiscListDlg::UpdateList()
{
   CString cszText;
   unsigned int nMax = m_vecResults.size();
   for (unsigned int n = 0; n < nMax; n++)
   {
      cszText.Format(_T("%hs / %hs"), m_vecResults[n].dartist.c_str(), m_vecResults[n].dtitle.c_str());
      cszText.Replace(_T("\n"), _T("")); // libfreedb may add a linefeed character

      int nItem = m_list.InsertItem(m_list.GetItemCount(), cszText);
      cszText = m_vecResults[n].category.c_str();
      m_list.SetItemText(nItem, 1, cszText);
   }
}

LRESULT FreeDbDiscListDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   int iItem = m_list.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
   if (iItem != -1)
   {
      EndDialog(wID);
      m_uSelectedItem = static_cast<unsigned int>(iItem);
   }

   return 0;
}