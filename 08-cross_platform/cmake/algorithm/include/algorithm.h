#include <iostream>
#include <string>

namespace algorithm {

    typedef std::function<void(int code, std::string message)> AlgorithmLambda;

    int invokeAlgorithmFunc(int number, AlgorithmLambda lambda);
}
