# Compilation, Setup and Running.
Ensure you are in the correct base directory.
To compile run the following commands:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

The compiled program will be `build/453-skeleton.exe` for windows OS.

# Problems encountered
on part 4 of the assignment,Refraction could not be implemented as there is no
transparency value which can be used to modulate the refractive color or determine if a material is transparent.
It is also unrecommendable to change other code other than the main.cpp and 
raytrace.cpp files.