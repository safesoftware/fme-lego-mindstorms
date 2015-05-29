/* =============================================================================

   Name     : legomindstormsmotorfunc.cpp

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Implementation of LegoMindstormsMotorFunc

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

#include "legomindstormsfunc_motor.h"
#include "legomindstormsfunc_priv.h"
#include "legomindstormsfunc_util.h"

#include <fmetypes.h>
#include <fmepluginutil.h>

#using <System.dll>
using namespace System;
using namespace System::Threading;

#using <MonoBrick.dll>
using namespace MonoBrick::EV3;

//------------------------------------------------------------------------------
LegoMindstormsMotorFunc::LegoMindstormsMotorFunc(const string& functionName)
: functionName_(functionName)
{
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsMotorFunc::execute(
   IFMEFeature& feature,
   const IFMEStringArray& parameterList,
   Brick<Sensor^, Sensor^, Sensor^, Sensor^>^ ev3Brick)
{
   // Requires 4 parameters.
	if (parameterList.entries() < 4)
	{
		return FME_FAILURE;
	}

   string paramStr;

   // What motor are we trying to move?
   MotorPort port;
   Motor^ activeMotor;
   paramStr = FME_getString(*parameterList.elementAt(1));
   if (!LMFUtil::stringToMotorPort(paramStr, port))
   {
      return FME_FAILURE;
   }
   switch (port)
   {
   case MotorPort::OutA: activeMotor = ev3Brick->MotorA; break;
   case MotorPort::OutB: activeMotor = ev3Brick->MotorB; break;
   case MotorPort::OutC: activeMotor = ev3Brick->MotorC; break;
   case MotorPort::OutD: activeMotor = ev3Brick->MotorD; break;
   default: return FME_FAILURE;
   }

   FME_Int8 speed;
   paramStr = FME_getString(*parameterList.elementAt(2));
   if (FME_stringToInt(paramStr, speed) != FME_SUCCESS)
   {
      return FME_FAILURE;
   }

   LMFDuration durationType;
   paramStr = FME_getString(*parameterList.elementAt(3));
   if (!LMFUtil::stringToDuration(paramStr, durationType))
   {
      return FME_FAILURE;
   }

   FME_Int32 durationValue;
   paramStr = FME_getString(*parameterList.elementAt(4));
   if (FME_stringToInt(paramStr, durationValue) != FME_SUCCESS)
   {
      return FME_FAILURE;
   }

   if (durationType == LMFDuration_Time)
   {
      runOnTime(activeMotor, speed, durationValue);
   }
   else if (durationType == LMFDuration_Rotations)
   {
      runOnRotations(activeMotor, speed, durationValue);
   }

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsMotorFunc::runOnTime(
   Motor ^motor, 
   const FME_Int8 speed, 
   const FME_Int32 seconds)
{
   motor->On(speed);
   Thread::Sleep(seconds*kLMF_milliSecsPerSecond);
   motor->Brake();
   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsMotorFunc::runOnRotations(
   Motor ^motor, 
   const FME_Int8 speed, 
   const FME_Int32 degrees)
{
   motor->On(speed, degrees, true);
   LMFUtil::waitForMotorToStop(motor, false);
   return FME_SUCCESS;
}
