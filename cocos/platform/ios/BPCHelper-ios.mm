//
//  BPCHelper-ios.mm
//  cocos2d_libs
//
//  Created by Brooke on 9/15/16.
//
//

#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import "BPCHelper-ios.h"
#import <UIKit/UIKit.h>
#import <CoreFoundation/CoreFoundation.h>

static bool a_isiOS9 = false;

bool cocos2d::isiOS9()
{
    static bool isInited = false;
    if (!isInited)
    {
        a_isiOS9 = [[[UIDevice currentDevice] systemVersion] compare:@"9.0" options:NSNumericSearch] != NSOrderedAscending &&
                   [[[UIDevice currentDevice] systemVersion] compare:@"10.0" options:NSNumericSearch] == NSOrderedAscending;
        isInited = true;
    }
    return a_isiOS9;
}

#endif // CC_PLATFORM_IOS