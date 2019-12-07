//
//  SDKObjectiveC.h
//  Algorithm
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef void(^AlgorithmBlock)(NSInteger code, NSString *message);

@interface SDKObjectiveC : NSObject

- (NSInteger)invokeAlgorithmFunc:(NSInteger)number block:(AlgorithmBlock)block;

@end

NS_ASSUME_NONNULL_END
