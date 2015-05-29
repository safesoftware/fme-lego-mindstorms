#ifndef LEGOMINDSTORMSFUNC_SENSE_H
#define LEGOMINDSTORMSFUNC_SENSE_H
/* =============================================================================

   Name     : legomindstormsfunc_sense.h

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Declaration of LegoMindstormsSenseFunc

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
class LegoMindstormsSenseFunc
{
public:
   //---------------------------------------------------------------------------
   LegoMindstormsSenseFunc(const string& functionName);

   //---------------------------------------------------------------------------
   FME_Status execute(
      IFMEFeature& feature,
      const IFMEStringArray& parameterList,
      Brick<Sensor^, Sensor^, Sensor^, Sensor^>^ ev3);

private:
   //---------------------------------------------------------------------------
   // The various sensor types we support.
   enum LMFSensorType
   {
      LMFSensorType_Color,
      LMFSensorType_IR,
      LMFSensorType_Touch,
      LMFSensorType_None
   };

   //---------------------------------------------------------------------------
   // Sets sensor attributes.
   FME_Status setSensorAttributes(
      const IFMEStringArray& parameterList,
      FME_UInt32& nextParmIndex,
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
      IFMEFeature& feature) const;

   //---------------------------------------------------------------------------
   // Gets the sensor type from the parameter list.
   // Returns false on error.
   bool getSensorType(
      const IFMEStringArray& parameterList,
      FME_UInt32& nextParmIndex,
      LMFSensorType& sensorType) const;

   //---------------------------------------------------------------------------
   // Sets attributes that are common for all sensor types.
   FME_Status setCommonSensorAttributes(
      const IFMEStringArray& parameterList,
      FME_UInt32& nextParmIndex,
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
      FME_UInt8 sensorIndex,
      IFMEFeature& feature) const;

   //---------------------------------------------------------------------------
   // Sets color attributes on a feature.
   FME_Status setColorSensorAttributes(
      const IFMEStringArray& parameterList,
      FME_UInt32& nextParmIndex,
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
      FME_UInt8 sensorIndex,
      SensorType sensorType,
      IFMEFeature& feature) const;

   //---------------------------------------------------------------------------
   // Gets a few attribute names from a parameter list.
   bool getAttributeNames(
      const IFMEStringArray& parameterList,
      FME_UInt32& nextParmIndex,
      FME_UInt32 numAttrs,
      IFMEStringArray& attrNames) const;

   //---------------------------------------------------------------------------
   // Gets an attribute name from a parameter list.
   bool getAttributeName(
      const IFMEStringArray& parameterList,
      FME_UInt32 parmIndex,
      IFMEString& attrName) const;

   //---------------------------------------------------------------------------
   // Gets a value from the parameter list.
   bool getParameter(
      const IFMEStringArray& parameterList,
      FME_UInt32 parmIndex,
      IFMEString& parmVal) const;

   //---------------------------------------------------------------------------
   // Attaches a sensor to the brick.
   void attachSensor(
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
      FME_UInt8 sensorIndex,
      Sensor^ sensor) const;

   //---------------------------------------------------------------------------
   // Gets an fme_color for a color enumerated value.
   string getFMEColor(Color color) const;

   //---------------------------------------------------------------------------
   // Sets a sensor attribute.
   template <class SensorClass, typename SensorClassMode>
   void setSensorIntAttribute(
      SensorClass^ sensor,
      SensorClassMode mode,
      const IFMEString& attrName,
      IFMEFeature& feature) const;

   //---------------------------------------------------------------------------
   // Sets IR attributes on a feature.
   FME_Status setIRSensorAttributes(
      const IFMEStringArray& parameterList,
      FME_UInt32& nextParmIndex,
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
      FME_UInt8 sensorIndex,
      SensorType sensorType,
      IFMEFeature& feature) const;

   //---------------------------------------------------------------------------
   // Sets touch attributes on a feature.
   FME_Status setTouchSensorAttributes(
      const IFMEStringArray& parameterList,
      FME_UInt32& nextParmIndex,
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
      FME_UInt8 sensorIndex,
      SensorType sensorType,
      IFMEFeature& feature) const;

   //---------------------------------------------------------------------------
   // Sets motor attributes.
   FME_Status setMotorAttributes(
      const IFMEStringArray& parameterList,
      FME_UInt32& nextParmIndex,
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
      IFMEFeature& feature) const;

   //---------------------------------------------------------------------------
   // Sets motor attributes for a single motor.
   FME_Status setSingleMotorAttributes(
      const IFMEStringArray& parameterList,
      FME_UInt32& nextParmIndex,
      Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
      FME_UInt8 motorIndex,
      IFMEFeature& feature) const;

   //---------------------------------------------------------------------------
   // Private variables.

   // The name of the function.
   string functionName_;
};

#endif
