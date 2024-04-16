#ifndef USERAUTH_WINDOWS
#define FLAG_WINDOW

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

std::vector<std::string> getUsersInGroups(const std::vector<std::string>& groups);
void DisplayGroups(std::vector<std::string>& userz);
std::vector<std::string> getUsernames();
void DisplayUsers(std::vector<std::string> &usernamez);
#endif