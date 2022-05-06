// Runevm.m

#import "Runevm.h"
#include "ObjcppBridge.h"
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
@implementation Runevm

RCT_EXPORT_MODULE()

/*RCT_EXPORT_METHOD(runeVMLoad:(NSString *)base64Encoded callback:(RCTResponseSenderBlock)callback)
{
    NSData *wasm = [[NSData alloc]initWithBase64EncodedString:base64Encoded options:0];
    NSUInteger count = [wasm length] ;
    const char *bytes = [wasm bytes];
    struct Config cfg = {
        .rune = bytes,
        .rune_len = count,
    };
    Runtime *runtime = NULL;
    Error *error = rune_runtime_load(&cfg, &runtime);
    if (error) {
        const char *msg = rune_error_to_string(error);
    
         printf("Unable to load the Rune: %s\n", msg);
    
         free(msg);
         rune_error_free(error);
         exit(1);
    }

    callback(@[[NSString stringWithFormat: @"ok"]]);
}*/

RCT_EXPORT_METHOD(loadWasm:(NSString *)base64Encoded callback:(RCTResponseSenderBlock)callback)
{
    NSData *wasm = [[NSData alloc]initWithBase64EncodedString:base64Encoded options:0];
    NSUInteger count = [wasm length] ;
    const char *bytes = [wasm bytes];
    NSString *manifest = [ObjcppBridge loadManifestWithBytes: (uint8_t *)bytes
                                                ofLength:(int)count];
    callback(@[[NSString stringWithFormat: @"%@",manifest]]);
}

RCT_EXPORT_METHOD(addInput:(NSNumber * _Nonnull)nodeId  input:(NSString *)base64Encoded  dimensions:(NSArray *)dimensions type:(NSNumber * _Nonnull)type  callback:(RCTResponseSenderBlock)callback)
{
    NSData *data = [[NSData alloc]initWithBase64EncodedString:base64Encoded options:0];
    [ObjcppBridge addInputTensor:[nodeId intValue] input:data type:[type intValue] dimensions:dimensions];
    callback(@[[NSString stringWithFormat: @"ok"]]);
}

RCT_EXPORT_METHOD(getRuneLogs:(RCTResponseSenderBlock)callback)
{
    NSString *output = [ObjcppBridge getLogs];
    callback(@[[NSString stringWithFormat: @"%@",output]]);
}

RCT_EXPORT_METHOD(runRune:(RCTResponseSenderBlock)callback)
{
    NSString *output = [ObjcppBridge callRune];
    callback(@[[NSString stringWithFormat: @"%@",output]]);
}


@end
