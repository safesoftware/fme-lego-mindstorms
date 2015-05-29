/* =============================================================================

   Name     : legomindstormsmovefunc.cpp

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Implementation of LegoMindstormsMoveFunc

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

#include "legomindstormsfunc_move.h"
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
LegoMindstormsMoveFunc::LegoMindstormsMoveFunc(const string& functionName)
: functionName_(functionName)
{
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsMoveFunc::execute(
   IFMEFeature& feature,
   const IFMEStringArray& parameterList,
   Brick<Sensor^, Sensor^, Sensor^, Sensor^>^ ev3Brick)
{
   if (parameterList.entries() < 5)
   {
      return FME_FAILURE;
   }

   string paramStr;

   // Left Motor
   MotorPort port;
   paramStr = FME_getString(*parameterList.elementAt(1));
   if (!LMFUtil::stringToMotorPort(paramStr, port))
   {
      return FME_FAILURE;
   }
   ev3Brick->Vehicle->LeftPort = port;

   // Right Motor
   paramStr = FME_getString(*parameterList.elementAt(2));
   if (!LMFUtil::stringToMotorPort(paramStr, port))
   {
      return FME_FAILURE;
   }
   ev3Brick->Vehicle->RightPort = port;

   // Left Reversed
   bool reversed;
   paramStr = FME_getString(*parameterList.elementAt(3));
   if (FME_stringToBool(paramStr, reversed) != FME_SUCCESS)
   {
      return FME_FAILURE;
   }
   ev3Brick->Vehicle->ReverseLeft = reversed;

   // Right Reversed
   paramStr = FME_getString(*parameterList.elementAt(4));
   if (FME_stringToBool(paramStr, reversed) != FME_SUCCESS)
   {
      return FME_FAILURE;
   }
   ev3Brick->Vehicle->ReverseRight = reversed;

	// How do we want to move? STRAIGHT / TURN / SPIN
	const string moveType(FME_getString(*parameterList.elementAt(5)));
	if (moveType == kLMF_MoveTypeStraight)
	{
		// Requires 9 parameters.
		if (parameterList.entries() < 9)
		{
			return FME_FAILURE;
		}

      FME_Int8 speed;
      paramStr = FME_getString(*parameterList.elementAt(6));
		if (FME_stringToInt(paramStr, speed))
      {
         return FME_FAILURE;
      }

      LMFDuration durationType;
		paramStr = FME_getString(*parameterList.elementAt(7));
      if (!LMFUtil::stringToDuration(paramStr, durationType))
      {
         return FME_FAILURE;
      }

		FME_Int32 durationValue;
      paramStr = FME_getString(*parameterList.elementAt(8));
      if (FME_stringToInt(paramStr, durationValue) != FME_SUCCESS)
      {
         return FME_FAILURE;
      }

      if (durationType == LMFDuration_Time)
      {
         vehicleStraightTime(ev3Brick, speed, durationValue);
      }
      else if (durationType == LMFDuration_Rotations)
      {
         vehicleStraightRotations(ev3Brick, speed, durationValue);
      }
	}
   else if (moveType == kLMF_MoveTypeTurn)
   {
		// Requires 11 parameters.
		if (parameterList.entries() < 11)
		{
			return FME_FAILURE;
		}

      FME_Int8 speed;
      paramStr = FME_getString(*parameterList.elementAt(6));
      if (FME_stringToInt(paramStr, speed) != FME_SUCCESS)
      {
         return FME_FAILURE;
      }

      LMFDirection direction;
      paramStr = FME_getString(*parameterList.elementAt(7));
      if (!getDirectionTypeFromString(paramStr, direction))
      {
         return FME_FAILURE;
      }

      FME_Int8 turnPercentage;
      paramStr = FME_getString(*parameterList.elementAt(8));
      if (FME_stringToInt(paramStr, turnPercentage) != FME_SUCCESS)
      {
         return FME_FAILURE;
      }

      LMFDuration durationType;
      paramStr = FME_getString(*parameterList.elementAt(9));
      if (!LMFUtil::stringToDuration(paramStr, durationType))
      {
         return FME_FAILURE;
      }

      FME_Int32 durationValue;
      paramStr = FME_getString(*parameterList.elementAt(10));
      if (FME_stringToInt(paramStr, durationValue) != FME_SUCCESS)
      {
         return FME_FAILURE;
      }

      if (durationType == LMFDuration_Time)
      {
         vehicleTurnTime(ev3Brick, speed, direction, turnPercentage, durationValue);
      }
      else if (durationType == LMFDuration_Rotations)
      {
         vehicleTurnRotations(ev3Brick, speed, direction, turnPercentage, durationValue);
      }
   }
   else if (moveType == kLMF_MoveTypeSpin)
   {
      // Requires 10 parameters.
		if (parameterList.entries() < 10)
		{
			return FME_FAILURE;
      }

      FME_Int8 speed;
      paramStr = FME_getString(*parameterList.elementAt(6));
      if (FME_stringToInt(paramStr, speed) != FME_SUCCESS)
      {
         return FME_FAILURE;
      }

      LMFDirection direction;
      paramStr = FME_getString(*parameterList.elementAt(7));
      if (!getDirectionTypeFromString(paramStr, direction))
      {
         return FME_FAILURE;
      }

      LMFDuration durationType;
      paramStr = FME_getString(*parameterList.elementAt(8));
      if (!LMFUtil::stringToDuration(paramStr, durationType))
      {
         return FME_FAILURE;
      }

      FME_Int32 durationValue;
      paramStr = FME_getString(*parameterList.elementAt(9));
      if (FME_stringToInt(paramStr, durationValue) != FME_SUCCESS)
      {
         return FME_FAILURE;
      }

      if (durationType == LMFDuration_Time)
      {
         vehicleSpinTime(ev3Brick, speed, direction, durationValue);
      }
      else if (durationType == LMFDuration_Rotations)
      {
         vehicleSpinRotations(ev3Brick, speed, direction, durationValue);
      }
   }
   else
   {
      return FME_FAILURE;
   }

   ev3Brick->Vehicle->Off();

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
bool LegoMindstormsMoveFunc::getDirectionTypeFromString(
   const string& directionStr,
   LMFDirection& direction) const
{
   if (directionStr == kLMF_Left)
   {
      direction = LMFDirection_Left;
      return true;
   }
   else if (directionStr == kLMF_Right)
   {
      direction = LMFDirection_Right;
      return true;
   }

   return false;
}

//------------------------------------------------------------------------------
void LegoMindstormsMoveFunc::vehicleStraightTime(
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
   FME_Int8 speed, 
   FME_Int32 seconds) const
{
   ev3Brick->Vehicle->Forward(speed);
   Thread::Sleep(seconds*kLMF_milliSecsPerSecond);
   ev3Brick->Vehicle->Brake();
}

//------------------------------------------------------------------------------
void LegoMindstormsMoveFunc::vehicleStraightRotations(
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
   const FME_Int8 speed, 
   const FME_Int32 degrees) const
{
   ev3Brick->Vehicle->Forward(speed, degrees, false);
   LMFUtil::waitForMotorsToStop(ev3Brick, false);
}

//------------------------------------------------------------------------------
void LegoMindstormsMoveFunc::vehicleTurnTime(
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
   FME_Int8 speed, 
   LMFDirection direction, 
   FME_Int8 percentage, 
   FME_Int32 seconds) const
{
   if (direction == LMFDirection_Left)
   {
      ev3Brick->Vehicle->TurnLeftForward(speed, percentage);
      Thread::Sleep(seconds*kLMF_milliSecsPerSecond);
      ev3Brick->Vehicle->Brake();
   }
   else if (direction == LMFDirection_Right)
   {
      ev3Brick->Vehicle->TurnRightForward(speed, percentage);
      Thread::Sleep(seconds*kLMF_milliSecsPerSecond);
      ev3Brick->Vehicle->Brake();
   }
}

//------------------------------------------------------------------------------
void LegoMindstormsMoveFunc::vehicleTurnRotations(
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
   FME_Int8 speed, 
   LMFDirection direction, 
   FME_Int8 percentage, 
   FME_Int32 degrees) const
{
   if (direction == LMFDirection_Left)
   {
      ev3Brick->Vehicle->TurnLeftForward(speed, percentage, degrees, false);
      LMFUtil::waitForMotorsToStop(ev3Brick, false);
   }
   else if (direction == LMFDirection_Right)
   {
      ev3Brick->Vehicle->TurnRightForward(speed, percentage, degrees, false);
      LMFUtil::waitForMotorsToStop(ev3Brick, false);
   }
}

//------------------------------------------------------------------------------
void LegoMindstormsMoveFunc::vehicleSpinTime(
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
   FME_Int8 speed, 
   LMFDirection direction, 
   FME_Int32 seconds) const
{
   if (direction == LMFDirection_Left)
   {
      ev3Brick->Vehicle->SpinLeft(speed);
      Thread::Sleep(seconds*kLMF_milliSecsPerSecond);
      ev3Brick->Vehicle->Brake();
   }
   else if (direction == LMFDirection_Right)
   {
      ev3Brick->Vehicle->SpinRight(speed);
      Thread::Sleep(seconds*kLMF_milliSecsPerSecond);
      ev3Brick->Vehicle->Brake();
   }
}

//------------------------------------------------------------------------------
void LegoMindstormsMoveFunc::vehicleSpinRotations(
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
   FME_Int8 speed, 
   LMFDirection direction, 
   FME_Int32 degrees) const
{
   if (direction == LMFDirection_Left)
   {
      ev3Brick->Vehicle->SpinLeft(speed, degrees, false);
      LMFUtil::waitForMotorsToStop(ev3Brick, false);
   }
   else if (direction == LMFDirection_Right)
   {
      ev3Brick->Vehicle->SpinRight(speed, degrees, false);
      LMFUtil::waitForMotorsToStop(ev3Brick, false);
   }
}
