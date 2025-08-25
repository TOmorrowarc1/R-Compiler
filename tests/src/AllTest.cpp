#include "utils.hpp"
#include <fstream>
#include <sstream>

TEST(AllTest, VaildAll) {
  std::string input_path = test_cases_dir + "/semantic/AllResult.in";
  std::ifstream input_file(input_path);
  std::string line;
  while (std::getline(input_file, line, '\n')) {
    if (line.empty()) {
      continue;
    }
    std::stringstream ss(line);
    std::string prefix;
    int32_t code;
    ss >> prefix >> code;

    std::string input_path = test_cases_dir + "/semantic/" + prefix + ".rx";
    std::string text = readFileToString(input_path);
    int32_t result = 0;
    try {
      auto lex_result = lex(text);
      auto parse_result = parse(lex_result);

      Scope init_scope;
      auto root = parse_result.get();
      ASTAnnotate(root, &init_scope);
    } catch (const CompilerException &e) {
      result = -1;
    } catch (const std::exception &e) {
      result = -1;
    }
    if (result != code) {
      std::cout << "Test case " << prefix << " failed. Expected " << code
                << ", got " << result << '\n';
    } else {
      std::cout << "Test case " << prefix << " passed." << '\n';
    }
  }
}