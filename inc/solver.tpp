// File: inc/solver.tpp
namespace fluid_dynamics {

template<typename T>
Solver<T>::Solver() : epsilon_{kDefaultEpsilon}, max_iter_{kDefaultMaxIter} {}

template<typename T>
Solver<T>::Solver(T epsilon) : epsilon_{epsilon}, max_iter_{kDefaultMaxIter} {}

template<typename T>
Solver<T>::Solver(size_t max_iter) : epsilon_{kDefaultEpsilon}, max_iter_{max_iter} {}
template<typename T>
Solver<T>::Solver(T epsilon, size_t max_iter) : epsilon_{epsilon}, max_iter_{max_iter} {}

template<typename T>
size_t Solver<T>::max_iter() const {
  return max_iter_;
}

template<typename T>
T Solver<T>::epsilon() const {
  return epsilon_;
}

template<typename T>
void Solver<T>::max_iter(size_t max_iter) {
  max_iter_ = max_iter;
}

template<typename T>
void Solver<T>::epsilon(T epsilon) {
  epsilon_ = epsilon;
}

} // namespace fluid_dynamics
