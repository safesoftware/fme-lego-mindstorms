/* =============================================================================

   Name     : legomindstormssensefunc.cpp

   System   : FME Plug-in SDK

   Language : C++

   Purpose  : Implementation of LegoMindstormsSenseFunc

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

#include "legomindstormsfunc_sense.h"
#include "legomindstormsfunc_priv.h"
#include "legomindstormsfunc_util.h"

#include <fmetypes.h>
#include <ifeature.h>
#include <ilogfile.h>
#include <fmestring.h>
#include <fmepluginutil.h>

#using <System.dll>
using namespace System;

#using <MonoBrick.dll>
using namespace MonoBrick::EV3;

//------------------------------------------------------------------------------
extern IFMELogFile* gLogFile;

//------------------------------------------------------------------------------
LegoMindstormsSenseFunc::LegoMindstormsSenseFunc(const string& functionName)
: functionName_(functionName)
{
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsSenseFunc::execute(
   IFMEFeature& feature,
   const IFMEStringArray& parameterList,
   Brick<Sensor^, Sensor^, Sensor^, Sensor^>^ ev3)
{
   FME_UInt32 nextParmIndex = 1; // Start at 1 since 0 was the mode (SENSE).

   // Set sensor attributes.
   FME_Status status = setSensorAttributes(parameterList, nextParmIndex, ev3, feature);
   if (status != FME_SUCCESS)
   {
      return FME_FAILURE;
   }

   // Set motor attributes.
   status = setMotorAttributes(parameterList, nextParmIndex, ev3, feature);
   if (status != FME_SUCCESS)
   {
      return FME_FAILURE;
   }

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsSenseFunc::setSensorAttributes(
   const IFMEStringArray& parameterList,
   FME_UInt32& nextParmIndex,
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
   IFMEFeature& feature) const
{
   // Get the sensor types.
   array<SensorType,1>^ sensorTypes = ev3->GetSensorTypes();

   // Loop over all the sensors.
   for (FME_UInt8 i=0; i < kLMF_NumSensors; ++i)
   {
      // Get the expected type for this sensor.
      LMFSensorType expectedSensorType;
      if (!getSensorType(parameterList, nextParmIndex, expectedSensorType))
      {
         return FME_FAILURE;
      }

      // Set attributes for the sensor.
      if (expectedSensorType != LMFSensorType_None)
      {
         // Set common attributes.
         FME_Status status = setCommonSensorAttributes(parameterList, nextParmIndex, ev3, i, feature);
         if (status != FME_SUCCESS)
         {
            return FME_FAILURE;
         }

         // Set more attributes depending on the type.
         switch (expectedSensorType)
         {
         case LMFSensorType_Color:
            status = setColorSensorAttributes(parameterList, nextParmIndex, ev3, i, sensorTypes[i], feature);
            break;
         case LMFSensorType_IR:
            status = setIRSensorAttributes(parameterList, nextParmIndex, ev3, i, sensorTypes[i], feature);
            break;
         case LMFSensorType_Touch:
            status = setTouchSensorAttributes(parameterList, nextParmIndex, ev3, i, sensorTypes[i], feature);
            break;
         default:
            return FME_FAILURE;
         }

         if (status != FME_SUCCESS)
         {
            return FME_FAILURE;
         }
      }
   }

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
bool LegoMindstormsSenseFunc::getSensorType(
   const IFMEStringArray& parameterList,
   FME_UInt32& nextParmIndex,
   LMFSensorType& sensorType) const
{
   if (nextParmIndex + 1 > parameterList.entries())
   {
      return false;
   }

   const string sensorTypeStr(FME_getString(*parameterList.elementAt(nextParmIndex++)));
   if (sensorTypeStr == kLMF_SensorTypeColor)
   {
      sensorType = LMFSensorType_Color;
   }
   else if (sensorTypeStr == kLMF_SensorTypeIR)
   {
      sensorType = LMFSensorType_IR;
   }
   else if (sensorTypeStr == kLMF_SensorTypeTouch)
   {
      sensorType = LMFSensorType_Touch;
   }
   else if (sensorTypeStr == kLMF_SensorTypeNone)
   {
      sensorType = LMFSensorType_None;
   }
   else
   {
      return false;
   }

   return true;
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsSenseFunc::setCommonSensorAttributes(
   const IFMEStringArray& parameterList,
   FME_UInt32& nextParmIndex,
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
   FME_UInt8 sensorIndex,
   IFMEFeature& feature) const
{
   // Get the attribute names.
   FMEStringArray attrNames;
   if (!getAttributeNames(parameterList, nextParmIndex, kLMF_SensorNumAttrsCommon, attrNames))
   {
      return FME_FAILURE;
   }
   const IFMEString& sensorNameAttr   = *attrNames->elementAt(0);
   const IFMEString& sensorSymbolAttr = *attrNames->elementAt(1);

   // Get the sensor.
   Sensor^ sensor;
   switch (sensorIndex)
   {
   case 0:  sensor = ev3->Sensor1; break;
   case 1:  sensor = ev3->Sensor2; break;
   case 2:  sensor = ev3->Sensor3; break;
   case 3:  sensor = ev3->Sensor4; break;
   default: return FME_FAILURE;
   }

   // Set the sensor name.
   if (sensorNameAttr.length() > 0)
   {
      feature.setAttribute(sensorNameAttr, LMFUtil::getString(sensor->GetName()).c_str());
   }

   // Set the sensor symbol.
   if (sensorSymbolAttr.length() > 0)
   {
      feature.setAttribute(sensorSymbolAttr, LMFUtil::getString(sensor->GetSymbole()).c_str());
   }

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsSenseFunc::setColorSensorAttributes(
   const IFMEStringArray& parameterList,
   FME_UInt32& nextParmIndex,
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
   FME_UInt8 sensorIndex,
   SensorType sensorType,
   IFMEFeature& feature) const
{
   // Get the attribute names.
   FMEStringArray attrNames;
   if (!getAttributeNames(parameterList, nextParmIndex, kLMF_SensorNumAttrsColor, attrNames))
   {
      return FME_FAILURE;
   }
   const IFMEString& colorAttr                   = *attrNames->elementAt(0);
   const IFMEString& reflectedLightIntensityAttr = *attrNames->elementAt(1);
   const IFMEString& ambientLightIntensityAttr   = *attrNames->elementAt(2);

   // Ensure this is the right type.
   if (sensorType != SensorType::Color)
   {
      string msg = functionName_ + ": Type for sensor " + FME_uintToString(sensorIndex) + " is incorrect.";
      gLogFile->logMessageString(msg.c_str(), FME_WARN);
      return FME_SUCCESS;
   }

   // Create and attach a color sensor.
   ColorSensor^ colorSensor = gcnew ColorSensor();
   attachSensor(ev3, sensorIndex, colorSensor);

   // Set the color attribute.
   if (colorAttr.length() > 0)
   {
      colorSensor->Mode = ColorMode::Color;
      Color color = colorSensor->ReadColor();
      const string colorStr(getFMEColor(color));
      if (!colorStr.empty())
      {
         feature.setAttribute(colorAttr, colorStr.c_str());
      }
   }

   // Set the reflected light intensity attribute.
   setSensorIntAttribute(colorSensor, ColorMode::Reflection, reflectedLightIntensityAttr, feature);

   // Set the ambient light intensity attribute.
   setSensorIntAttribute(colorSensor, ColorMode::Ambient, ambientLightIntensityAttr, feature);

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
bool LegoMindstormsSenseFunc::getAttributeNames(
   const IFMEStringArray& parameterList,
   FME_UInt32& nextParmIndex,
   FME_UInt32 numAttrs,
   IFMEStringArray& attrNames) const
{
   for (FME_UInt32 i=0; i < numAttrs; ++i)
   {
      FMEString attrName;
      if (!getAttributeName(parameterList, nextParmIndex++, *attrName))
      {
         return false;
      }
      attrNames.append(*attrName);
   }

   return true;
}

//------------------------------------------------------------------------------
bool LegoMindstormsSenseFunc::getAttributeName(
   const IFMEStringArray& parameterList,
   FME_UInt32 parmIndex,
   IFMEString& attrName) const
{
   if (!getParameter(parameterList, parmIndex, attrName))
   {
      return false;
   }

   attrName.decodeFromFMEParsableText();

   return true;
}

//------------------------------------------------------------------------------
bool LegoMindstormsSenseFunc::getParameter(
   const IFMEStringArray& parameterList,
   FME_UInt32 parmIndex,
   IFMEString& parmVal) const
{
   if (parmIndex >= parameterList.entries())
   {
      return false;
   }

   const IFMEString& sourceStr = *parameterList.elementAt(parmIndex);
   parmVal.set(sourceStr.data(), sourceStr.length());

   return true;
}

//------------------------------------------------------------------------------
void LegoMindstormsSenseFunc::attachSensor(
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
   FME_UInt8 sensorIndex,
   Sensor^ sensor) const
{
   switch (sensorIndex)
   {
   case 0: ev3->Sensor1 = sensor; break;
   case 1: ev3->Sensor2 = sensor; break;
   case 2: ev3->Sensor3 = sensor; break;
   case 3: ev3->Sensor4 = sensor; break;
   }
}

//------------------------------------------------------------------------------
string LegoMindstormsSenseFunc::getFMEColor(Color color) const
{
   switch (color)
   {
   case Color::Black:   return "0,0,0";
   case Color::Blue:    return "0,0,1";
   case Color::Green:   return "0,1,0";
   case Color::Yellow:  return "0,1,1";
   case Color::Red:     return "1,0,0";
   case Color::White:   return "1,1,1";
   case Color::Brown:   return "0.64,0.16,0.16";
   case Color::None:    return "";
   default:             return "";
   }
}

//------------------------------------------------------------------------------
template <class SensorClass, typename SensorClassMode>
void LegoMindstormsSenseFunc::setSensorIntAttribute(
   SensorClass^ sensor,
   SensorClassMode mode,
   const IFMEString& attrName,
   IFMEFeature& feature) const
{
   if (attrName.length() > 0)
   {
      sensor->Mode = mode;
      feature.setAttribute(attrName, static_cast<FME_Int32>(sensor->Read()));
   }
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsSenseFunc::setIRSensorAttributes(
   const IFMEStringArray& parameterList,
   FME_UInt32& nextParmIndex,
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
   FME_UInt8 sensorIndex,
   SensorType sensorType,
   IFMEFeature& feature) const
{
   // Get the attribute names.
   FMEStringArray attrNames;
   if (!getAttributeNames(parameterList, nextParmIndex, kLMF_SensorNumAttrsIR, attrNames))
   {
      return FME_FAILURE;
   }
   const IFMEString& proximityAttr       = *attrNames->elementAt(0);
   const IFMEString& beaconProximityAttr = *attrNames->elementAt(1);
   const IFMEString& remoteButtonAttr    = *attrNames->elementAt(2);

   // Ensure this is the right type.
   if (sensorType != SensorType::IR)
   {
      string msg = functionName_ + ": Type for sensor " + FME_uintToString(sensorIndex) + " is incorrect.";
      gLogFile->logMessageString(msg.c_str(), FME_WARN);
      return FME_SUCCESS;
   }

   // Create and attach an IR sensor.
   IRSensor^ irSensor = gcnew IRSensor();
   attachSensor(ev3, sensorIndex, irSensor);

   // Set the proximity attribute.
   if (proximityAttr.length() > 0)
   {
      irSensor->Mode = IRMode::Proximity;

      // The proximity sensor is a bit flaky, so we'll try reading it a few
      // times to get a valid value.
      FME_Int32 proximityVal = kLMF_InvalidProximityValue;
      for (FME_UInt8 i=0; i < kLMF_MaxProximitySensorReads && proximityVal == kLMF_InvalidProximityValue; ++i)
      {
         proximityVal = static_cast<FME_Int32>(irSensor->Read());
      }

      feature.setAttribute(proximityAttr, proximityVal);
   }

   // Set other attributes.
   setSensorIntAttribute(irSensor, IRMode::Seek, beaconProximityAttr, feature);
   setSensorIntAttribute(irSensor, IRMode::Remote, remoteButtonAttr, feature);

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsSenseFunc::setTouchSensorAttributes(
   const IFMEStringArray& parameterList,
   FME_UInt32& nextParmIndex,
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
   FME_UInt8 sensorIndex,
   SensorType sensorType,
   IFMEFeature& feature) const
{
   // Get the attribute names.
   FMEStringArray attrNames;
   if (!getAttributeNames(parameterList, nextParmIndex, kLMF_SensorNumAttrsTouch, attrNames))
   {
      return FME_FAILURE;
   }
   const IFMEString& pressedAttr = *attrNames->elementAt(0);

   // Ensure this is the right type.
   if (sensorType != SensorType::Touch)
   {
      string msg = functionName_ + ": Type for sensor " + FME_uintToString(sensorIndex) + " is incorrect.";
      gLogFile->logMessageString(msg.c_str(), FME_WARN);
      return FME_SUCCESS;
   }

   // Create and attach a touch sensor.
   TouchSensor^ touchSensor = gcnew TouchSensor();
   attachSensor(ev3, sensorIndex, touchSensor);

   // Set the attributes.
   if (pressedAttr.length() > 0)
   {
      feature.setBooleanAttribute(pressedAttr, touchSensor->Read() ? FME_TRUE : FME_FALSE);
   }

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsSenseFunc::setMotorAttributes(
   const IFMEStringArray& parameterList,
   FME_UInt32& nextParmIndex,
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
   IFMEFeature& feature) const
{
   for (FME_UInt8 i=0; i < kLMF_NumMotors; ++i)
   {
      FME_Status status = setSingleMotorAttributes(parameterList, nextParmIndex, ev3, i, feature);
      if (status != FME_SUCCESS)
      {
         return FME_FAILURE;
      }
   }

   return FME_SUCCESS;
}

//------------------------------------------------------------------------------
FME_Status LegoMindstormsSenseFunc::setSingleMotorAttributes(
   const IFMEStringArray& parameterList,
   FME_UInt32& nextParmIndex,
   Brick<Sensor ^, Sensor ^, Sensor ^, Sensor ^> ^ ev3,
   FME_UInt8 motorIndex,
   IFMEFeature& feature) const
{
   // Figure out whether to do anything for this motor.
   FMEString processMotorStr;
   bool processMotor;
   if (!getParameter(parameterList, nextParmIndex++, *processMotorStr) ||
       FME_stringToBool(FME_getString(*processMotorStr), processMotor) != FME_SUCCESS)
   {
      return FME_FAILURE;
   }

   // If we don't want to do anything, we're done.
   if (!processMotor)
   {
      return FME_SUCCESS;
   }

   // Get the attribute names.
   FMEStringArray attrNames;
   if (!getAttributeNames(parameterList, nextParmIndex, kLMF_MotorNumAttrs, attrNames))
   {
      return FME_FAILURE;
   }
   const IFMEString& tachoAttr   = *attrNames->elementAt(0);
   const IFMEString& runningAttr = *attrNames->elementAt(1);
   const IFMEString& speedAttr   = *attrNames->elementAt(2);

   // Get whether to reset the tachometer.
   FMEString resetTachoStr;
   bool resetTacho;
   if (!getParameter(parameterList, nextParmIndex++, *resetTachoStr) ||
       FME_stringToBool(FME_getString(*resetTachoStr), resetTacho) != FME_SUCCESS)
   {
      return FME_FAILURE;
   }

   // Get the motor.
   Motor^ motor;
   switch (motorIndex)
   {
   case 0:  motor = ev3->MotorA; break;
   case 1:  motor = ev3->MotorB; break;
   case 2:  motor = ev3->MotorC; break;
   case 3:  motor = ev3->MotorD; break;
   default: return FME_FAILURE;
   }

   // Set the tachometer count.
   if (tachoAttr.length() > 0)
   {
      feature.setAttribute(tachoAttr, static_cast<FME_Int32>(motor->GetTachoCount()));
   }

   // Set the running attribute.
   if (runningAttr.length() > 0)
   {
      feature.setBooleanAttribute(runningAttr, motor->IsRunning() ? FME_TRUE : FME_FALSE);
   }

   // Set the speed attribute.
   if (speedAttr.length() > 0)
   {
      feature.setInt8Attribute(speedAttr, static_cast<FME_Int8>(motor->GetSpeed()));
   }

   // Reset the tachometer if requested.
   if (resetTacho)
   {
      motor->ResetTacho();
   }

   return FME_SUCCESS;
}
