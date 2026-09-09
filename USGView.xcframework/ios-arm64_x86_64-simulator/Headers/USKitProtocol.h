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
*    实时超声图像消息 Real time image proxy message
*    如果不需要获取实时图像，请不要实现此代理方法，因为会一直截屏耗费资源
*    @param image 图像
*    @param mode  模式
*    @param scale   比例，单位  毫米 / 像素
*    @param velocity PW速度， 单位 厘米 / 秒，此值代表PW图像区域总高度转换的速度
*  ( don't implement this proxy method if you do not need to obtain real-time images)
*/
- (void)onRealTimeImage:(UIImage*)image mode:(USMode)mode scale:(float)scale velocity:(float)velocity;

/**
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
 */
- (void)onSaveVideo:(NSData *)videoData patientJson:(NSString*)patientJson;

/**
*     click "saveReport" button， agent message returned after saving successfully
*    @param    image   report Image
*/
- (void)onSaveReport:(UIImage *)image patientJson:(NSString*)patientJson;

@end

@protocol USKitControl <NSObject>

#pragma mark - 参数调节相关
/**
*   设置检查模式 （预设参数）  ( Preset parameters )
*    @param   type   EXAM_TYPE
*/
- (void)setExamination:(EXAM_TYPE)type;

/// Get  Probe Type
- (PROBE_TYPE)probeType;

/// 模式设置 Set Mode
- (void)setUSMode:(USMode)mode;
- (USMode)getUSMode;

// 黑白超调节

/// 深度调节 0 ~ 3
- (void)setDepthIndex:(int)depthIndex;
/// 获取深度， (单位mm)
- (int)getDepth;
/// List probe  support depth
- (NSArray<NSString*>*)getDepthList;

/// 增益调节 30 ~ 105
- (void)setGain:(int)gain;
- (int)getGain;

/// 图像增强（降噪） 0 ~ 3  Image Enhance / Denoise
- (void)setEnhanceLevel:(int)enhanceLevel;
- (int)getEnhanceLevel;

/// 动态范围 40 ~ 110   image  Dynamic Range
- (void)setDynamicRange:(int)dynamicRange;
- (int)getDynamicRange;

/// Harmonic imaging
/// Whether harmonic imaging is available
- (BOOL)harmonicEnable;
///  Frequency list
- (NSArray<NSString*>*)getFreqList;

- (void)setHarmonic:(BOOL)harmonic;
/// Harmonic imaging Status
- (BOOL)getHarmonic;

/// Set focus position  0 ~ 3
- (void)setFocusPos:(int)focusPos;
- (int)getFocusPos;

/// V  gain adjustment,  gain: 0 ~ 255,   index: 0~7
- (void)setVGain:(int)gain index:(int)index;
- (int)getVGain:(int)index;

/// Whether spatial composites are supported /   image compound
- (BOOL)compoundEnable;
/// Set up a spatial compound
- (void)setCompound:(BOOL)compound;
- (BOOL)getCompound;


// Color / PDI  Adjust

/// Set Color Gain 0 ~ 100
- (void)setColorGain:(int)gain;
- (int)getColorGain;

/// To get the color ultrasound Steer series, if there are 3 levels, the following means that you can set 0 ~ 2, if <=1, it means that you can't set it
- (int)getColorSteerLevels;
- (NSArray<NSString*>*)getColorSteerList;
/// Set Color Steer
- (void)setColorSteerIndex:(int)index;
- (int)getColorSteerIndex;

///  To get the color ultrasound PRF level, if there are 3 levels, the following means that you can set 0 ~ 2, if <=1, it means that you can't set it
- (int)getColorPRFLevels;
- (NSArray<NSString*>*)getColorPRFList;
/// Set Color PRF
- (void)setColorPRFIndex:(int)index;
- (int)getColorPRFIndex;

/// Get the Wall filtering level of color ultrasound, if there are 3 levels, the following means that you can set 0 ~ 2, if <=1, it means that you can't set it
- (int)getColorWFLevels;
/// Set Color WF   ( Wall filtering  )
- (void)setColorWFIndex:(int)index;
- (int)getColorWFIndex;

/// Whether the point is within the Color sampling area or not, the converted relative coordinates need to be used
- (BOOL)onTouchInColor:(CGPoint)point;

// PW Adjust

/// Set PW Gain 0 ~ 100
- (void)setPWGain:(int)gain;
- (int)getPWGain;

/// Get the PW Steer level, if there are 3 levels, the following means that you can set 0 ~ 2, if <=1, it means that you can't set it
- (int)getPWSteerLevels;
- (NSArray<NSString*>*)getPWSteerList;
/// Set PW Steer
- (void)setPWSteerIndex:(int)index;
- (int)getPWSteerIndex;

/// Get the PW PRF level, if there are 3 levels, the following means that you can set 0 ~ 2, if <=1, it means that you can't set it
- (int)getPWPRFLevels;
- (NSArray<NSString*>*)getPWPRFList;
/// Set PW PRF
- (void)setPWPRFIndex:(int)index;
- (int)getPWPRFIndex;

/// Set PW  Volume  0 ~ 2
- (void)setPWVolumeIndex:(int)index;
- (int)getPWVolumeIndex;
- (NSArray<NSString*>*)getPWVolumeList;

/// Set PW Angle  -90 <  Angle  < 90
- (void)setPWAngle:(int)angle;
- (int)getPWAngle;


/*
       |------ 3
       |------ 4
       |------ 0
       |------ 1
       |------ 2
 */
/// Set PW Base line  0 ~ 4
- (void)setPWBaseLine:(int)baseline;
- (int)getPWBaseLine;


/// Whether the point is within the PW sampling area or not, the converted relative coordinates need to be used
- (BOOL)onTouchInPW:(CGPoint)point;


///  Get the current B scale in millimeters per pixel
- (float)getCurrentBScale;
///  Get the current PW spectral area velocity scale in (cm/s) / pixels
- (float)getCurrentPWVelocityScale;
///  Get the current PW spectral area time scale in seconds/pixel
- (float)getCurrentPWTimeScale;

@end
