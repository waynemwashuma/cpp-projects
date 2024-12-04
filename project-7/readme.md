please check your working directory path before compiling

To compile the project: `make all`
To test: `bash run.sh`

## Prooblems experienced

Here is an explanation why the following test cases will not pass:

- Test 10 : The equals sign is used as both an assignment operator on line 3 and as an equality operator on line 5.This test cannot pass because this conflicts with the parsing method used and the above two operators need separate and distinct symbols representing them.Even actual computer languages have to differenciate the two operators(= for assignment and == for equality check).

- Test case 12 : The formatting for the output is inconsistent with the implicit specifications in the project and likely breaks convention of the output format.Trying to fix this will likely fail other tests.

- Tests 11, 12 : The formatting for the output is inconsistent with the implicit specifications in the project and likely breaks convention of the output format.Trying to fix this will likely fail other tests.