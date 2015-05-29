/* =============================================================================

   Name     : legomindstormsfunc_util.cpp

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

#include "legomindstormsfunc_util.h"
#include "legomindstormsfunc_priv.h"

#using <System.dll>
using namespace System;
using namespace System::Runtime::InteropServices;

#using <MonoBrick.dll>
using namespace MonoBrick::EV3;

//------------------------------------------------------------------------------
string LMFUtil::getString(System::String^ str)
{
   char* temp = reinterpret_cast<char*>(Marshal::StringToHGlobalAnsi(str).ToPointer());
   string outputStr(temp);
   Marshal::FreeHGlobal((IntPtr)temp);
   return outputStr;
}

//------------------------------------------------------------------------------
bool LMFUtil::stringToMotorPort(
   const string& portLetter,
   MotorPort% port)
{
   if (portLetter == "A")
   {
      port = MotorPort::OutA;
   }
   else if (portLetter == "B")
   {
      port = MotorPort::OutB;
   }
   else if (portLetter == "C")
   {
      port = MotorPort::OutC;
   }
   else if (portLetter == "D")
   {
      port = MotorPort::OutD;
   }
   else
   {
      return false;
   }

   return true;
}

//------------------------------------------------------------------------------
bool LMFUtil::stringToDuration(
   const string& durationStr,
   LMFDuration& duration)
{
   if (durationStr == kLMF_durationTypeTime)
   {
      duration = LMFDuration_Time;
   }
   else if (durationStr == kLMF_durationTypeRotation)
   {
      duration = LMFDuration_Rotations;
   }
   else
   {
      return false;
   }

   return true;
}

//------------------------------------------------------------------------------
void LMFUtil::waitForMotorsToStop(
   Brick<Sensor^, Sensor^, Sensor^, Sensor^>^ brick,
   bool resetTachos)
{
   //Thread::Sleep(500);
   _sleep(500);
   while (brick->MotorA->IsRunning() || brick->MotorB->IsRunning() || brick->MotorC->IsRunning() || brick->MotorD->IsRunning())
      _sleep(50);
   // Reset all motor tachos
   if (resetTachos)
   {
      brick->MotorA->ResetTacho();
      brick->MotorB->ResetTacho();
      brick->MotorC->ResetTacho();
      brick->MotorD->ResetTacho();
   }
}
//------------------------------------------------------------------------------
void LMFUtil::waitForMotorToStop(
   Motor^ motor,
   bool resetTachos)
{
   //Thread::Sleep(500);
   _sleep(500);
   while (motor->IsRunning())
      _sleep(50);
   // Reset all motor tachos
   if (resetTachos)
   {
      motor->ResetTacho();
   }
}
