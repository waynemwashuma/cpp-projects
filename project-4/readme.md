## Compiling the program and running it
In the terminal, navigate to the current directory.

If you have `make` installed on your computer,run the following command to compile:
  `make all`
else you should run this:
  `g++ -o main main.cpp -Wall -lpthread -std=c++17`

The compiled application will be called `main.exe`

The minimum amount of threads to get a race condition is 10 with 10 iteration counts.