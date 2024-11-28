# Project
## Running the program.

To compile the project,run `make all`.

To test the project,run `bash run02` for the normal tests and `bash run` for all tests.
Note `bash run` will take a very significant amount of time to run(i wasnt able to run it) due to `samples/network03.txt` 

The following should be run on unix style os e.g Linux or on Windows using WSL.

## Problems encountered
There are some problematic test cases which contradict the specifications in the given project:
The following are those tests:
 - Example 17: The output.txt is empty but the output.txt should have something in it as the instruction `PrintPath 1 2` is a valid instruction and always outputs something whether it fails or not.This is probably meant for the grader rather than for you to implement.The program should know about the tests its running or whether it passed or not.
 - Example 18: I noticed that the instructions `PrintPath 2 3` and `PrintPath 1 2` outputs have been skipped in the output.txt.Note that any print instuction should always output something back.This might be an oversight by the person who made the tests.
 - Example 20: Same case as example 18.The output.txt in the tests does not match what the actual absolute output should be.
 - Examples 14, 15, 19: There are no contradictions here but it takes an excessive amount of time to complete due to the sheer amount of data being processed at a time.
 
Those that are contradicting what the project describes cannot be fixed.
 
Another thing to note is that any test using "network03.txt" as an input and djiskstra's algorithm will run much longer than the rest because it has a much bigger data set than the rest.Also note the time complexity of this djiskstra's algorithm  is O(n^2) where n is the number of vertices.This makes "network03.txt" be (175813 ^ 2) which is quite ridiculous!