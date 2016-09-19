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

static double version() {
  static auto const value(floor(NSFoundationVersionNumber));
  return value;
}

bool cocos2d::isiOS9()
{
    static bool const value{ []{
        auto const v(version());
        return v >= NSFoundationVersionNumber_iOS_9_0 &&
               v <= NSFoundationVersionNumber_iOS_9_x_Max;
    }() };
    return value;
}

bool cocos2d::isiOS10OrLater()
{
    static bool const value
    { version() > NSFoundationVersionNumber_iOS_9_x_Max };
    return value;
}

#endif // CC_PLATFORM_IOS
