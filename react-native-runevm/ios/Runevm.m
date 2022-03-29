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
    NSArray *array = [ObjcppBridge loadManifestWithBytes: bytes
                                                ofLength:count];
    Byte* manifestBytes = calloc(array.count, sizeof(Byte));

    [array enumerateObjectsUsingBlock:^(NSNumber* number, NSUInteger index, BOOL* stop){

        manifestBytes[index] = number.integerValue;
    }];
    
    NSString *manifest = [NSString stringWithUTF8String:manifestBytes];
    NSLog(@"manifest %@", manifest);
    callback(@[[NSString stringWithFormat: @"%@",manifest]]);
}


RCT_EXPORT_METHOD(runRune:(NSString *)base64Encoded lengths:(NSArray *)lengths callback:(RCTResponseSenderBlock)callback)
{
    NSData *input = [[NSData alloc]initWithBase64EncodedString:base64Encoded options:0];
    NSUInteger count = [input length] ;
    const char *bytes = [input bytes];
    NSString *output = [ObjcppBridge callRunewithInput: bytes
                                         withLengths:lengths];
    callback(@[[NSString stringWithFormat: @"%@",output]]);
}


@end
