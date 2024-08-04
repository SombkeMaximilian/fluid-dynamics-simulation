// File: inc/poisson2d/grid_io.h
#ifndef FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_GRID_IO_H_
#define FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_GRID_IO_H_

#include <fstream>
#include <string>
#include "grid.h"

namespace fluid_dynamics {

template<typename T> void WriteGridBinary(const Grid<T>& grid, const std::string& filename);
template<typename T> void WriteGridBinary(const Grid<std::pair<T, T>>& grid, const std::string& filename);
template<typename T> void WriteGridText(const Grid<T>& grid, const std::string& filename);
template<typename T> void WriteGridText(const Grid<std::pair<T, T>>& grid, const std::string& filename);

} // namespace fluid_dynamics

#include "grid_io.tpp"

#endif //FLUID_DYNAMICS_SIMULATION_INC_POISSON2D_GRID_IO_H_
