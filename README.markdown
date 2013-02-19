Atmospheric Scattering
======================

Simulates atmospheric scattering from the perspective of a camera on the
surface of an Earth-like planet.

Requirements:
-------------
* Windows 8
* Visual Studio 2012

To build the project, open `Scattering.sln` and run on Debug settings for x64.
The program has not been tested in Release mode, and a known issue prevents it
from closing when the window has been destroyed.

You will also need to edit the ScatteringWindow project properties, so that is
working directory while executing is $(OutDir).
