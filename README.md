## Design goals

There are myriads of [C++](http://en.cppreference.com/w/cpp) libraries out there, and each may even have its reason to exist. Our class had these design goals:

- **Intuitive coding style** Simpler code is usually better code [examples below](#examples) and you'll know what I mean.
- **Modern CMake** We dont want variable and globbing but instead use [`targets`](https://gist.github.com/mbinna/c61dbb39bca0e4fb7d1f73b0d66a4fd1). 
- **Vanilla C++** Whenever possible resort to the official standard and refrain from using fancy third-party libs as much as possible.


```cpp
#include <auxiliary/auxiliary.inl>

