#include "math_solver.hpp"

// this functions serves to handle the occurrence of white spaces
// it takes in the original input and then eliminates ant space/tab
void erase_whites(std::string& input)
{
    std::string no_whites = "\0";

    for (auto& i:input)
    {
        if (i != ' ')
        {
            no_whites.push_back(i);
        }
    }

    input = no_whites;
}

int index_of_element(std::vector<char> vchar, char element)
{
    int index = -1;

    auto it = std::find(vchar.begin(), vchar.end(), element);
  
    // If element was found
    if (it != vchar.end()) 
    {    
        index = it - vchar.begin();
    }

    return index;
}

// this function serves to validate the integrity of a math expression based on brackets count
// in addition, it stores inedxes on brackets to identify sub operation in the expression
bool brackets_count(std::string str, std::vector<uint>& left_idx, std::vector<uint>& right_idx)
{
    left_idx.clear();
    right_idx.clear();
    int left_br_count = 0, right_br_count = 0;

    for (uint i = 0; i < str.size(); i++)
    {
        if (str[i] == '(')
        {
            left_idx.push_back(i);
            left_br_count++;
        }
        if (str[i] == ')')
        {
            right_idx.push_back(i);
            right_br_count++;
        }
    }
    // Check whether the expression has same number of left and right brackets
    if (left_br_count == right_br_count)
    {
        // accept case where no brackets are present
        if (left_idx.empty() && right_idx.empty())
        {
            return true;
        }
        else
        {
            // if present, check whether they are balanced
            for (uint i = 0; i < str.size(); i++)
            {
                if (right_idx[i] < left_idx[i])
                {
                    throw std::logic_error("expression is not bracket-wise balanced");
                }
                else
                {
                    return true;
                }
            }
        }
    }
    else
    {
        throw std::logic_error("left and right brackets are not even numbered");
    }
}

bool sign_check(std::string& input, std::vector<uint>& left_idx_v, std::vector<uint>& right_idx_v)
{
    // in case an expression is parsed without brackets whatsoever, artifiacls ones are created to contain the whole expressio
    // I admit this is indeed a little workaround in order not to overcomplicate the code that follows
    if (left_idx_v.empty() && right_idx_v.empty())
    {
        input = "(" + input + ")";
        left_idx_v.push_back(0);
        right_idx_v.push_back(input.size()-1);
    }

    for (uint i = 0; i < left_idx_v.size(); i++)
    {
        if ((input[left_idx_v[i]+1] == '*') || (input[left_idx_v[i]+1] == '/'))
        {
            throw std::logic_error("one of the operators has non left operand");
        }
        else if (input[left_idx_v[i]+1] == '+')
        {
            //erase useless sign
            input.erase(input.begin() + (left_idx_v[i]+1));

            //adjust brackets indexes
            for (uint j = i+1; j < left_idx_v.size(); j++)
                left_idx_v[j] -= 1;
            for (uint j = i; j < right_idx_v.size(); j++)
                right_idx_v[j] -= 1;

            return true;
        }
        else
        {
            return true;
        }
    }
}

// this function serves to extract the current highest priority operation in a math expression
// example: 3*(4+2) --> 4+2
std::string operation_identifier(std::string input, std::vector<uint> left_idx_v, std::vector<uint> right_idx_v, std::pair<int, int>& erase_range)
{
    if (!left_idx_v.empty() && !right_idx_v.empty())
    {
        // identify the correspondant left bracket to the current right one
        auto current_r_idx = right_idx_v.front();
        int target_l_idx = -1;
        int temp_value = -1;

        for (uint i = 0; i < left_idx_v.size(); i++)
        {
            if (left_idx_v[i] > current_r_idx)
            {
                break;
            }
            else
            {
                target_l_idx = left_idx_v[i];
            }
        }

        // save calculation range for replacing in further with its result
        erase_range = std::make_pair(target_l_idx, right_idx_v.front());

        // return characters in the identified range
        std::string identified_str = "\0";

        for (uint i = target_l_idx + 1; i < current_r_idx; i++)
        {
            identified_str.push_back(input[i]);
        }

        return identified_str;
    }
    else
    {
        return input;
    }

}

// this function serve to identify sequence of numbers and compact them into a unique char
// example: '1' '2' '1' '*' '3' --> '121' '*' '3'
bool operation_formatter(std::string input, std::vector<int>& numbers, std::vector<char>& operators)
{
    std::string num_holder = "\0";
    bool negative_start = false;

    // check whether the string starts with a '-' sign
    // in case it does, it must not be considered as operator but turn first number into its negative counterpart
    if (input[0] == '-')
    {
        negative_start = true;
        input.erase(input.begin());
    }

    for (uint i = 0; i < input.size(); i++)
    {
        if ((input[i] == '+') || (input[i] == '-') || (input[i] == '*') || (input[i] == '/'))
        {
            if (!num_holder.empty())
            {
                erase_whites(num_holder);
                
                auto temp_int = std::stoi(num_holder);
                num_holder = "\0";

                if (negative_start)
                {
                    numbers.push_back(-temp_int);
                    negative_start = false;
                }
                else
                {
                    numbers.push_back(temp_int);
                }

                operators.push_back(input[i]); 
            }
            else //this is the case two consecutives signs are found
            {
                if (input[i] == '-')
                {
                    negative_start = true;
                    num_holder += ' ';
                }
                else if (input[i] == '+')
                {
                    num_holder += ' ';
                }
                else if ((input[i] == '*') || (input[i] == '/'))
                {
                    throw std::logic_error("multiple operators act on same pair of operands");
                }
            } 
        }
        else
        {
            num_holder += input[i];
        }
    }
    erase_whites(num_holder);
    if (negative_start)
    {
        numbers.push_back(-std::stoi(num_holder));
        negative_start = false;
    }
    else
    {
        numbers.push_back(std::stoi(num_holder));
    }
    return true;
}

