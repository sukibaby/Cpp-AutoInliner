// The program is self-contained in the .cpp, but I'd rather have this here in case I need to use it later.

#ifndef CPP_AUTO_INLINER_H
#define CPP_AUTO_INLINER_H

#include <string>

std::string readFile(const std::string& absoluteFilePath);
void writeFile(const std::string& absoluteFilePath, const std::string& fileContent);
std::string inlineMacros(const std::string& fileContent);

#endif // CPP_AUTO_INLINER_H
