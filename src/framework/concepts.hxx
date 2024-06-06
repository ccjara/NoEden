#ifndef NOEDEN_CONCEPTS_HXX
#define NOEDEN_CONCEPTS_HXX

template<typename T>
concept StringLike = std::convertible_to<T, std::string_view>;

#endif
