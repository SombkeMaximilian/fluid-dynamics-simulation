// File inc/bound.tpp
namespace fluid_dynamics {

template<typename T>
Bound<T>::Bound() : type_{BoundaryType::kDirichlet}, boundaries_{} {}

template<typename T>
Bound<T>::Bound(BoundaryType type) : type_{type}, boundaries_{} {}

template<typename T>
Bound<T>::Bound(BoundaryType type, const std::vector<Boundary<T>>& boundaries)
    : type_{type}, boundaries_{boundaries} {}

template<typename T>
Bound<T>::Bound(BoundaryType type, std::vector<Boundary<T>>&& boundaries)
    : type_{type}, boundaries_{std::move(boundaries)} {}

template<typename T>
BoundaryType Bound<T>::type() const {
  return type_;
}

template<typename T>
const std::vector<Boundary<T>>& Bound<T>::boundaries() const {
  return boundaries_;
}

template<typename T>
std::vector<Boundary<T>>& Bound<T>::boundaries() {
  return boundaries_;
}

template<typename T>
size_t Bound<T>::size() const {
  return boundaries_.size();
}

} // namespace fluid_dynamics