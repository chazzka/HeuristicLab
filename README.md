# heuristic

HOW TO RUN:
Main file is src/Main.cpp
- #include your algorithm at the top of the file following these rules:
1. run function is ```vector<result> run(dimension, testFunctionNumber, boundaryLow, boundaryUp)```
2. result is a struct containing: int fez, double cost.
3. class name is Algorithm

all the files are included in Main.cpp before compilation. Sufficient command should be:

`g++ -o heuristic src/Main.cpp\`

or 

```clang++ -O2 src/Main.cpp -o heuristic\```


fez - current function evaluation count\
cost - function evaluation value at current fez number

HOW TO RUN TESTS:
install bazel: https://bazel.build/

`
cd test
bazel test //:your_test_file
`

for information about writing tests, visit: https://google.github.io/googletest/quickstart-bazel.html