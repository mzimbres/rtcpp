#pragma once

#include <cstring>

namespace rt {

  /*_________________________________________________________

  This function takes a pointer p to an array of n bytes.
  The address to which p points is expected to be aligned on
  a sizeof (char*) boundary. The minimum value of n expected
  is 2 * S. The buffer is split into blocks of size S and
  the blocks are linked in a LIFO fashion. The address of
  the next block is recorded in the first bytes of the
  block.

  returns a pointer to the top of the stack.
  ___________________________________________________________*/

template <std::size_t S> // Block size in bytes.
char* link_stack(char* p, std::size_t n)
{
  const std::size_t m = n / S; // Number of blocks of size S available.
  if (m < 2) // The minimum number of blocks we need is 2.
    return 0;

  const std::size_t ptr_size = sizeof (char*);
  for (std::size_t i = 1; i < m; ++i) {
    char* pp = p + (i - 1) * S; // Pointer to the previous block.
    char* pn = p + i * S; // Pointer to the next block.
    std::memcpy(pn, &pp, ptr_size);
  }

  std::memset(p, 0, ptr_size);
  return p + (m - 1) * S; // Pointer to the top of the stack.
}

}

