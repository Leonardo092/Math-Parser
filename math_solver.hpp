#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

void erase_whites(std::string&);

int index_of_element(std::vector<char>, char);

bool brackets_count(std::string, std::vector<uint>&, std::vector<uint>&);

bool sign_check(std::string&, std::vector<uint>&, std::vector<uint>&);

std::string operation_identifier(std::string, std::vector<uint>, std::vector<uint>, std::pair<int, int>&);

bool operation_formatter(std::string, std::vector<int>&, std::vector<char>&);

int calculate(std::vector<int>, std::vector<char>);

void emplace_result(std::string&, int, std::pair<int, int>);

bool stop_condition(std::string);

bool evaluate(const char*, int&);