//
//  ViewController.m
//  PebbleDataTransfer
//
//  Created by Matt Hartenbower on 10/3/15.
//  Copyright (c) 2015 Matt Hartenbower. All rights reserved.
//

#import "ViewController.h"
#import <PebbleKit/PebbleKit.h>
#import "AppDelegate.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    AppDelegate *appDelegate = (AppDelegate *)[[UIApplication sharedApplication] delegate];
    [appDelegate.currentWatch appMessagesLaunch:^(PBWatch *watch, NSError *error) {
        if (!error) {
            NSLog(@"Successfully launched app.");
        }
        else {
            NSLog(@"Error launching app - Error: %@", error);
        }
    }
     ];
    
    if(1)
    {
        [appDelegate.currentWatch appMessagesAddReceiveUpdateHandler:^BOOL(PBWatch *watch, NSDictionary *update) {
            for(NSString *key in [update allKeys]) {
                NSLog(@"Hello %@",[update objectForKey:key]);
            }
            return 5;
        }];
    }
    
    
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
