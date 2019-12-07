//
//  SDKObjectiveC.m
//  Algorithm
//

#include "algorithm.h"
#import "SDKObjectiveC.h"

@implementation SDKObjectiveC

- (NSInteger)invokeAlgorithmFunc:(NSInteger)number block:(AlgorithmBlock)block {
    return algorithm::invokeAlgorithmFunc((int)number, [block](int code, std::string message) {
        @autoreleasepool {
            if (block) {
                NSString *oc_message = [NSString stringWithUTF8String:message.c_str()];
                block(code, oc_message);
            }
        }
    });
}

@end
