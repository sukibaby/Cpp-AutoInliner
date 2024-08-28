﻿#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <regex>
#include <filesystem>

// Take an absolute file path as input and returns the content of the file as a string.
// It first validates that the provided path is absolute and that the file exists.
// The content of the file is read into a stringstream and then converted to a string which is returned.
std::string ReadFile(const std::string& path)
{
    std::filesystem::path filepath(path);

    if (!filepath.is_absolute())
        throw std::runtime_error("Path must be absolute: " + path);

    if (!std::filesystem::exists(filepath))
        throw std::runtime_error("File does not exist: " + path);

    std::ifstream uninlined_content(path);

    if (!uninlined_content.is_open())
        throw std::runtime_error("Could not open file: " + path);

    std::stringstream buffer;
    buffer << uninlined_content.rdbuf();

    return buffer.str();
}

// Takes an absolute file path and a string containing the file content as input.
// It writes the provided content to the file specified by the absolute file path.
void WriteFile(const std::string& writepath, const std::string& inlined_output)
{
    if (std::filesystem::exists(writepath))
    {
        std::string user_input;
        std::cout << "File " << writepath << " already exists. Do you want to overwrite it? (yes/no): ";
        std::cin >> user_input;

        if (user_input != "yes")
        {
            std::cout << "File not overwritten." << std::endl;
            return;
        }
    }

    std::ofstream inlined_buffer(writepath);

    if (!inlined_buffer.is_open())
        throw std::runtime_error("Could not open file: " + writepath);

    inlined_buffer << inlined_output;
}

// Takes the content of a file as a string and returns a new string with all macros inlined.
// It first finds all macro definitions in the file content using a regular expression and stores them in a map.
// Then, it replaces all occurrences of the macros in the file content with their corresponding definitions.
// This is the function that (hopefully) provides an output identical to the one generated by the compiler.
std::string InlineMacros(const std::string& file_content)
{
    std::unordered_map<std::string, std::string> macro_definitions;
    std::regex macro_definition_regex(R"(#define\s+(\w+)\s+(.+))");
    std::smatch regex_match;
    std::string inlined_content = file_content;
    std::string::const_iterator it(file_content.cbegin());

    while (std::regex_search(it, file_content.cend(), regex_match, macro_definition_regex))
    {
        macro_definitions[regex_match[1]] = regex_match[2];
        it = regex_match.suffix().first;
    }

    std::istringstream content_stream(file_content);
    std::ostringstream inlined_content_stream;
    std::string line;
    int line_number = 0;

    while (std::getline(content_stream, line))
    {
        line_number++;

        for (const auto& macro_definition : macro_definitions)
        {
            std::regex macro_usage_regex("\\b" + macro_definition.first + "\\b");

            if (std::regex_search(line, macro_usage_regex))
            {
                line = std::regex_replace(line, macro_usage_regex, macro_definition.second);
                std::cout << "Function inlined at line " << line_number << std::endl;
            }

        }
        inlined_content_stream << line << "\n";
    }

    return inlined_content_stream.str();
}

// 1. Validates the number of command-line arguments.
// 2. Reads the content of the input file using the ReadFile function.
// 3. Inlines all macros in the file content using the InlineMacros function.
// 4. Writes the inlined content to a new file with the same name as the input file but with an ".inlined"
//    extension using the WriteFile function.
// 5. Handles any exceptions that occur during the process and prints
int main(int argumentCount, char* argumentValues[])
{
    // Check if the correct number of arguments were provided
    if (argumentCount != 2)
    {
        std::cerr << "Usage: Cpp-AutoInliner.exe <file_path>" << std::endl;
        return 1;
    }

	try
	{
		std::string source_path = argumentValues[1];
		std::string original_content = ReadFile(source_path);
		std::string inlined_content = InlineMacros(original_content);
		WriteFile(source_path + ".inlined", inlined_content);
		std::cout << "Inlined file written to " << source_path << ".inlined" << std::endl;
	}
    
    catch (const std::exception& exception)
    {
        std::cerr << "Error: " << exception.what() << std::endl;
        return 1;
    }

    return 0;
}
