#include "sdk_web_assembly.h"
#include "algorithm.h"
#include <emscripten/bind.h>
#include <sstream>

using namespace emscripten;

int com::sdk::SDKWebAssembly::invokeAlgorithmFunc(int number, emscripten::val lambda) {
    printf("com::sdk::SDKWebAssembly::invokeAlgorithmFunc");
    return algorithm::invokeAlgorithmFunc(number, [lambda](int code, std::string message) {
        if (!lambda.isNull()) {
            lambda(code, message);
        }
    });
}

EMSCRIPTEN_BINDINGS(sdk_web_assembly) {
    class_<com::sdk::SDKWebAssembly>("SDKWebAssembly")
        .constructor()
        .function("invokeAlgorithmFunc", &com::sdk::SDKWebAssembly::invokeAlgorithmFunc);
}