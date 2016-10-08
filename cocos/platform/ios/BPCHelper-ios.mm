//
//  BPCHelper-ios.mm
//  cocos2d_libs
//
//  Created by Brooke on 9/15/16.
//
//

#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#import "BPCHelper.h"
#import <UIKit/UIKit.h>
#import <CoreFoundation/CoreFoundation.h>

// Needed when compiling with <iOS 10.0 SDK.
#ifndef NSFoundationVersionNumber_iOS_9_0
#define NSFoundationVersionNumber_iOS_9_0 1240.1
#endif  // NSFoundationVersionNumber_iOS_9_0

// Needed when compiling with <iOS 10.0 SDK
#ifndef NSFoundationVersionNumber_iOS_9_x_Max
#define NSFoundationVersionNumber_iOS_9_x_Max 1299
#endif // NSFoundationVersionNumber_iOS_9_x_Max

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

bool cocos2d::supportsBroadcasting() {
    return isiOS10OrLater();
}

#endif // CC_PLATFORM_IOS
