#include "SDConfiguration.h"

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <utility>

#include <llvm/Support/raw_ostream.h>

using json = nlohmann::json;
namespace fs = std::filesystem;

bool SDConfiguration::loadFromFile(const fs::path& file) {
    std::ifstream inputFile(file);

    if (!inputFile.is_open()) {
        std::cerr << "Failed to open configuration file: " << file << std::endl;
        return false;
    }

    try {
        json config;
        inputFile >> config;

        projectName_ = config.at("project").at("name").get<std::string>();

        buildDirectory_ = config.at("project").at("build_directory").get<fs::path>();

        files_ = config.at("analysis").at("files").get<std::vector<std::string>>();

        outputDirectory_ = config.at("reports").at("output_directory").get<fs::path>();

        formats_.clear();

        for (const auto& formatNode : config.at("reports").at("formats")) {
            const std::string format = formatNode.get<std::string>();

            if (format == "terminal") {
                formats_.push_back(SDConfiguration::ReportFormat::Terminal);
            } else if (format == "txt") {
                formats_.push_back(SDConfiguration::ReportFormat::Text);
            } else if (format == "csv") {
                formats_.push_back(SDConfiguration::ReportFormat::CSV);
            } else if (format == "json") {
                formats_.push_back(SDConfiguration::ReportFormat::JSON);
            } else if (format == "html") {
                formats_.push_back(SDConfiguration::ReportFormat::HTML);
            } else {
                llvm::errs() << "Unsupported report format: " << format << '\n';
                return false;
            }
        }
    }

    catch (const json::out_of_range& error) {
        llvm::errs() << "Missing required configuration field in: " << file << '\n' << error.what() << '\n'; 
        return false;
    }
    catch (const json::type_error& error) 
    { 
        llvm::errs() << "Invalid configuration value type in: " << file << '\n' << error.what() << '\n'; 
        return false; 
    }

    return true;
}

const std::string& SDConfiguration::projectName() const {
    return projectName_;
}

const fs::path& SDConfiguration::buildDirectory() const noexcept
{
    return buildDirectory_;
}

const std::vector<std::string>& SDConfiguration::files() const {
    return files_;
}

const fs::path& SDConfiguration::outputDirectory() const noexcept {
    return outputDirectory_;
}

const std::vector<SDConfiguration::ReportFormat>& SDConfiguration::formats() const {
    return formats_;
}
