#include "FacilityManager.h"
#include "FileHelp.h"
#include <cctype>
#include <fstream>
#include <iostream>

static const size_t FACILITY_FIELDS = 4;

bool FacilityManager::isValidIdFormat(const std::string& id) {
    if (id.size() != 4 || id[0] != 'F') return false;
    for (size_t i = 1; i < id.size(); i++) {
        if (!std::isdigit(static_cast<unsigned char>(id[i]))) return false;
    }
    return true;
}
