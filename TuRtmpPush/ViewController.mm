//
//  ViewController.m
//  TuRtmpPush
//
//  Created by zhantu wu on 2025/5/18.
//

#import "ViewController.h"
#include "socketconnect.hpp"
#include "handshake.hpp"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
//    nginx
//    http://localhost:8080/
    
    socketconnect sc;
    sc.connectServer();
    handshake hs(sc.socketVal);
    hs.threehandshake();
}


@end
