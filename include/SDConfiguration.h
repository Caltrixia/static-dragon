#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

class SDConfiguration {
public:
    enum class ReportFormat {
        Terminal,
        Text,
        CSV,
        JSON,
        HTML
    };
    bool loadFromFile(const fs::path& file);
    const std::string& projectName() const;
    const fs::path& buildDirectory() const noexcept;
    const std::vector<std::string>& files() const;
    const fs::path& outputDirectory() const noexcept;
    const std::vector<ReportFormat>& formats() const;

private:
    std::string projectName_;
    fs::path buildDirectory_;
    std::vector<std::string> files_;
    fs::path outputDirectory_;
    std::vector<ReportFormat> formats_;
};