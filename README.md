# fme-lego-mindstorms
LEGO Mindstorms plug-in for FME

This is a sample plug-in that uses the [FME C++ pluginbuilder API](https://knowledge.safe.com/articles/How_To/Developing-a-new-transformer-using-the-FME-Plug-in-SDK) together with the [C# MonoBrick Communication Library](http://www.monobrick.dk/software/monobrick/) to [control a LEGO Mindstorms EV3 robot](https://vine.co/v/eZTlXr17nEP).


Steps for using this plug-in:

1. Copy the directory containing the code to `<FME_HOME>\pluginbuilder\samples\cpp\code\FMEFunction`

2. Obtain a copy of the [MonoBrick Communication Library](http://www.monobrick.dk/software/monobrick/). `MonoBrick.dll` needs to be accessible both during compilation (e.g. you may want to copy it to the same directory as the code), as well as during runtime (e.g. you may want to copy it to `<FME_HOME>`).

3. Change the constant `kLMF_ConnectionType` defined in `legomindstormsfunc_priv.h`, depending on how you will be connecting to the robot

4. Build the plug-in

5. Copy the transformer files to `<FME_HOME>\transformers`


This project contains the following transformers:

1. LegoMindstormsVehicleMover
  * Moves the robot as a vehicle (forward / turn / spin)
  * Based on [EV3RSTORMs](http://www.lego.com/en-us/mindstorms/build-a-robot/ev3rstorm) build (i.e. 2 large motors together as a "vehicle")

2. LegoMindstormsPropertyExtractor
  * Takes readings from sensors and motors
    * IR, Color, Touch, Motor Tachometers

3. LegoMindstormsPathTraverser
  * Traverses a path defined by a line geometry

4. LegoMindstormsMotorMover
  * Moves a single motor (specified in Transformer parameters)


In addition, a couple sample workspaces are included:

1. RoombaSimulator.fmw
  * Demonstrates the LegoMindstormsVehicleMover and LegoMindstormsPropertyExtractor transformers
  * Moves the robot forward until it gets close to an obstacle, then spins in a random direction

2. PathTraverser.fmw
  * Demonstrates the LegoMindstormsPathTraverser transformer
