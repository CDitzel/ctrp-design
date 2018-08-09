#include <fstream>
#include <functional>
#include <iostream>

bool write_data(char const *const filename, char const *const data,
                size_t const size) {
  auto success = false;
  std::ofstream ofile(filename, std::ios::binary);

  if (ofile.is_open()) {
    try {
      ofile.write(data, size);
      success = true;
    } catch (std::ios_base::failure &) {
      // handle the error
    }
    ofile.close();
  }

  return success;
}

size_t read_data(char const *const filename,
                 std::function<char *(size_t const)> allocator) {
  size_t readbytes = 0;
  std::ifstream ifile(filename, std::ios::ate | std::ios::binary);

  if (ifile.is_open()) {
    auto length = static_cast<size_t>(ifile.tellg());
    ifile.seekg(0, std::ios_base::beg);

    auto buffer = allocator(length);

    try {
      ifile.read(buffer, length);

      readbytes = static_cast<size_t>(ifile.gcount());
    } catch (std::ios_base::failure &) {
      // handle the error
    }

    ifile.close();
  }

  return readbytes;
}

int main() {
  std::vector<unsigned char> output{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<unsigned char> input;

  if (write_data("sample.bin", reinterpret_cast<char *>(output.data()),
                 output.size())) {
    if (read_data("sample.bin", [&input](size_t const length) {
          input.resize(length);
          return reinterpret_cast<char *>(input.data());
        }) > 0) {
      std::clog << (output == input ? "equal" : "not equal") << std::endl;
    }
  }
}

// The following are possible alternatives for reading data from a file stream:
// Initializing an std::vector directly using std::istreambuf_iterator iterators
// (similarly, this can be used with std::string):
std::vector<unsigned char> input;
std::ifstream ifile("sample.bin", std::ios::binary);
if (ifile.is_open()) {
  input = std::vector<unsigned char>(std::istreambuf_iterator<char>(ifile),
                                     std::istreambuf_iterator<char>());
  ifile.close();
}

// Assigning the content of an std::vector from std::istreambuf_iterator
// iterators:
std::vector<unsigned char> input;
std::ifstream ifile("sample.bin", std::ios::binary);
if (ifile.is_open()) {
  ifile.seekg(0, std::ios_base::end);
  auto length = ifile.tellg();
  ifile.seekg(0, std::ios_base::beg);

  input.reserve(static_cast<size_t>(length));
  input.assign(std::istreambuf_iterator<char>(ifile),
               std::istreambuf_iterator<char>());
  ifile.close();
}

// Copying the content of the file stream to a vector using
// std::istreambuf_iterator iterators and an std::back_inserter adapter to write
// to the end of the vector:
std::vector<unsigned char> input;
std::ifstream ifile("sample.bin", std::ios::binary);
if (ifile.is_open()) {
  ifile.seekg(0, std::ios_base::end);
  auto length = ifile.tellg();
  ifile.seekg(0, std::ios_base::beg);

  input.reserve(static_cast<size_t>(length));
  std::copy(std::istreambuf_iterator<char>(ifile),
            std::istreambuf_iterator<char>(), std::back_inserter(input));
  ifile.close();
}
