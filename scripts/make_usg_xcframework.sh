#!/usr/bin/env bash
set -euo pipefail

# Defaults for /Users/zakariadalil/Desktop/generate
DEVICE_LIB="./libUSGView.a"
HEADERS_DIR="./Headers"
OUTPUT_DIR="./output"
NAME="USGView"
EXISTING_SIM_LIB=""          # optional path to a prebuilt simulator .a
MIN_IOS_SIM="17.6"           # match your project's iOS Deployment Target for Simulator

while [[ $# -gt 0 ]]; do
  case "$1" in
    --device-lib) DEVICE_LIB="$2"; shift 2 ;;
    --headers)    HEADERS_DIR="$2"; shift 2 ;;
    --output-dir) OUTPUT_DIR="$2"; shift 2 ;;
    --name)       NAME="$2"; shift 2 ;;
    --sim-lib)    EXISTING_SIM_LIB="$2"; shift 2 ;;
    --min-ios)    MIN_IOS_SIM="$2"; shift 2 ;;
    -h|--help) echo "Usage: $0 [--device-lib PATH] [--headers DIR] [--output-dir DIR] [--name NAME] [--sim-lib PATH] [--min-ios 17.6]"; exit 0 ;;
    *) echo "Unknown arg: $1"; exit 1 ;;
  esac
done

[[ -f "$DEVICE_LIB" ]]  || { echo "ERROR: device lib not found: $DEVICE_LIB"; exit 1; }
[[ -d "$HEADERS_DIR" ]] || { echo "ERROR: headers dir not found: $HEADERS_DIR"; exit 1; }
command -v xcrun >/dev/null      || { echo "ERROR: xcrun not found"; exit 1; }
command -v xcodebuild >/dev/null || { echo "ERROR: xcodebuild not found"; exit 1; }

mkdir -p "$OUTPUT_DIR"
WORKDIR="$(mktemp -d "${TMPDIR:-/tmp}/usgxfw.XXXXXX")"
trap 'rm -rf "$WORKDIR"' EXIT

echo "• Workdir: $WORKDIR"
echo "• Using simulator minimum iOS: $MIN_IOS_SIM"

SIM_LIB_FAT="$WORKDIR/libUSGView_sim.a"

if [[ -n "$EXISTING_SIM_LIB" ]]; then
  [[ -f "$EXISTING_SIM_LIB" ]] || { echo "ERROR: --sim-lib not found: $EXISTING_SIM_LIB"; exit 1; }
  cp -f "$EXISTING_SIM_LIB" "$SIM_LIB_FAT"
  echo "• Using existing simulator lib: $EXISTING_SIM_LIB"
else
  echo "• Generating Objective-C simulator stubs (arm64 + x86_64)…"

  cat > "$WORKDIR/fake.m" <<'EOF'
#import <UIKit/UIKit.h>  // for CGRect

// Minimal USKit class to satisfy the linker on Simulator.
@interface USKit : NSObject
+ (instancetype)sharedInstance;
- (void)start;
- (void)stop;
- (void)registeOberver:(id)observer;
- (void)deregisterObserver:(id)observer;

// Commonly called methods in your Swift code:
- (void)setUltrasoundViewFrame:(CGRect)frame;
- (void)setCineLoop:(int32_t)frames;
- (void)visibleInformation:(BOOL)visible;
- (void)enableSavedToPhotosAlbum:(BOOL)on;
- (void)toggleLive;
- (void)setLanguage:(int)lang;
- (void)setExamination:(int)exam;
- (void)setDepthIndex:(int32_t)index;
- (void)setGain:(int32_t)value;
- (void)setEnhanceLevel:(int32_t)value;
- (void)setDynamicRange:(int32_t)value;
- (void)setHarmonic:(BOOL)on;
- (void)setCompound:(BOOL)on;
- (void)setUSMode:(int)mode;

// Getters used by your model updates:
- (int32_t)getGain;
- (int32_t)getDepth;            // mm
- (float)getCurrentBScale;
- (NSArray *)getDepthList;
- (int)probeType;
@end

@implementation USKit
+ (instancetype)sharedInstance {
  static USKit *s; static dispatch_once_t once;
  dispatch_once(&once, ^{ s = [USKit new]; });
  return s;
}
- (void)start {}
- (void)stop {}
- (void)registeOberver:(id)observer {}
- (void)deregisterObserver:(id)observer {}

- (void)setUltrasoundViewFrame:(CGRect)frame {}
- (void)setCineLoop:(int32_t)frames {}
- (void)visibleInformation:(BOOL)visible {}
- (void)enableSavedToPhotosAlbum:(BOOL)on {}
- (void)toggleLive {}
- (void)setLanguage:(int)lang {}
- (void)setExamination:(int)exam {}
- (void)setDepthIndex:(int32_t)index {}
- (void)setGain:(int32_t)value {}
- (void)setEnhanceLevel:(int32_t)value {}
- (void)setDynamicRange:(int32_t)value {}
- (void)setHarmonic:(BOOL)on {}
- (void)setCompound:(BOOL)on {}
- (void)setUSMode:(int)mode {}

- (int32_t)getGain { return 0; }
- (int32_t)getDepth { return 0; }
- (float)getCurrentBScale { return 1.0f; }
- (NSArray *)getDepthList { return @[@"1", @"10"]; }
- (int)probeType { return 0; }
@end

// Uncomment if the linker complains about a missing global:
// int US_L_en = 0;
EOF

  ARM64_FLAGS="-fobjc-arc -arch arm64  -mios-simulator-version-min=${MIN_IOS_SIM} -target arm64-apple-ios${MIN_IOS_SIM}-simulator"
  X86_FLAGS="-fobjc-arc -arch x86_64 -mios-simulator-version-min=${MIN_IOS_SIM} -target x86_64-apple-ios${MIN_IOS_SIM}-simulator"

  xcrun -sdk iphonesimulator clang ${ARM64_FLAGS} -c "$WORKDIR/fake.m" -o "$WORKDIR/fake_arm64.o"
  ar rcs "$WORKDIR/libUSGView_arm64_sim.a" "$WORKDIR/fake_arm64.o"

  xcrun -sdk iphonesimulator clang ${X86_FLAGS}  -c "$WORKDIR/fake.m" -o "$WORKDIR/fake_x86_64.o"
  ar rcs "$WORKDIR/libUSGView_x86_64_sim.a" "$WORKDIR/fake_x86_64.o"

  lipo -create \
    "$WORKDIR/libUSGView_arm64_sim.a" \
    "$WORKDIR/libUSGView_x86_64_sim.a" \
    -output "$SIM_LIB_FAT"
fi

OUT_XCFW="$OUTPUT_DIR/$NAME.xcframework"
rm -rf "$OUT_XCFW"

echo "• Creating XCFramework → $OUT_XCFW"
xcodebuild -create-xcframework \
  -library "$DEVICE_LIB"  -headers "$HEADERS_DIR" \
  -library "$SIM_LIB_FAT" -headers "$HEADERS_DIR" \
  -output "$OUT_XCFW" >/dev/null

echo "• Slices:"
find "$OUT_XCFW" -name "*.a" -print -exec file {} \;

echo "✅ Done: $OUT_XCFW"