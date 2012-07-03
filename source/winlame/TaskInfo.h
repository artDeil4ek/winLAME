//
// winLAME - a frontend for the LAME encoding engine
// Copyright (c) 2000-2012 Michael Fink
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
/// \file TaskInfo.h
/// \brief TaskInfo class

// include guard
#pragma once

/// task info
class TaskInfo
{
public:
   /// ctor
   TaskInfo()
      :m_taskStatus(statusWaiting),
       m_taskType(taskOther),
       m_uiProgress(0)
   {
   }

   /// task status
   enum TaskStatus
   {
      statusWaiting = 0,
      statusRunning,
      statusCompleted
   };

   /// task type
   enum TaskType
   {
      taskEncoding = 0,
      taskCdExtraction,
      taskOther
   };

   // get methods

   /// returns name of file, track, etc. associated with the task
   CString Name() const throw() { return m_cszName; }

   /// returns status of task
   TaskStatus Status() const throw() { return m_taskStatus; }

   /// returns type of task
   TaskType Type() const throw() { return m_taskType; }

   /// returns progress in percent; [0; 100]
   unsigned int Progress() const throw() { return m_uiProgress; }

   // set methods

   /// sets progress in percent; [0; 100]
   void Progress(unsigned int uiProgress) throw() { m_uiProgress = uiProgress; }

private:
   CString m_cszName;         ///< task name
   TaskStatus m_taskStatus;   ///< status
   TaskType m_taskType;       ///< task type
   unsigned int m_uiProgress; ///< progress in percent
};