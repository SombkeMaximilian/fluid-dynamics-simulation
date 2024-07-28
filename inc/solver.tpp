// File: inc/solver.tpp
namespace fluid_dynamics {

template<typename T>
Solver<T>::Solver() : epsilon_{kDefaultEpsilon}, max_iter_{kDefaultMaxIter}, norm_{DefaultNorm} {}

template<typename T>
Solver<T>::Solver(T epsilon) : epsilon_{epsilon}, max_iter_{kDefaultMaxIter}, norm_{DefaultNorm} {}

template<typename T>
Solver<T>::Solver(size_t max_iter) : epsilon_{kDefaultEpsilon}, max_iter_{max_iter}, norm_{DefaultNorm} {}

template<typename T>
Solver<T>::Solver(std::function<T(const Grid<T>&, const Grid<T>&)> norm)
    : epsilon_{kDefaultEpsilon}, max_iter_{kDefaultMaxIter}, norm_{norm} {}

template<typename T>
Solver<T>::Solver(T epsilon, size_t max_iter) : epsilon_{epsilon}, max_iter_{max_iter}, norm_{DefaultNorm} {}

template<typename T>
Solver<T>::Solver(T epsilon, std::function<T(const Grid<T>&, const Grid<T>&)> norm)
    : epsilon_{epsilon}, max_iter_{kDefaultMaxIter}, norm_{norm} {}

template<typename T>
Solver<T>::Solver(size_t max_iter, std::function<T(const Grid<T>&, const Grid<T>&)> norm)
    : epsilon_{kDefaultEpsilon}, max_iter_{max_iter}, norm_{norm} {}

template<typename T>
Solver<T>::Solver(T epsilon, size_t max_iter, std::function<T(const Grid<T>&, const Grid<T>&)> norm)
    : epsilon_{epsilon}, max_iter_{max_iter}, norm_{norm} {}

template<typename T>
T Solver<T>::epsilon() const {
  return epsilon_;
}

template<typename T>
size_t Solver<T>::max_iter() const {
  return max_iter_;
}

template<typename T>
void Solver<T>::epsilon(T epsilon) {
  epsilon_ = epsilon;
}

template<typename T>
void Solver<T>::max_iter(size_t max_iter) {
  max_iter_ = max_iter;
}

template<typename T>
T Solver<T>::DefaultNorm(const Grid<T>& prev, const Grid<T>& curr) {
  T norm = 0;
  for (size_t i = 0; i < prev.width(); ++i) {
    for (size_t j = 0; j < prev.height(); ++j) {
      norm += (prev(i, j) - curr(i, j)) * (prev(i, j) - curr(i, j));
    }
  }
  return sqrt(norm);
}

} // namespace fluid_dynamics
