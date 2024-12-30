#include "path_utils.h"
#include <filesystem>

std::string PathUtils::projectRoot;
bool PathUtils::isInitialized = false;

void PathUtils::initializeProjectRoot() {
    if (isInitialized) return;
    
    // Get the current executable path
    std::filesystem::path execPath = std::filesystem::current_path();
    
    // Go up one level from build directory to find project root
    projectRoot = execPath.parent_path().string();
    
    isInitialized = true;
}

std::string PathUtils::getProjectRoot() {
    initializeProjectRoot();
    return projectRoot;
}

std::string PathUtils::getResourcePath(const std::string& resourcePath) {
    initializeProjectRoot();
    return (std::filesystem::path(projectRoot) / resourcePath).string();
}
