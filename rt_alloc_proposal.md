## Real-time allocators proposal.

The importance of linked data structures in computer science like
trees and linked lists cannot be over-emphasised, however there has
been a continuous move away from such data structures in recent years.
The reasons behind that is not due to the memory overhead of having to
store links in each node, but on the fact that such data structures
are not cache friendly. As matter of fact, many people today prefer
too use the flat alternatives and pay linear "insertion in the middle"
complexities than constant time complexity at the cost of memory
fragmentation and unpredictable performance loss.

The unpredictability is still worse on applications that need real-time
guarantees or aim 24/7 availability.

Even though the sub-optimal access patterns are inherent in linked
data structures, in c++ node-based containers it is made worser than
it should due to the design of allocators. In the opinion of the
author of this proposal, a small non-breaking addition to the standard
could strongly improve performance and render c++ node-based
containers usable in hard-real-time contexts. Following is an
explanation of how this can be achieved.

## Real-time node allocation

One of the most simple ways of to a linked list for example, is to
have a buffer of nodes use their links to link them together as a
stack. When we need to insert an element into the list we pop one node
from our buffer, when we remove one element we push it back into the
stack. Pushing and popping from a stack as as simple as changing
pointers, an operation that is constant time, only a few instructions
are needed to perform these tasks and therefore this is a real-time
operation.

# What prevents real-time node allocation in current c++?

