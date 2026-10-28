//
//  USView.h
//  
//
//  Created by mrq on 2020/8/21.
//  Copyright © 2020 MrQ. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "USKitProtocol.h"

typedef enum : NSUInteger {
    US_L_en,
    US_L_zh
} USKit_LANGUAGE;

@interface USKit : NSObject<USKitControl>

/// 版本号
+ (NSString*)getVersion;

/// 创建单例
+ (USKit*)sharedInstance;

/// 注册观察者
- (void)registeOberver: (id)observer;
/// 注销观察者
- (void)deregisterObserver: (id)observer;

/// 初始化视图，带默认UI Set USView frame with default UI
- (void)setViewFrame:(CGRect)frame;

/**
*    初始化视图，带默认UI
*    Set USView frame and screen type  with default UI
*    @param frame  view frame
*    @param verticalScreen   Vertical (YES) or Horizontal (NO) screen,  default is Horizontal;
*/
- (void)setViewFrame:(CGRect)frame verticalScreen:(BOOL)verticalScreen;

/// 获取总视图 Overall View
- (UIView*)getView;

/// 获取超声视图  only ultrasound View
- (UIView*)getUltrasoundView;


/// 初始化视图，只有超声视图，不含其他 Set  ultrasoundView frame  only ultrasound view, excluding others
- (void)setUltrasoundViewFrame:(CGRect)frame;

- (void)deallocView;

#pragma mark - 预设

/// set app version 设置显示的版本号
- (void)setAppVersion:(NSString*)version;

/**
  设置语言
 *    @param language   USKit_LANGUAGE
 */
- (void)setLanguage:(USKit_LANGUAGE)language;

/**
 是否存储图片和视频到相册，默认为不存储，你可以在保存回调里自行保存
 Whether to save the image / video to the system album (default is NO,  you can be saved by the save callback)
 */
- (void)enableSavedToPhotosAlbum:(BOOL)enable;

/**
*   设置病例信息
*    @param    patientJson   patient information
    like:
    {
     "Patient_ID":"20190425001",
     "Patient_Name":"Jack",
     "Patient_Gender":"Female",
     "Patient_Birth":"20120425"
     }
*/
- (void)setPatientInfo:(NSString*)patientJson;

/**
* 是否显示信息 ( Patient,  Params)
*    @param   visible   default YES
*/
- (void)visibleInformation:(BOOL)visible;

- (void)visiblePatientInformation:(BOOL)visible;
- (void)visibleParamsInformation:(BOOL)visible;

/**
* 是否显示比例尺
*    @param   visible   default YES
*/
- (void)visibleScaleView:(BOOL)visible;

/**
*   设置回放帧数，默认为100
*    @param   cineLoop 100~1000 , default 100
*/
- (void)setCineLoop:(int)cineLoop;

/**
*   设置信道
*    @param   wirelessChannel   2.4G： 1~13  ;
                        5G： 40、44、48、149、153、157、161、165
*/
- (void)setWirelessChannel:(int)wirelessChannel;

/// 启动
- (void)start;

/// 停止并释放内存
- (void)stop;

/// 探头 运行 / 冻结 状态切换
- (void)toggleLive;

/// 连接状态 Connect status
- (BOOL)isConn;

/// 运行状态 Live status
- (BOOL)isLive;

- (NSString*)connProbeSSID;

/**
 NSString *probeSSID = [self.usKit connProbeSSID];
 [self.usKit addWiFiWhiteListWithProbeSSID:probeSSID];
 */
- (void)addWiFiWhiteListWithProbeSSID:(NSString*)probeSSID;

@end

