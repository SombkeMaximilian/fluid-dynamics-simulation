// File: examples/parse_args.h
#ifndef FLUID_DYNAMICS_SIMULATION_EXAMPLES_PARSE_ARGS_H_
#define FLUID_DYNAMICS_SIMULATION_EXAMPLES_PARSE_ARGS_H_

namespace parse_args {

void PrintOptions(char* argv[]) {
  std::cout << "Usage: " << std::endl;
  std::cout << "  " << argv[0] << " [options]" << std::endl;
  std::cout << "Options:" << std::endl;
  std::cout << "  -h, --help  Show this help message and exit" << std::endl;
  std::cout << "  -L          The size of the grid (Default 102)" << std::endl;
  std::cout << "  -epsilon    The stopping criterion for the solver (Default 1e-2)" << std::endl;
  std::cout << "  -max_iter   The maximum number of iterations (Default 3000)" << std::endl;
}

void ParseArgs(int argc, char* argv[], size_t& L, double& epsilon, size_t& max_iter, bool& terminate) {
  long long L_test;
  long long max_iter_test;
  bool L_flag = false;
  bool epsilon_flag = false;
  bool max_iter_flag = false;

  terminate = false;

  if (argc == 1) {
    PrintOptions(argv);
    terminate = true;
  } else if (argc > 1) {
    for (size_t i = 1; i < argc; ++i) {
      if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
        PrintOptions(argv);
        terminate = true;
        break;
      } else if (std::string(argv[i]) == "--default") {
        L_flag = false;
        epsilon_flag = false;
        max_iter_flag = false;
        break;
      } else if (std::string(argv[i]) == "-L") {
        L_test = std::stoll(argv[++i]);
        if (L_test <= 0) {
          std::cerr << "L must be greater than 0" << std::endl;
          terminate = true;
          break;
        }
        L = static_cast<size_t>(L_test);
        L_flag = true;
      } else if (std::string(argv[i]) == "-epsilon") {
        epsilon = std::stod(argv[++i]);
        epsilon_flag = true;
        if (epsilon <= 0) {
          std::cerr << "epsilon must be greater than 0" << std::endl;
          terminate = true;
          break;
        }
      } else if (std::string(argv[i]) == "-max_iter") {
        max_iter_test = std::stoll(argv[++i]);
        if (max_iter_test <= 0) {
          std::cerr << "max_iter must be greater than 0" << std::endl;
          terminate = true;
          break;
        }
        max_iter = static_cast<size_t>(max_iter_test);
        max_iter_flag = true;
      } else {
        std::cerr << "Unknown option: " << argv[i] << std::endl;
        terminate = true;
        break;
      }
    }
  }

  if (!L_flag) {
    L = 102;
  }
  if (!epsilon_flag) {
    epsilon = 1e-2;
  }
  if (!max_iter_flag) {
    max_iter = 3000;
  }
}

} // namespace parse_args

#endif // FLUID_DYNAMICS_SIMULATION_EXAMPLES_PARSE_ARGS_H_
