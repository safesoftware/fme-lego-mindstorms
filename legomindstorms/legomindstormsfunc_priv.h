#ifndef LEGOMINDSTORMSFUNC_PRIV_H
#define LEGOMINDSTORMSFUNC_PRIV_H
/* =============================================================================

   Name     : legomindstormsfunc_priv.h

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : LegoMindstormsFunc constants

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

//------------------------------------------------------------------------------

// The mode used to connect to the EV3 brick.
static const char* kLMF_ConnectionType = "usb";

// Function ID
static const FME_UInt32 kLegoMindstormsFunc_Id = 0;

// Function modes.
static const string kLMF_ModeMove = "MOVE";
static const string kLMF_ModeSense = "SENSE";
static const string kLMF_ModePath = "PATH";
static const string kLMF_ModeMotor = "MOTOR";

static const string kLMF_MoveTypeStraight = "STRAIGHT";
static const string kLMF_MoveTypeTurn = "TURN";
static const string kLMF_MoveTypeSpin = "SPIN";

static const string kLMF_Left = "LEFT";
static const string kLMF_Right = "RIGHT";

static const string kLMF_durationTypeTime = "TIME";
static const string kLMF_durationTypeRotation = "ROTATIONS";

static const FME_UInt16 kLMF_degreesPerRotation = 360;
static const FME_UInt16 kLMF_milliSecsPerSecond = 1000;

// The number of sensors.
static const FME_UInt8 kLMF_NumSensors = 4;

// Sensor types.
static const string kLMF_SensorTypeColor = "Color";
static const string kLMF_SensorTypeIR = "IR";
static const string kLMF_SensorTypeTouch = "Touch";
static const string kLMF_SensorTypeNone = "None";

// The number of attributes we expect for each sensor type.
static const FME_UInt32 kLMF_SensorNumAttrsCommon = 2;
static const FME_UInt32 kLMF_SensorNumAttrsColor = 3;
static const FME_UInt32 kLMF_SensorNumAttrsIR = 3;
static const FME_UInt32 kLMF_SensorNumAttrsTouch = 1;

// Reading values from the proximity sensor seems to be a bit flaky,
// so we'll try a handful of times before giving up.
static const FME_UInt8 kLMF_MaxProximitySensorReads = 50;
static const FME_Int32 kLMF_InvalidProximityValue = 128;

// The number of motors.
static const FME_UInt8 kLMF_NumMotors = 4;

// The number of attributes we expect for a motor.
static const FME_UInt32 kLMF_MotorNumAttrs = 3;

#endif
