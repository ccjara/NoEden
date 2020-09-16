#include "system_interface.hxx"

// initialize identity group for systems
template<>
j_static_id_t j_static_identity_provider<j_system_interface>::next_ = 0U;
