#ifndef FILE_HELP_H
#define FILE_HELP_H

#include <string>
#include <vector>

std::string trim(const std::string& s);
std::string toUpperStr(const std::string& s);
std::string toLowerStr(const std::string& s);
bool equalsIgnoreCase(const std::string& a, const std::string& b);
std::vector<std::string> splitPipe(const std::string& line);
bool isSkippableLine(const std::string& trimmedLine);
void printWarning(const std::string& file, int lineNumber, const std::string& reason);

#endif
