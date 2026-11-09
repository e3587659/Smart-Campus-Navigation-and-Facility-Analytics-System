#include "FileHelp.h"
#include <cctype>
#include <iostream>

std::string trim(const std::string& s) {
    size_t start = 0;
    size_t end = s.size();
    while (start < end && std::isspace(static_cast<unsigned char>(s[start]))) start++;
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;
    return s.substr(start, end - start);
}

std::string toUpperStr(const std::string& s) {
    std::string r = s;
    for (char& c : r) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return r;
}

std::string toLowerStr(const std::string& s) {
    std::string r = s;
    for (char& c : r) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return r;
}

bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    return toLowerStr(trim(a)) == toLowerStr(trim(b));
}

std::vector<std::string> splitPipe(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    for (char c : line) {
        if (c == '|') {
            fields.push_back(trim(current));
            current.clear();
        } else {
            current += c;
        }
    }
    fields.push_back(trim(current));
    return fields;
}

bool isSkippableLine(const std::string& trimmedLine) {
    return trimmedLine.empty() || trimmedLine[0] == '#';
}

void printWarning(const std::string& file, int lineNumber, const std::string& reason) {
    std::cout << "Warning: " << file << " line " << lineNumber << ": " << reason << "\n";
}
