#pragma once

namespace rt {

inline
std::size_t row_major_idx(std::size_t i, std::size_t j, std::size_t n_cols)
{ return i * n_cols + j; }

}

