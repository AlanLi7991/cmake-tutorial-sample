#include <emscripten/val.h>
#include <iostream>
#include <string>

namespace com {
namespace sdk {

class SDKWebAssembly {

  public:
    SDKWebAssembly() = default;
    int invokeAlgorithmFunc(int number, emscripten::val lambda);
};

} // namespace sdk
} // namespace com
