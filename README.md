# Solar System Simulation in C
This project was provided to us by our institution, Indian Institute of Technology, Mandi.

## Instructions
* Clone this repository or download its `.zip`
* Go to the directory and open a terminal.
* Create an input file say `input.txt` and add the details of the bodies as the given format.
* Build the program by `make`.
* Run the program by `./body input.txt` and wait.
* When done, open `Octave` or `MATLAB`(untested) and type and run `plot_all` and you will obtain the trajectory plots of the bodies.

### Format for Input file
----

[Number of Bodies]

[Mass1] [Pos1 X] [Pos1 Y] [Pos1 Z] [Vel1 X] [Vel1 Y] [Vel1 Z]

[Mass2] [Pos2 X] [Pos2 Y] [Pos2 Z] [Vel2 X] [Vel2 Y] [Vel2 Z]

...

[Duration]

[Time Step]

Mass in kg, Distance in metres, and Time in seconds.

----

Example, for Sun-Earth System-

2

1.989e30 0 0 0 0 0 0

5.972e24 1.496e9 0 0 0 30000 0

31557600

10

----

Created by Utkarsh Kunwar, and Munish Kumar.
