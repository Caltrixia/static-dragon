#include <fstream>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

int main(){    
    fs ::path reportDir = std::filesystem::path(PROJECT_ROOT) / "reports";
    
    if(!fs::exists(reportDir)){
        std::cout << "create reports folder\n" << std::endl;
        fs::create_directories(reportDir);
    } else if(!fs::is_directory(reportDir)){
        std::cerr << reportDir << " exists but is not a directory.\n";
        return 1;        
    }
    

    std::cout << "Program started\n";
    const std::string outputPath = "reports/his-dragon-report.txt";
    std::ofstream outputFile(outputPath);
    if (!outputFile.is_open()) {
        std::cerr
            << "Failed to create CSV report: "
            << outputPath
            << '\n';

        return false;
    }
    std::cout << "File opened successfully\n";

    outputFile << "Hello, World!" << '\n';

    std::cout << "Done\n";
    return 0;
}