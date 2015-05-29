#ifndef LEGOMINDSTORMSFUNC_H
#define LEGOMINDSTORMSFUNC_H
/* =============================================================================

   Name     : legomindstormsfunc.h

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Declaration of LegoMindstormsFunc

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


   Package Overview
   ================

   This defines an FME function, @LegoMindstorms, that controls a LEGO
   Mindstorms EV3.

   In order to build this function, copy the directory containing this code to
   <FME_HOME>\pluginbuilder\samples\cpp\code\FMEFunction

   Additionally, you will need a copy of the MonoBrick Communication
   Library (available from http://www.monobrick.dk/software/monobrick/).
   MonoBrick.dll needs to be accessible both during compilation (e.g. you may
   want to copy it to the same directory as the code), as well as during
   runtime (e.g. you may want to copy it to <FME_HOME>).

   This function has a few different modes:


   MOVE
   ----

   This mode performs a simple movement of the vehicle.

   Here are the motor parameters:

   @LegoMindstorms(MOVE, <LEFT_MOTOR_NAME>, <RIGHT_MOTOR_NAME>, <REVERSE_LEFT>, <REVERSE_RIGHT>, [described below])

   LEFT_MOTOR_NAME and RIGHT_MOTOR_NAME are the motors that drive the vehicle.
   The values must be A, B, C, or D.

   REVERSE_LEFT and REVERSE_RIGHT specify whether the motors have been reversed.

   This function takes additional parameters that describe the movement type.

   @LegoMindstorms(MOVE, [motor parameters], STRAIGHT, <SPEED>, <DURATION_TYPE>, <DURATION>)
   @LegoMindstorms(MOVE, [motor parameters], TURN, <SPEED>, <DIRECTION>, <TURN_RATIO>, <DURATION_TYPE>, <DURATION>)
   @LegoMindstorms(MOVE, [motor parameters], SPIN, <SPEED>, <DIRECTION>, <DURATION_TYPE>, <DURATION>)

   The STRAIGHT motion moves the vehicle straight.
   The TURN motion rotates the vehicle while moving forward.
   The SPIN motion rotates the vehicle in place.
   
   The SPEED parameter is a value from -100 to 100 that specifies how fast the
   vehicle should move.

   The DURATION_TYPE specifies how the DURATION parameter will be interpreted.
   When DURATION_TYPE is TIME, DURATION specifies the number of seconds the
   motion will occur for. When DURATION_TYPE is ROTATIONS, DURATION specifies
   the number of rotations of the motor rotations will occur for.

   The DIRECTION parameter specifies whether a rotation will be LEFT or RIGHT.

   The TURN_RATIO specifies the degree of rotation.


   MOTOR
   -----

   This mode performs a simple movement of a single motor (not the whole vehicle).

   @LegoMindstorms(MOTOR, <MOTOR_NAME>, <SPEED>, <DURATION_TYPE>, <DURATION>)

   All parameters were described above.


   SENSE
   -----

   This mode queries the sensors and motors to get the current state of the EV3.

   The basic format of the function is

   @LegoMindstorms(SENSE,[SENSOR1_ARGS],[SENSOR2_ARGS],[SENSOR3_ARGS],[SENSOR4_ARGS],[MOTORA_ARGS],[MOTORB_ARGS],[MOTORC_ARGS],[MOTORD_ARGS])

   Each SENSOR_ARGS entry indicates the type of the sensor attached to that port,
   as well as the attribute names that should be used for recording the values.

   The format of each SENSOR_ARGS entry is one of the following:

   Color, <SENSOR_NAME_ATTR>, <SENSOR_SYMBOL_ATTR>, <SENSOR_COLOR_ATTR>, <SENSOR_REFLECTED_LIGHT_ATTR>, <SENSOR_AMBIENT_LIGHT_ATTR>
   IR, <SENSOR_NAME_ATTR>, <SENSOR_SYMBOL_ATTR>, <PROXIMITY_ATTR>, <BEACON_PROXIMITY_ATTR>, <REMOTE_ATTR>
   Touch, <SENSOR_NAME_ATTR>, <SENSOR_SYMBOL_ATTR>, <PRESSED_ATTR>
   None

   Each MOTOR_ARGS entry indicates whether a motor is attached to that port, as
   well as the attribute names that should be used for recording the values.

   The format of each MOTOR_ARGS entry is one of the following:

   Yes, <TACHO_ATTR>, <IS_RUNNING_ATTR>, <SPEED_ATTR>, <RESET_TACHO>
   No

   The RESET_TACHO parameter specifies whether to reset tachometer readings
   after taking the value.


   PATH
   ----

   This mode moves the vehicle along a path specified by the geometry of the
   feature.

   The format of this function is

   @LegoMindstorms(PATH, [motor parameters], <SPEED>, <LINEAR_SCALE>, <ROTATIONAL_SCALE>, <RESET_TACHOS>)

   the motor parameters are identical to those for MOVE mode.

   The LINEAR_SCALE parameter is applied to segment lengths to scale them down
   to reasonable distances for the EV3 to move. Similarly, the ROTATIONAL_SCALE
   parameter is applied to rotation angles.

   The RESET_TACHOS parameter specifies whether to reset tachometer readings
   after each movement. Doing so may improve the accuracy of path traversal.

============================================================================= */

#include "legomindstormsfunc_priv.h"
#include <fmefunc.h>
#include <string>

//==============================================================================
class LegoMindstormsFunc : public IFMEFunction
{
public:
   //---------------------------------------------------------------------------
   LegoMindstormsFunc(const char* functionName);

   //---------------------------------------------------------------------------
   virtual ~LegoMindstormsFunc();

   //---------------------------------------------------------------------------
   virtual FME_MsgNum initialize();

   //---------------------------------------------------------------------------
   virtual FME_MsgNum abort();

   //---------------------------------------------------------------------------
   virtual FME_MsgNum close();

   //---------------------------------------------------------------------------
   virtual FME_UInt32 id() { return kLegoMindstormsFunc_Id; }

   //---------------------------------------------------------------------------
   virtual FME_MsgNum execute(IFMEFeature& feature,
                              const IFMEStringArray& parameterList,
                              IFMEString& returnValue);

   //---------------------------------------------------------------------------
   virtual FME_MsgNum inverse(IFMEFeature& feature,
                              const IFMEStringArray& parameterList,
                              IFMEString& returnValue);

private:
   //---------------------------------------------------------------------------
   // Private variables.

   // The name of the function.
   std::string functionName_;
};

#endif
