## Math-Parser #
A script to solve algebric expressions.
The user is supposed to insert a generic expression via terminal and the program outputs its integer result.
Some limitations are currently in place:
- the expression can make use of basic operators only, i.e. "+-*/"
- the expression can deal with round brackets only


## Functions and their usage
- **erase_whites()**: it scans the whole expression and eliminates white spaces
- **index_of_element()**: it looks for a specific character in a vector and returns its index, if found
- **brackets_count()**: it makes sure brackets are well placed and even-numbered. It raises an error in case something is not ok
- **sign_check()**: it checks no multiple operators are placed between subsequent operands. Moreover, it eliminates meaningless operators (such as a '+' at the beginning of an expression)
- **operation_identifier()**: it extracts sub-operations from the main expression and prioritizes their execution
- **operation_formatter()**: it separates operators from numbers and makes them ready for calculations
- **calculate()**: it calculates the result of a given operation
- **emplace_result()**: it places the result of an operation back in the expression, substituting the sub-expression it comes from
- **stop_condition()**: it checks whether no more calculations are to be executed
- **evaluate()**: the main function called from main that encapsulates all the functions listed above

## How to build
Install Eigen3 into your machine.
After cloning, just enter the main folder and run `$make`to compile the project.
Run `./solver` and type/paste a math expression onto the terminal.