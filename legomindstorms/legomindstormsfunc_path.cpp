/* =============================================================================

   Name     : legomindstormspathfunc.cpp

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Implementation of LegoMindstormsPathFunc

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

#include "legomindstormsfunc_path.h"
#include "legomindstormsfunc_util.h"

#include <fmetypes.h>
#include <istring.h>
#include <ifeature.h>
#include <isession.h>
#include <ipoint.h>
#include <iline.h>
#include <igeometrytools.h>
#include <ilogfile.h>
#include <fmepluginutil.h>
#include <math.h>

#using <System.dll>
using namespace System;
using namespace System::Threading;

#using <MonoBrick.dll>
using namespace MonoBrick::EV3;

//------------------------------------------------------------------------------
extern IFMELogFile* gLogFile;
extern IFMESession* gFMESession;

//------------------------------------------------------------------------------
LegoMindstormsPathFunc::LegoMindstormsPathFunc(const string& functionName)
: functionName_(functionName)
{
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsPathFunc::execute(
   IFMEFeature& feature,
   const IFMEStringArray& parameterList,
   Brick<Sensor^, Sensor^, Sensor^, Sensor^>^ ev3Brick)
{
   // Make sure we have the correct number of parameters
   if (parameterList.entries() != 9)
   {
      gLogFile->logMessageString("Incorrect number of parameters, requires 9", FME_ERROR);
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

   // Speed
   FME_Int8 speed;
   paramStr = FME_getString(*parameterList.elementAt(5));
   if (FME_stringToInt(paramStr, speed) != FME_SUCCESS)
   {
      return FME_FAILURE;
   }

   // Linear Scale
   FME_Real64 linearScale;
   paramStr = FME_getString(*parameterList.elementAt(6));
   if (FME_stringToFloat(paramStr, linearScale) != FME_SUCCESS)
   {
      return FME_FAILURE;
   }

   // Rotational Scale
   FME_Real64 rotationalScale;
   paramStr = FME_getString(*parameterList.elementAt(7));
   if (FME_stringToFloat(paramStr, rotationalScale) != FME_SUCCESS)
   {
      return FME_FAILURE;
   }

   // Reset Tachometers
   bool resetTachos;
   paramStr = FME_getString(*parameterList.elementAt(8));
   if (FME_stringToBool(paramStr, resetTachos) != FME_SUCCESS)
   {
      return FME_FAILURE;
   }
   if (resetTachos)
   {
      ev3Brick->MotorA->ResetTacho();
      ev3Brick->MotorB->ResetTacho();
      ev3Brick->MotorC->ResetTacho();
      ev3Brick->MotorD->ResetTacho();
   }

   // Get our geometry and make sure it's a line
   const IFMEGeometry* geom = feature.getGeometry();
   if (geom == NULL || !geom->canCastAs<IFMELine*>())
   {
      gLogFile->logMessageString("Requires a line geometry", FME_ERROR);
      return FME_FAILURE;
   }

   // Get our geometry as a line
   const IFMELine* line = geom->castAs<const IFMELine*>();

   // Get the number of points
   const FME_UInt32 numPoints = line->numPoints();
   if (numPoints < 2)
   {
      gLogFile->logMessageString("Line must contain at least 2 points", FME_ERROR);
      return FME_FAILURE;
   }

   IFMEGeometryTools* geomTools = gFMESession->getGeometryTools();
   IFMEPoint* prevPoint = geomTools->createPoint();
   IFMEPoint* currPoint = geomTools->createPoint();

   FME_Real64 prevAngle = 0.0;
   FME_Real64 currAngle = 0.0;
   FME_Real64 turnAngle = 0.0;

   FME_Real64 length = 0.0;

   FME_UInt32 intLength = 0;
   FME_UInt32 intTurnAngle = 0;
   bool firstTime = true;

   // We know we have more than a straight line, so let's loop over the rest
   for (FME_UInt32 i = 1; i < numPoints; i++)
   {
      // Get our current point and the previous point
      line->getPointAt(i - 1, *prevPoint);
      line->getPointAt(i, *currPoint);

      if (firstTime)
      {
         // Our first time in, so let's initialize our previous angle
         prevAngle = calculateAngle(
            prevPoint->getX(), prevPoint->getY(),
            currPoint->getX(), currPoint->getY());
         firstTime = false;
      }

      // Get the angle of the current line segment relative to the horizontal (ccw is positive)
      currAngle = calculateAngle(
         prevPoint->getX(), prevPoint->getY(),
         currPoint->getX(), currPoint->getY());

      // Figure out how far we'll have to turn to match the angle of the current line segment
      turnAngle = currAngle - prevAngle;
      
      // Store our previous angle
      prevAngle = currAngle;

      // |  /
      // | /
      // |/
      // Going counter clockwise this will either have an angle over 180, or a negative angle
      // either way we will be turning to the right
      // 
      // \  |
      //  \ |
      //   \|
      // Going counter clockwise this will be a positive angle between 0 and 180
      // so we will be turning left
      if (turnAngle < -180.0)
      {
         turnAngle = 360.0 + turnAngle;
         intTurnAngle = static_cast<FME_UInt32>(turnAngle * rotationalScale);

         ev3Brick->Vehicle->SpinLeft(speed, intTurnAngle, false);
         LMFUtil::waitForMotorsToStop(ev3Brick, resetTachos);
      }
      else if (turnAngle < 0.0)
      {
         turnAngle = -1 * turnAngle;
         intTurnAngle = static_cast<FME_UInt32>(turnAngle * rotationalScale);

         ev3Brick->Vehicle->SpinRight(speed, intTurnAngle, false);
         LMFUtil::waitForMotorsToStop(ev3Brick, resetTachos);
      }
      else if (turnAngle > 180.0)
      {
         turnAngle = turnAngle - 180.0;
         intTurnAngle = static_cast<FME_UInt32>(turnAngle * rotationalScale);

         ev3Brick->Vehicle->SpinRight(speed, intTurnAngle, false);
         LMFUtil::waitForMotorsToStop(ev3Brick, resetTachos);
      }
      else if (turnAngle != 0.0)
      {
         intTurnAngle = static_cast<FME_UInt32>(turnAngle * rotationalScale);

         ev3Brick->Vehicle->SpinLeft(speed, intTurnAngle, false);
         LMFUtil::waitForMotorsToStop(ev3Brick, resetTachos);
      }

      // Now we're facing the correct direction, let's go!
      length = calculateDistance(
         prevPoint->getX(), prevPoint->getY(),
         currPoint->getX(), currPoint->getY());
      length = 360.0 * length * linearScale;
      intLength = static_cast<FME_UInt32>(length);

      ev3Brick->Vehicle->Forward(speed, intLength, false);
      LMFUtil::waitForMotorsToStop(ev3Brick, resetTachos);
   }

   // Done driving so turn off the motors
   ev3Brick->Vehicle->Off();

   // Cleanup
   geomTools->destroyGeometry(prevPoint); prevPoint = NULL;
   geomTools->destroyGeometry(currPoint); currPoint = NULL;


   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_Real64 LegoMindstormsPathFunc::calculateAngle(
   FME_Real64 originX, FME_Real64 originY,
   FME_Real64 pointX, FME_Real64 pointY) const
{
   FME_Real64 run = pointX - originX;
   FME_Real64 rise = pointY - originY;

   FME_Real64 angle = radiansToDegrees(atan2(rise,run));

   // We want the angle to be between 0 and 360, so we add 360 when necessary.
   if (angle < 0)
   {
      angle += 360.0;
   }

   return angle;
}

//------------------------------------------------------------------------------
FME_Real64 LegoMindstormsPathFunc::radiansToDegrees(FME_Real64 radians) const
{
   return ((radians * 180.0) / M_PI); 
}

//------------------------------------------------------------------------------
FME_Real64 LegoMindstormsPathFunc::calculateDistance(
   FME_Real64 x1, FME_Real64 y1,
   FME_Real64 x2, FME_Real64 y2) const
{
   return sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
}
