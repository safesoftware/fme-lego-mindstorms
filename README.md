# fme-lego-mindstorms
LEGO Mindstorms plug-in for FME

The plugin is built with managed C++/CLI and it uses FME's C++ API together with the C# [MonoBrick Communication Library](http://www.monobrick.dk/software/monobrick/)

With this plugin, we created Transformers.

1. LegoMindstormsMover
  * Moves the robot (forward / turn / spin)
  * Based on [EV3RSTORMs](http://www.lego.com/en-us/mindstorms/build-a-robot/ev3rstorm) build (i.e. 2 large motors together as a "vehicle")

2. LegoMindstormsPropertyExtractor
  * Takes readings from sensors and motors
    * IR, Color, Touch, Motor Tachometers

3. LegoMindstormsPathTraverser
  * Follow's a line geometry

4. LegoMindstormsMotorMover
  * Activates a single motor (specified in Transformer parameters)

5. LegoMindstormsSoundPlayer (incomplete)
  * Plays a sound stored on the EV3 Brick.
