//
//  AppDelegate.h
//  PebbleDataTransfer
//
//  Created by Matt Hartenbower on 10/3/15.
//  Copyright (c) 2015 Matt Hartenbower. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <PebbleKit/PebbleKit.h>


@interface AppDelegate : UIResponder <UIApplicationDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) PBWatch *currentWatch;


@end

