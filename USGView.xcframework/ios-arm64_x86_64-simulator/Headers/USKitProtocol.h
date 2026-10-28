//
//  USKitProtocol.h
//  USGKit
//
//  Created by Qianlishun on 2024/11/6.
//

#import <Foundation/Foundation.h>

typedef enum : NSUInteger {
    PROBE_LINEAR = 0, // 线阵探头
    PROBE_CONVEX,     // 凸阵探头
    PROBE_PA,         // 凸阵带相控
    PROBE_UNKNOW
} PROBE_TYPE;

typedef enum : NSUInteger {
    USMode_B,
    USMode_BM,
    USMode_Color,
    USMode_PDI,
    USMode_PW
} USMode;

typedef enum : NSUInteger {
    // 线阵
    EXAM_THYROID = 100,  //甲状腺
    EXAM_SMALL_PARTS,    //小器官
    EXAM_PEDIATRICS,     //儿科
    EXAM_VASCULAR,       //血管
    EXAM_CAROTID,        //颈动脉
    EXAM_BREAST,         //乳腺
    EXAM_MSK,            //肌骨
    
    // 凸阵
    EXAM_ABDOMEN,       //腹部
    EXAM_GYNECOLOGY,    //妇科
    EXAM_OBSTETRIC,     //产科
    EXAM_CARDIAC,       //心脏
    EXAM_UROLOGY,       //泌尿系统
    EXAM_KIDNEY         //肾脏
    
} EXAM_TYPE;

@protocol USKitProtocol <NSObject>
@optional
/**
*     探头连接回调 probe connected / disconected callback
*    @param    probeSSID    探头SSID Probe  SSID
*    @param    connected     连接成功 / 断开连接 ，YES:  connected ，NO:  probe disconnect
*/
- (void)onProbe:(NSString*)probeSSID Connected:(BOOL)connected;

/**
*     探头运行回调
*    @param    isLive     运行  / 冻结 ，YES:  Live ，NO:  Freeze
*/
- (void)onProbeLive:(BOOL)isLive;

/**
*    实时超声图像消息 (屏幕尺寸版本) Real time image proxy message (size same screen)
*    如果不需要获取实时图像，请不要实现此代理方法，因为会一直截屏耗费资源
*    @param image 图像
*    @param mode  模式
*    @param scale   比例，单位  毫米 / 像素 (mm/pixel)
*    @param velocity PW速度， 单位 厘米 / 秒 (cm/s) ，此值代表PW图像区域总高度转换的速度
*  ( don't implement this proxy method if you do not need to obtain real-time images)
*/
- (void)onRealTimeImage:(UIImage*)image mode:(USMode)mode scale:(float)scale velocity:(float)velocity;

/**
*    实时超声图像消息 (裁剪版本) (fit cut size)
*    如果不需要获取实时图像，请不要实现此代理方法，因为会一直截屏耗费资源
*    Real Time  B mode Origin Capture ，only B Mode image and scale  ( mm / pixel )
 ( don't implement this proxy method if you do not need to obtain real-time images)
*/
- (void)onRawBModeCapture:(UIImage*)image scale:(float)scale;


/// 双头探头切换消息
- (void)onSwitchProbe:(PROBE_TYPE)type;

/**
*   click "saveImage" button， agent message returned after saving successfully
*    @param    image   ultrasound Image
*/
- (void)onSaveImage:(UIImage *)image patientJson:(NSString*)patientJson;

/**
 *   click "saveVideo" button， agent message returned after saving successfully
 *    @param    videoData   ultrasound Video
 *   NSString *videoPath = [[NSHomeDirectory() stringByAppendingPathComponent:@"Documents"]stringByAppendingPathComponent:@"test.mp4"];
 *   [videoData writeToFile:videoPath atomically:YES];
 */
- (void)onSaveVideo:(NSData *)videoData patientJson:(NSString*)patientJson;

/**
*     click "saveReport" button， agent message returned after saving successfully
*    @param    image   report Image
*/
- (void)onSaveReport:(UIImage *)image patientJson:(NSString*)patientJson;

@end

@protocol USKitControl <NSObject>
@optional

- (NSString*)probeSN;

- (void)cineLoopPlay;
- (void)cineLoopPause;
- (BOOL)cineLoopIsPlay;
- (void)setCineLoopIndex:(NSUInteger)index;

- (BOOL)saveImage;
- (BOOL)saveVideo;

#pragma mark - 参数调节相关
/**
*   设置检查模式
*    @param   type   EXAM_TYPE
*/
- (void)setExamination:(EXAM_TYPE)type;

- (PROBE_TYPE)probeType;

/// 模式设置
- (void)setUSMode:(USMode)mode;
- (USMode)getUSMode;

// 黑白超调节

