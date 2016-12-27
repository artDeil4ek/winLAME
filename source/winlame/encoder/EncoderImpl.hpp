//
// winLAME - a frontend for the LAME encoding engine
// Copyright (c) 2000-2016 Michael Fink
// Copyright (c) 2004 DeXT
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
/// \file EncoderImpl.hpp
/// \brief contains the encoder implementation class definition
//
#pragma once

#include <string>
#include "EncoderInterface.h"
#include "ModuleInterface.hpp"
#include "ModuleManagerImpl.hpp"
#include <atomic>

namespace Encoder
{
   /// encoder implementation class
   class EncoderImpl : public EncoderInterface
   {
   public:
      /// ctor
      EncoderImpl()
      {
         running = false;
         paused = false;
         finished = false;
         percent = 0.f;
         handler = NULL;
         out_module_id = 0;
         settings_mgr = NULL;
         mod_manager = NULL;
         thread_handle = 0;
         error = 0;
         overwrite = true;
         delete_after_encode = false;
         warn_lossy = true;
         m_useTrackInfo = false;

         mutex = ::CreateMutex(NULL, FALSE, NULL);
      }

      /// dtor
      virtual ~EncoderImpl()
      {
         lockAccess();
         ::CloseHandle(mutex);
      }

      /// locks access to encoder object
      virtual void lockAccess()
      {
         WaitForSingleObject(mutex, INFINITE);
      }

      /// try locking access to encoder object
      virtual bool tryLockAccess()
      {
         return WAIT_OBJECT_0 == WaitForSingleObject(mutex, 0);
      }

      /// unlocks access again
      virtual void unlockAccess()
      {
         ::ReleaseMutex(mutex);
      }

      /// sets input filename
      virtual void setInputFilename(LPCTSTR infile)
      {
         infilename = infile;
      }

      /// sets output path
      virtual void setOutputPath(LPCTSTR outpath)
      {
         outpathname = outpath;
      }

      /// sets the settings manager to use
      virtual void setSettingsManager(SettingsManager *mgr)
      {
         settings_mgr = mgr;
      }

      /// sets the module manager to use
      virtual void setModuleManager(ModuleManager *mgr)
      {
         mod_manager = mgr;
      }

      /// sets output module to use
      virtual void setOutputModule(int moduleId)
      {
         out_module_id = moduleId;
      }

      /// sets output module per index
      virtual void setOutputModulePerIndex(int idx)
      {
         out_module_id = mod_manager->GetOutputModuleID(idx);
      }

      /// sets error handler to use if an error occurs
      virtual void setErrorHandler(EncoderErrorHandler *thehandler)
      {
         handler = thehandler;
      }

      /// set if files can be overwritten
      virtual void setOverwriteFiles(bool overwr)
      {
         overwrite = overwr;
      }

      /// set if source file should be deleted
      virtual void setDeleteAfterEncode(bool del)
      {
         delete_after_encode = del;
      }

      /// set lossy transcoding warning
      virtual void setWarnLossy(bool warn)
      {
         warn_lossy = warn;
      }

      /// sets track info to use instead of track info read from input module
      virtual void setTrackInfo(const TrackInfo& trackInfo)
      {
         m_useTrackInfo = true;
         m_trackInfo = trackInfo;
      }

      /// sets output playlist filename and enables playlist creation
      virtual void setOutputPlaylistFilename(LPCTSTR plname)
      {
         playlist_filename = outpathname;
         playlist_filename += plname;
      }

      /// starts encoding thread; returns immediately
      virtual void startEncode()
      {
         if (running) return;
         if (paused)
         {
            pauseEncoding();
            return;
         }

         desc.Empty();

         running = true;
         paused = false;

         lockAccess();
         thread_handle = _beginthread(threadProc, 0, this);
         unlockAccess();
      }

      /// returns if the encoder thread is running
      virtual bool isRunning()
      {
         if (tryLockAccess())
         {
            bool run = running;
            unlockAccess();
            return run;
         }
         else
            return true;
      }

      /// pauses encoding
      virtual void pauseEncoding()
      {
         lockAccess();
         paused = !paused;
         unlockAccess();
      }

      /// returns if the encoder is currently paused
      virtual bool isPaused()
      {
         lockAccess();
         bool pa = paused;
         unlockAccess();
         return pa;
      }

