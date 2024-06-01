#ifndef JARALYN_CONCEPTS_HXX
#define JARALYN_CONCEPTS_HXX

template<typename T>
concept StringLike = std::convertible_to<T, std::string_view>;

#endif