// this function produces the result of a "non-bracketed" expression
int calculate(std::vector<int> numbers, std::vector<char> operators)
{
    int result = 0;

    while (!operators.empty())
    {

        // operations follow the priority order of algebra
        if (index_of_element(operators, '*') != -1)
        {
            // obtain index of operator
            auto i = index_of_element(operators, '*');
            // apply coherent calculation to numbers originally having that operator in between
            result = numbers[i] * numbers[i+1];
            // erase old operands and take them over with their result
            numbers[i] = result;
            numbers.erase(numbers.begin() + i + 1);
            // erase operator from operators vector
            operators.erase(operators.begin() + i);
        }
        else if (index_of_element(operators, '/') != -1)
        {
            // obtain index of operator
            auto i = index_of_element(operators, '/');
            // apply coherent calculation to numbers originally having that operator in between
            result = numbers[i] / (float)(numbers[i+1]);
            // erase old operands and take them over with their result
            numbers[i] = result;
            numbers.erase(numbers.begin() + i + 1);
            // erase operator from operators vector
            operators.erase(operators.begin() + i);
        }
        else
        {
            // here below, given neither multiplications nor divisions are found, operations are executed from left to right
            if ((index_of_element(operators, '+') != -1) && (index_of_element(operators, '-') != -1))
            {
                int i;
                // obtain index of operator
                auto j = index_of_element(operators, '+');
                auto k = index_of_element(operators, '-');
                // determine which among + and - comes first
                if (j < k)
                {
                    i = j;
                    // apply coherent calculation to numbers originally having that operator in between
                    result = numbers[i] + numbers[i+1];
                }
                else
                {
                    i = k;
                    // apply coherent calculation to numbers originally having that operator in between
                    result = numbers[i] - numbers[i+1];
                }
                // erase old operands and take them over with their result
                numbers[i] = result;
                numbers.erase(numbers.begin() + i + 1);
                // erase operator from operators vector
                operators.erase(operators.begin() + i);
            }
            else if ((index_of_element(operators, '+') != -1) && (index_of_element(operators, '-') == -1))
            {
                // obtain index of operator
                auto i = index_of_element(operators, '+');           
                // apply coherent calculation to numbers originally having that operator in between
                result = numbers[i] + numbers[i+1];
                // erase old operands and take them over with their result
                numbers[i] = result;
                numbers.erase(numbers.begin() + i + 1);
                // erase operator from operators vector
                operators.erase(operators.begin() + i);
            }
            else if ((index_of_element(operators, '+') == -1) && (index_of_element(operators, '-') != -1))
            {
                // obtain index of operator
                auto i = index_of_element(operators, '-');           
                // apply coherent calculation to numbers originally having that operator in between
                result = numbers[i] - numbers[i+1];
                // erase old operands and take them over with their result
                numbers[i] = result;
                numbers.erase(numbers.begin() + i + 1);
                // erase operator from operators vector
                operators.erase(operators.begin() + i);
            }
        }
    }
    return result;
}

// this function serves to substitute a subset of calculations with its result
// example: (1+(3*4)) --> (1+12)
void emplace_result(std::string& input, int result, std::pair<int, int> range)
{
    // if no erase range assigned (meaning no brackets in the expression)
    if ((range.first == 0) && (range.second == 0))
    {
        input = std::to_string(result);
    }
    else
    {
        //erase subsequent characters until sub-operation right bracket
        input.erase(range.first, (range.second - range.first + 1));

        // put the result in the same position of the sub-operation left bracket
        input.insert(range.first, std::to_string(result));
    }
}

// This function serves to identify whether there is still operations to be performed
bool stop_condition(std::string input)
{

    auto prod = std::find(input.begin(), input.end(), '*');
    auto div = std::find(input.begin(), input.end(), '/');
    auto sum = std::find(input.begin(), input.end(), '+');
  
    // If no operator is found ('-' is not included in this check as the expression may terminate with a negative number as result)
    if ((prod == input.end()) && (div == input.end()) && (sum == input.end())) 
    {    
        return true;
    }
    else
    {
        return false;
    }
}

bool evaluate(const char* expression, int& result)
{
    std::vector<uint> left_brackets_idx = {};
    std::vector<uint> right_brackets_idx = {};
    std::pair<int, int> erase_range;
    bool stop = false;
    bool success = true;

    //here starts the required function
    auto input_str = std::string(expression);

    // verify order of brackets in the expression
    while (!stop)
    {
        auto brackets_ok = brackets_count(input_str, left_brackets_idx, right_brackets_idx);
        if (brackets_ok)
        {
            auto sign_ok = sign_check(input_str, left_brackets_idx, right_brackets_idx);
            if (sign_ok)
            {
                // identify and exctract meaningful sub-operation
                auto operational_array = operation_identifier(input_str, left_brackets_idx, right_brackets_idx, erase_range);

                // format operands to be processed
                std::vector<int> numbers = {};
                std::vector<char> operators = {};
                auto format_ok = operation_formatter(operational_array, numbers, operators);

                if (format_ok)
                {
                    // calculate result
                    auto outcome = calculate(numbers, operators);

                    // substitute operation with the result
                    emplace_result(input_str, outcome, erase_range);

                    // check for further operations available
                    stop = stop_condition(input_str);
                }
                else
                {
                    success = false;
                    break;
                }

            }
            else
            {
                success = false;
                break;
            }
        }
        else
        {
            success = false;
            break;
        }
    }

    if (success)
    {
        result =  std::stoi(input_str);
    }

    return success;
}