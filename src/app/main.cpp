#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <thread>
#include <atomic>

#include "spreadsheet_loader.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
  using namespace std::filesystem;

  const auto kExecPath = canonical(absolute(u8path(argv[0])));
  const auto kBinPath = kExecPath.parent_path();
  const auto kInputFile = kBinPath.parent_path() / "input.txt";
  const auto kOutputFile = kBinPath.parent_path() / "output.txt";

  StdOutLogger logger;
  SpreadsheetLoader loader(&logger);
  
  auto spreadsheet = loader.Load(kInputFile);
  std::cout << "Saving..." << std::endl;
  spreadsheet.Save(kOutputFile);
  std::cout << kOutputFile <<  " saved!" << std::endl;
  
  std::cout << "Changing R64440=100" << std::endl;
  spreadsheet.ChangeCell("R64440", "100");
  std::cout << "Done!" << std::endl;

  std::vector<std::thread> threads;
  std::cout << "\nTest console\n"
            << "q - quit\n"
            << "[cell]=[formula] - change formula (Example: A0=A1+15+A2)\n"
            << "[cell] - print value (Example: A3)\n" 
            << std::endl;
  std::string line;
  while (true) {
    std::cout << "> ";
    std::cin >> line;
    if (line == "q") break;

    auto eq = line.find('=');
    if (eq == std::string::npos) {
      auto value = spreadsheet.GetCellValue(line);
      auto formula = spreadsheet.GetCellFormula(line);
      std::cout << value << " (" << formula << ")" << std::endl;
      continue;
    }

    auto cell = line.substr(0, eq);
    auto formula = line.substr(eq + 1);
    std::cout << spreadsheet.ChangeCell(cell, formula) << std::endl;
  }

  return 0;
}