#include <sstream>
#include "../include/algorithm.h"

namespace algorithm {

    int invokeAlgorithmFunc(int number, AlgorithmLambda lambda) {
        for (int i = 0; i < number; ++i) {
            if (lambda) {
                std::stringstream message;
                message << "AlgorithmFunc invoke " << i+1 << " times";
                lambda(i+1, message.str());
            }
        }
        return 0;
    }


}
