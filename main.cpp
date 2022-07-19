#include "math_solver.hpp"

int main()
{
    std::string input;
    int result;
    std::getline(std::cin, input);

    erase_whites(input);

    // convert string to char array
    const char* input_array = input.c_str();

    // main code
    try
    {
        auto valid = evaluate(input_array, result);
    
        if (valid)
        {
            std::cout<<"result is: "<<result<<std::endl;
        }
    }
    catch(std::logic_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}