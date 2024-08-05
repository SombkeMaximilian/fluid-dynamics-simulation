// File: examples/parse_args.h
#ifndef FLUID_DYNAMICS_SIMULATION_EXAMPLES_PARSE_ARGS_H_
#define FLUID_DYNAMICS_SIMULATION_EXAMPLES_PARSE_ARGS_H_

namespace parse_args {

void ParseArgs(int argc, char* argv[], size_t& L, double& epsilon, size_t& max_iter) {
  bool L_flag = false;
  bool epsilon_flag = false;
  bool max_iter_flag = false;

  if (argc == 0) {
    std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
  }
  if (argc > 1) {
    for (size_t i = 1; i < argc; ++i) {
      if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help") {
        std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
        std::cout << "Options:" << std::endl;
        std::cout << "  -h, --help  Show this help message and exit" << std::endl;
        std::cout << "  -L          The size of the grid (Default 102)" << std::endl;
        std::cout << "  -epsilon    The stopping criterion for the solver (Default 1e-2)" << std::endl;
        std::cout << "  -max_iter   The maximum number of iterations (Default 10000)" << std::endl;
        exit(0);
      } else if (std::string(argv[i]) == "-L") {
        L = std::stoul(argv[++i]);
        L_flag = true;
      } else if (std::string(argv[i]) == "-epsilon") {
        epsilon = std::stod(argv[++i]);
        epsilon_flag = true;
      } else if (std::string(argv[i]) == "-max_iter") {
        max_iter = std::stoul(argv[++i]);
        max_iter_flag = true;
      } else {
        std::cerr << "Unknown option: " << argv[i] << std::endl;
        exit(1);
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
    max_iter = 10000;
  }
}

} // namespace parse_args

#endif // FLUID_DYNAMICS_SIMULATION_EXAMPLES_PARSE_ARGS_H_
