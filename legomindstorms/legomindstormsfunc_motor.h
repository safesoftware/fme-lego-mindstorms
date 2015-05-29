#ifndef LEGOMINDSTORMSFUNC_MOTOR_H
#define LEGOMINDSTORMSFUNC_MOTOR_H
/* =============================================================================

   Name     : legomindstormsfunc_motor.h

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Declaration of LegoMindstormsMotorFunc

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
class LegoMindstormsMotorFunc
{
public:
   //---------------------------------------------------------------------------
   LegoMindstormsMotorFunc(const string& functionName);

   //---------------------------------------------------------------------------
   FME_Status execute(
      IFMEFeature& feature,
      const IFMEStringArray& parameterList,
      Brick<Sensor^, Sensor^, Sensor^, Sensor^>^ ev3Brick);

private:
   //---------------------------------------------------------------------------
   FME_Status runOnTime(
      Motor ^motor, 
      FME_Int8 speed, 
      FME_Int32 seconds);

   //---------------------------------------------------------------------------
   FME_Status runOnRotations(
      Motor ^motor, 
      FME_Int8 speed, 
      FME_Int32 degrees);

   //---------------------------------------------------------------------------
   // Private variables.

   // The name of the function.
   string functionName_;
};

#endif
