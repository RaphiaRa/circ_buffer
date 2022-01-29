# circ_buffer
**Requirements**  
A compiler that supports at least C++11

**Installation**  
Copy the header file into your project and rename the namespace to whatever you want.

**Usage**  
The usage of the circular buffer is similar to that of a STL container.
The circular buffer is initialized with a fixed buffer size, no hidden allocations
are done while using the circular buffer. Just like with STL containers, a custom allocator can be used if needed (std::allocator is used by default).
```c++
#include "circ_buffer.hpp"
#include <iostream>
int main()
{
    raphia::circ_buffer<char> circ(64);
    circ.push_back('h');
    circ.push_back('e');
    circ.push_back('l');
    circ.push_back('l');
    circ.push_back('o');

    for (auto c : circ)
        std::cout << c;
    std::cout << std::endl;
}
```
Output:
```bash
>> hello
```

If data is pushed into the buffer via push_back and the buffer is out of space, then
the data at the beginning will be overwritten.

```c++
int main()
{
    raphia::circ_buffer<char> circ(4);
    circ.push_back('h');
    circ.push_back('e');
    circ.push_back('l');
    circ.push_back('l');
    circ.push_back('o');

    for (auto c : circ)
        std::cout << c;
    std::cout << std::endl;
}
```
Output:
```bash
>> ello
```
The behaviour of the ring buffer is similar when emblace_back is used.
On the contrary, if data is pushed into the buffer via push_front, then
the data in the back will be overwritten if the buffer is full.

**Building & running the tests**
```bash
git clone git@github.com:RaphiaRa/circ_buffer.git
mkdir circ_buffer/build; cd circ_buffer/build;
git submodule update --init
cmake ../
make
./Test
```

