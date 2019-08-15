#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {

  auto build_dir = fs::canonical(fs::path{"."});
  fs::create_directories(build_dir / "output");

  return 0;
}