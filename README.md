# Sonostar SDK

The Sonostar `USGView.xcframework`, packaged for Swift Package Manager so the
[deepecho-mobile-app](https://github.com/Deepecho-org/deepecho-mobile-app)
does not have to carry a vendor binary in its own history.

This repository is private because the Sonostar SDK is not ours to redistribute.

## Contents

| Path | What it is |
| --- | --- |
| `USGView.xcframework/ios-arm64/libUSGView.a` | The real Sonostar SDK, from the vendor. Device only. |
| `USGView.xcframework/ios-arm64_x86_64-simulator/libUSGView_sim.a` | A hand-written no-op stub. Defines `USKit` and nothing else. |
| `scripts/make_usg_xcframework.sh` | Rebuilds the XCFramework from a vendor `libUSGView.a`. |

**The simulator slice is a stub.** It exists only so the app links and runs in
the Simulator; every call through it does nothing. A green simulator build says
nothing about whether Sonostar works — that requires a physical probe and a
device build.

## Consuming it

```swift
// Tuist/Package.swift
.package(url: "git@github.com:zikossnake/sonostar-sdk.git", exact: "1.0.0"),
```

```swift
// Project.swift
.external(name: "USGView"),
```

Pin with `exact:`. The app's `Tuist/Package.resolved` is gitignored, so a
version range would let different machines link different SDK builds without
anyone noticing.

## Headers

`USKit.h` and `USKitProtocol.h` exist twice: here, inside the XCFramework, and
in the app at `ProjectAurora/Core/Probes/Sonostar/USGKit/`, where the app's
bridging header imports them via `HEADER_SEARCH_PATHS`. The app does not read
the copies in this repository. **Keep them in sync** — if a new SDK version
changes a signature, update both.

## Updating to a new vendor SDK

```bash
./scripts/make_usg_xcframework.sh \
    --device-lib <path>/libUSGView.a \
    --headers USGView.xcframework/ios-arm64/Headers \
    --min-ios 17.6
```

Then commit, tag the new version, and bump the `exact:` pin in the app. Update
the app's loose header copies in the same pass if the headers changed.
