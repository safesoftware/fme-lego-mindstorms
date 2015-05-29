/* =============================================================================

   Name     : legomindstormsfunc.cpp

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Implementation of LegoMindstormsFunc

         Copyright (c) 1994 - 2015, Safe Software Inc. All rights reserved.

   Redistribution and use of this sample code in source and binary forms, with 
   or without modification, are permitted provided that the following 
   conditions are met:
   * Redistributions of source code must retain the above copyright notice, 
     this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright notice, 
     this list of conditions and the following disclaimer in the documentation 
     and/or other materials provided with the distribution.

   THIS SAMPLE CODE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
   TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
   OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SAMPLE CODE, EVEN IF 
   ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

============================================================================= */

#include "legomindstormsfunc.h"
#include "legomindstormsfunc_move.h"
#include "legomindstormsfunc_motor.h"
#include "legomindstormsfunc_sense.h"
#include "legomindstormsfunc_path.h"
#include "legomindstormsfunc_util.h"

#include <fmetypes.h>
#include <fmepluginutil.h>
#include <isession.h>
#include <ilogfile.h>
#include <fmedllexport.h>
#include <fmestring.h>

#using <MonoBrick.dll>

using namespace System;
using namespace MonoBrick::EV3;

//------------------------------------------------------------------------------
// Global pointers for utility objects.
IFMELogFile* gLogFile = NULL;
IFMESession* gFMESession = NULL;

// We assume we will only ever control one EV3 robot at a time.
// Here's our connection to that one robot.
ref class ManagedGlobals
{
public:
   static Brick<Sensor^, Sensor^, Sensor^, Sensor^>^ ev3 = nullptr;
};

// Whether we initialized our robot.
static bool gInitializedEV3 = false;

//------------------------------------------------------------------------------
FME_DLLEXPORT_C void FME_acceptSession(IFMESession* fmeSession)
{
   gFMESession = fmeSession;
   gLogFile = gFMESession->logFile();
   FMEString::setSession(gFMESession);
   FMEStringArray::setSession(gFMESession);
}

//------------------------------------------------------------------------------
FME_DLLEXPORT_C const char* FME_apiVersion()
{
   return kFMEDevKitVersion;
}

//------------------------------------------------------------------------------
FME_DLLEXPORT_C FME_MsgNum FME_createFunction(
   IFMELogFile& logFile,
   IFMEMappingFile& mappingFile,
   IFMECoordSysManager& coordSysMan,
   IFMEFunction*& function,
   const char* functionName)
{
   function = new LegoMindstormsFunc(functionName);

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_DLLEXPORT_C FME_MsgNum FME_destroyFunction(IFMEFunction*& function)
{
   delete function; function = NULL;
   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_DLLEXPORT_C FME_MsgNum FME_finalize(IFMEServiceManager& serviceManager)
{
   // Shut down the EV3 if we ever started it.
   if (gInitializedEV3)
   {
      ManagedGlobals::ev3->Vehicle->Off();
      ManagedGlobals::ev3->Connection->Close();
      ManagedGlobals::ev3 = nullptr;
      gInitializedEV3 = false;
   }

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
LegoMindstormsFunc::LegoMindstormsFunc(const char* functionName)
: functionName_(functionName)
{
}

//------------------------------------------------------------------------------
LegoMindstormsFunc::~LegoMindstormsFunc()
{
}

//------------------------------------------------------------------------------
FME_MsgNum LegoMindstormsFunc::initialize()
{
   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_MsgNum LegoMindstormsFunc::abort()
{
   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_MsgNum LegoMindstormsFunc::close()
{
   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_MsgNum LegoMindstormsFunc::execute(
   IFMEFeature& feature,
   const IFMEStringArray& parameterList,
   IFMEString& returnValue)
{
   // Require at least one parameter.
   if (parameterList.entries() == 0)
   {
      return FME_FAILURE;
   }

   try
   {
      // If we've never initialized our robot, do so now.
      if (!gInitializedEV3)
      {
         String^ connType = gcnew String(kLMF_ConnectionType);
         ManagedGlobals::ev3 = gcnew Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^>(connType);
         ManagedGlobals::ev3->Connection->Open();
         gInitializedEV3 = true;
      }

      // Do the rest of the work based on the mode.
      FME_Status status;
      const string modeStr(FME_getString(*parameterList.elementAt(0)));
      if (modeStr == kLMF_ModeMove)
      {
         LegoMindstormsMoveFunc func(functionName_);
         status = func.execute(feature, parameterList, ManagedGlobals::ev3);
      }
      else if (modeStr == kLMF_ModeSense)
      {
         LegoMindstormsSenseFunc func(functionName_);
         status = func.execute(feature, parameterList, ManagedGlobals::ev3);
      }
      else if (modeStr == kLMF_ModePath)
      {
         LegoMindstormsPathFunc func(functionName_);
         status = func.execute(feature, parameterList, ManagedGlobals::ev3);
      }
      else if (modeStr == kLMF_ModeMotor)
      {
         LegoMindstormsMotorFunc func(functionName_);
         status = func.execute(feature, parameterList, ManagedGlobals::ev3);
      }
      else
      {
         return FME_FAILURE;
      }

      if (status != FME_SUCCESS)
      {
         return FME_FAILURE;
      }
   }
   catch (Exception ^e)
   {
      string msg = "Error: " + LMFUtil::getString(e->Message);
      gLogFile->logMessageString(msg.c_str(), FME_ERROR);

      msg = LMFUtil::getString(e->StackTrace);
      gLogFile->logMessageString(msg.c_str(), FME_ERROR);

      return FME_FAILURE;
   }

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_MsgNum LegoMindstormsFunc::inverse(
   IFMEFeature& feature,
   const IFMEStringArray& parameterList,
   IFMEString& returnValue)
{
   return FME_SUCCESS;
}
