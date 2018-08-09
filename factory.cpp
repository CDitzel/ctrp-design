#include <functional>
#include <map>
#include <memory>
#include <string>
#include <string_view>

class Image {};
class BitmapImage : public Image {};
class PngImage : public Image {};
class JpgImage : public Image {};

struct IImageFactory {
  virtual std::shared_ptr<Image> Create(std::string_view type) const = 0;
};

struct ImageFactory : IImageFactory {
  std::shared_ptr<Image> Create(std::string_view type) const final {
    static std::map<std::string, std::function<std::shared_ptr<Image>()>>
        mapping{{"bmp", [] {
      return std::make_shared<BitmapImage>(); }},
                {"png", [] {
      return std::make_shared<PngImage>(); }},
                {"jpg", []{
      return std::make_shared<JpgImage>(); }}};

    if (auto it = mapping.find(std::data(type)); it != mapping.end())
      return it->second();
    return nullptr;
  }
};

int main() {
  auto factory = ImageFactory{};
  auto image = factory.Create("png");
}
