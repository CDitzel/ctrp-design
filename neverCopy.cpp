#include <string>


template <class T>
class TextDisplayer
{
public:
   explicit TextDisplayer(T&& text) : text_(std::forward<T>(text)) {}
private:
   T text_;
};


template<class T> TextDisplayer(T&&) -> TextDisplayer<T>; // deduction guide

int main() {
  std::string txt = "Hello World";
  TextDisplayer displayer1(txt);
}


// either move or reference the original