/// 深度调节 0 ~ 3
- (void)setDepthIndex:(int)depthIndex;
- (int)getDepthIndex;
/// 获取 当前深度， (单位mm)
- (int)getDepth;
/// 获取深度列表  (mm)
- (NSArray<NSString*>*)getDepthList;

/// 增益调节 30 ~ 105
- (void)setGain:(int)gain;
- (int)getGain;

/// 图像增强（降噪） 0 ~ 3
- (void)setEnhanceLevel:(int)enhanceLevel;
- (int)getEnhanceLevel;

/// 动态范围 40 ~ 110
- (void)setDynamicRange:(int)dynamicRange;
- (int)getDynamicRange;

/// 谐波成像
- (BOOL)harmonicEnable;
- (NSArray<NSString*>*)getFreqList;
- (void)setHarmonic:(BOOL)harmonic;
- (BOOL)getHarmonic;

/// 焦点位置 0 ~ 3
- (void)setFocusPos:(int)focusPos;
- (int)getFocusPos;

/// 局部增益调节 gain: 0 ~ 255,   index: 0~7
- (void)setVGain:(int)gain index:(int)index;
- (int)getVGain:(int)index;

/// 是否支持空间复合
- (BOOL)compoundEnable;
/// 设置空间复合
- (void)setCompound:(BOOL)compound;
- (BOOL)getCompound;


// 彩超调节  Color / PDI

/// 采样框调节， YES：Move， NO：Size
- (void)setColorSampleQuadsIsMove:(BOOL)isMove;
- (BOOL)getColorSampleQuadsIsMove;

/// 彩超增益 0 ~ 100
- (void)setColorGain:(int)gain;
- (int)getColorGain;

/// 获取彩超 Steer 级数， 如果有3级，下面代表可以设置 0 ～ 2， 如果<=1，则代表不能设置
- (int)getColorSteerLevels;
- (NSArray<NSString*>*)getColorSteerList;
/// 设置彩超 Steer
- (void)setColorSteerIndex:(int)index;
- (int)getColorSteerIndex;

/// 获取彩超 PRF 级数， 如果有3级，下面代表可以设置 0 ～ 2， 如果<=1，则代表不能设置
- (int)getColorPRFLevels;
- (NSArray<NSString*>*)getColorPRFList;
/// 设置彩超 PRF
- (void)setColorPRFIndex:(int)index;
- (int)getColorPRFIndex;

/// 获取彩超 WF 级数， 如果有3级，下面代表可以设置 0 ～ 2， 如果<=1，则代表不能设置
- (int)getColorWFLevels;
/// 设置彩超 WF
- (void)setColorWFIndex:(int)index;
- (int)getColorWFIndex;

/// 点是否在 Color 采样区内，需要使用转换过的相对坐标
- (BOOL)onTouchInColor:(CGPoint)point;

// PW 调节

/// PW增益 0 ~ 100
- (void)setPWGain:(int)gain;
- (int)getPWGain;

/// 获取PW Steer 级数， 如果有3级，下面代表可以设置 0 ～ 2， 如果<=1，则代表不能设置
- (int)getPWSteerLevels;
- (NSArray<NSString*>*)getPWSteerList;
/// 设置PW Steer
- (void)setPWSteerIndex:(int)index;
- (int)getPWSteerIndex;

/// 获取PW PRF 级数， 如果有3级，下面代表可以设置 0 ～ 2， 如果<=1，则代表不能设置
- (int)getPWPRFLevels;
- (NSArray<NSString*>*)getPWPRFList;
/// 设置PW PRF
- (void)setPWPRFIndex:(int)index;
- (int)getPWPRFIndex;

/// 设置PW  Volume  0 ~ 2
- (void)setPWVolumeIndex:(int)index;
- (int)getPWVolumeIndex;
- (NSArray<NSString*>*)getPWVolumeList;

/// 设置PW Angle  -90 <  Angle  < 90
- (void)setPWAngle:(int)angle;
- (int)getPWAngle;


/*
       |------ 3
       |------ 4
       |------ 0
       |------ 1
       |------ 2
 */
/// 设置 PW 基线  0 ~ 4
- (void)setPWBaseLine:(int)baseline;
- (int)getPWBaseLine;


/// 点是否在 PW 采样区内，需要使用转换过的相对坐标
- (BOOL)onTouchInPW:(CGPoint)point;


///  获取当前黑白超区域比例 单位： 毫米 / 像素
- (float)getCurrentBScale;
///  获取当前PW频谱区域速度比例  单位：（  厘米 / 秒） / 像素
- (float)getCurrentPWVelocityScale;
///  获取当前PW频谱区域时间比例  单位：  秒 / 像素
- (float)getCurrentPWTimeScale;

/// 设置图像左右翻转 （非BM模式）
- (void)setHorizontalReversal:(BOOL)reversal;
/// 设置图像上下翻转 （非BM模式）
- (void)setVerticalReversal:(BOOL)reversal;

@end