      /// stops encoding
      virtual void stopEncode()
      {
         // forces encoder to stop
         running = false;
         paused = false;

         // wait for thread to finish, at most 5 seconds
         if (thread_handle != 0)
         {
            ::WaitForSingleObject((HANDLE)thread_handle, 5 * 1000);
            thread_handle = 0;
         }
      }

      /// returns if there were errors during encoding
      virtual int getError()
      {
         lockAccess();
         int err = error;
         unlockAccess();
         return err;
      }

      /// returns the percent done of the encoding process
      virtual float queryPercentDone()
      {
         lockAccess();
         float perc = percent;
         unlockAccess();
         return perc;
      }

      /// returns encoding description string
      virtual CString getEncodingDescription()
      {
         lockAccess();
         CString str(desc);
         unlockAccess();
         return str;
      }

      /// creates output filename from input filename
      static CString GetOutputFilename(const CString& outputPath, const CString& cszInputFilename, OutputModule& outputModule);

      /// returns if input module with given id is lossy
      static bool IsLossyInputModule(int in_module_id);

      /// returns if output module with given id is lossy
      static bool IsLossyOutputModule(int out_module_id);

   protected:
      /// static thread procedure
      static void __cdecl threadProc(void *ptr)
      {
         EncoderImpl* enc = reinterpret_cast<EncoderImpl*>(ptr);
         enc->encode();
         _endthread();
      }

      /// the real encoder function
      void encode();

      /// prepares input module for work
      bool PrepareInputModule(InputModule& inputModule, CString& cszInputFilename,
         SettingsManager& settingsManager, TrackInfo& trackInfo, SampleContainer& sampleContainer,
         int& error);

      /// prepares output module for work; step 1 of 2; see InitOutputModule()
      bool PrepareOutputModule(InputModule& inputModule, OutputModule& outputModule,
         SettingsManager& settingsManager,
         const CString& cszInputFilename, CString& cszOutputFilename, int& error);

      /// checks if the input and output filenames are the same and modifies output filename
      bool CheckSameInputOutputFilenames(const CString& cszInputFilename,
         CString& cszOutputFilename, OutputModule& outputModule);

      /// checks if a direct copy of the cd extracted wave file is possible
      bool CheckCDExtractDirectCopy(InputModule& inputModule, OutputModule& outputModule,
         SettingsManager& settingsManager);

      /// generates temporary output filename
      void GenerateTempOutFilename(const CString& cszOriginalFilename, CString& tempFilename);

      /// inits output module; step 2 of 2; see PrepareOutputModule()
      bool InitOutputModule(OutputModule& outputModule, const CString& cszTempOutputFilename,
         SettingsManager& settingsManager, TrackInfo& trackInfo, SampleContainer& sampleContainer,
         int& error);

      /// formats encoding description
      void FormatEncodingDescription(InputModule& inputModule, OutputModule& outputModule,
         SampleContainer& sampleContainer);

      /// checks if we should warn about transcoding
      bool CheckWarnTranscoding(InputModule& inputModule, OutputModule& outputModule);

      /// main encoding loop
      void MainLoop(InputModule& inputModule, OutputModule& outputModule,
         SampleContainer& sampleContainer, bool& bSkipFile);

      /// writes playlist entry
      void WritePlaylistEntry(const CString& cszOutputFilename);

   protected:
      /// input filename
      CString infilename;

      /// output pathname
      CString outpathname;

      CString m_precalculatedOutputFilename;

      /// the settings manager to use
      SettingsManager *settings_mgr;

      /// module manager
      ModuleManager *mod_manager;

      /// error handler interface
      EncoderErrorHandler *handler;

      /// id of the output module to use
      int out_module_id;

      /// indicates if files can be overwritten
      bool overwrite;

      /// indicates if source file should be deleted after encoding
      bool delete_after_encode;

      /// warn about lossy transcoding
      bool warn_lossy;

      /// track info to store in output
      TrackInfo m_trackInfo;

      /// indicates if the provided track info should be used instead of track info read from
      /// the input file
      bool m_useTrackInfo;

      /// indicates percent done
      float percent;

      /// indicates if encoder is paused
      std::atomic<bool> paused;

      /// indicates if encoder is running
      std::atomic<bool> running;

      /// indicates if encoder has been finished
      std::atomic<bool> finished;

      /// error indicator
      int error;

      /// encoding description
      CString desc;

      /// playlist filename
      CString playlist_filename;

      /// current thread handle
      unsigned long thread_handle;

      /// mutex to lock encoder object access
      HANDLE mutex;
   };

} // namespace Encoder