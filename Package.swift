// swift-tools-version:5.9
import PackageDescription

let package = Package(
    name: "SonostarSDK",
    products: [
        .library(name: "USGView", targets: ["USGView"]),
    ],
    targets: [
        .binaryTarget(name: "USGView", path: "USGView.xcframework"),
    ]
)
