#ifndef LEGOMINDSTORMSFUNC_MOVE_H
#define LEGOMINDSTORMSFUNC_MOVE_H
/* =============================================================================

   Name     : legomindstormsfunc_move.h

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Declaration of LegoMindstormsMoveFunc

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

#include <fmetypes.h>

#include <string>
using std::string;

#using <MonoBrick.dll>
using namespace MonoBrick::EV3;

class IFMEFeature;
class IFMEStringArray;

//==============================================================================
class LegoMindstormsMoveFunc
{
public:
   //---------------------------------------------------------------------------
   LegoMindstormsMoveFunc(const string& functionName);

   //---------------------------------------------------------------------------
   FME_Status execute(
      IFMEFeature& feature,
      const IFMEStringArray& parameterList,
      Brick<Sensor^, Sensor^, Sensor^, Sensor^>^ ev3Brick);

private:
   //---------------------------------------------------------------------------
   // Directions.
   enum LMFDirection
   {
      LMFDirection_Left,
      LMFDirection_Right
   };

   //---------------------------------------------------------------------------
   // Gets a direction type from a string.
   // Returns true if the string was valid.
   bool getDirectionTypeFromString(
      const string& directionStr,
      LMFDirection& direction) const;

   //---------------------------------------------------------------------------
   // Various ways to move.
   void vehicleStraightTime(
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
      FME_Int8 speed, 
      FME_Int32 seconds) const;

   void vehicleStraightRotations(
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
      FME_Int8 speed, 
      FME_Int32 degrees) const;

   void vehicleTurnTime(
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
      FME_Int8 speed, 
      LMFDirection direction, 
      FME_Int8 percentage, 
      FME_Int32 seconds) const;

   void vehicleTurnRotations(
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
      FME_Int8 speed, 
      LMFDirection direction, 
      FME_Int8 percentage,
      FME_Int32 degrees) const;

   void vehicleSpinTime(
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
      FME_Int8 speed, 
      LMFDirection direction,
      FME_Int32 seconds) const;

   void vehicleSpinRotations(
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ev3Brick, 
      FME_Int8 speed, 
      LMFDirection direction, 
      FME_Int32 degrees) const;

   //---------------------------------------------------------------------------
   // Private variables.

   // The name of the function.
   string functionName_;
};

#endif
