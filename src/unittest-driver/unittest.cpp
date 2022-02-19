// ========================================================================== //
// Depenencies

// STL
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;

#include <vector>
#include <string>
using namespace std::string_literals;

// own
#include "unittest.hpp"

// ========================================================================== //
// procs

void unittest_check_files_present(const Unittest_RessorceList& files)
{
    for (auto& file : files)
    {
        // *INDENT-OFF*
        if (!fs::exists (file)) {throw std::runtime_error("Inconsistent state of test system: file '"s + file + "' not found");}
        if (fs::is_empty(file)) {throw std::runtime_error("Inconsistent state of test system: file '"s + file + "' is empty");}
        // *INDENT-ON*
    }
}

void unittest_check_directories(const Unittest_RessorceList& directories)
{
    for (auto& directory : directories)
    {
        // *INDENT-OFF*
        if (!fs::exists      (directory)) {throw std::runtime_error("Inconsistent state of test system: directory '"s + directory  + "' not found");}
        if (!fs::is_directory(directory)) {throw std::runtime_error("Inconsistent state of test system: directory '"s + directory  + "' is not a directory");}
        if (fs::is_empty     (directory)) {throw std::runtime_error("Inconsistent state of test system: directory '"s + directory  + "' is empty");}
        // *INDENT-ON*
    }
}
