#ifndef LEGOMINDSTORMSFUNC_UTIL_H
#define LEGOMINDSTORMSFUNC_UTIL_H
/* =============================================================================

   Name     : legomindstormsfunc_util.h

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Utility functions for LegoMindstormsFunc

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

#include <string>
using std::string;

#using <System.dll>
using namespace System;

#using <MonoBrick.dll>
using namespace MonoBrick::EV3;

//==============================================================================
// Ways to specify a duration.
enum LMFDuration
{
   LMFDuration_Time,
   LMFDuration_Rotations
};

//==============================================================================
class LMFUtil
{
public:
   //---------------------------------------------------------------------------
   // Gets a standard string from a managed string.
   static string getString(System::String^ str);

   //---------------------------------------------------------------------------
   // Gets a motor port from a string.
   static bool stringToMotorPort(
      const string& portLetter,
      MotorPort% port);

   //---------------------------------------------------------------------------
   // Gets a duration from a string.
   static bool stringToDuration(
      const string& durationStr,
      LMFDuration& duration);

   //---------------------------------------------------------------------------
   // Waits until the motors on the brick stop.
   static void waitForMotorsToStop(
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^>^ brick,
      bool resetTachos);

   //---------------------------------------------------------------------------
   // Waits until a single motor stops.
   static void waitForMotorToStop(
      Motor^ motor,
      bool resetTachos);
};

#endif
