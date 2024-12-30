#pragma once

#include <string>
#include <filesystem>

class PathUtils {
public:
    static std::string getProjectRoot();
    static std::string getResourcePath(const std::string& resourcePath);

private:
    static std::string projectRoot;
    static void initializeProjectRoot();
    static bool isInitialized;
};
