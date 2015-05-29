#ifndef LEGOMINDSTORMSFUNC_PATH_H
#define LEGOMINDSTORMSFUNC_PATH_H
/* =============================================================================

   Name     : legomindstormsfunc_path.h

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Declaration of LegoMindstormsPathFunc

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
class LegoMindstormsPathFunc
{
public:
   //---------------------------------------------------------------------------
   LegoMindstormsPathFunc(const string& functionName);

   //---------------------------------------------------------------------------
   FME_Status execute(
      IFMEFeature& feature,
      const IFMEStringArray& parameterList,
      Brick<Sensor^, Sensor^, Sensor^, Sensor^>^ ev3);

private:
   //---------------------------------------------------------------------------
   // Calculates the angle between an origin and some other point, in degrees
   // counterclockwise from horizontal.
   FME_Real64 calculateAngle(
      FME_Real64 originX, FME_Real64 originY,
      FME_Real64 pointX, FME_Real64 pointY) const;

   //---------------------------------------------------------------------------
   // Converts an angle in radians to degrees.
   FME_Real64 radiansToDegrees(FME_Real64 radians) const;

   //---------------------------------------------------------------------------
   // Calculates the distance between two points.
   FME_Real64 calculateDistance(
      FME_Real64 x1, FME_Real64 y1,
      FME_Real64 x2, FME_Real64 y2) const;

   //---------------------------------------------------------------------------
   // Private variables.

   // The name of the function.
   string functionName_;
};

#endif
