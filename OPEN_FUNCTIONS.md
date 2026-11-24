# Open functions: Mario Party 6 (US)

Generated from the objdiff report of a build of this repository. Regenerate rather than edit
by hand.

**Columns.** *match*: objdiff's score for the function as it builds now (100% is identical).
*bytes*: size of the original function. Functions at 0% either have no C yet or have C whose
shape differs from the original (functions merged or split, wrong inlining).

**Summary:** 2,612 functions short of 100%: 19 at 95% or better, 8 at 80-95%, 10 below 80%, 2,575 at 0%; in 191 units.

## Started, not matching yet

Best first: near the top, the remaining difference is usually register allocation, instruction
order or one wrong type.

| match | function | unit | bytes |
|--:|---|---|--:|
| 99.52% | `MgCallBattleExec` | `main/board/mgcall` | 3,312 |
| 99.35% | `_BodyMeshCol` | `main/game/colman` | 5,520 |
| 99.17% | `DiceNumObjBendOMExec` | `main/board/dice` | 968 |
| 98.84% | `PlayerTurn` | `main/board/player` | 1,380 |
| 98.73% | `mbAudGuidePlay` | `main/board/audio` | 632 |
| 98.61% | `_BodyMeshCylCol` | `main/game/colman` | 4,608 |
| 98.59% | `MgRouletteExec` | `main/board/mgcall` | 3,412 |
| 98.21% | `MasuDraw` | `main/board/masu` | 2,900 |
| 97.84% | `DiceRun` | `main/board/player` | 1,852 |
| 97.71% | `DiceObjEffCreate` | `main/board/dice` | 872 |
| 97.61% | `WipeImageDraw` | `main/board/wipe` | 1,676 |
| 96.72% | `mbPos2Dto3D` | `main/board/math` | 244 |
| 96.69% | `mbev_MgCallSingleKoopa` | `main/board/mgcall` | 1,448 |
| 96.52% | `PlayerMoveOMExec` | `main/board/player` | 916 |
| 96.10% | `ev_DiceZoromeCoin` | `main/board/dice` | 820 |
| 95.88% | `Last5RouletteCreate` | `main/board/last5` | 680 |
| 95.85% | `ev_DiceZorome` | `main/board/dice` | 868 |
| 95.32% | `ObjectCullUpdate` | `main/board/math` | 1,796 |
| 95.24% | `mbMusBoardFadeOut` | `main/board/audio` | 1,008 |
| 94.64% | `Hu3DTevStageTexSet` | `main/game/hsfdraw` | 9,764 |
| 94.23% | `ev_Branch` | `main/board/branch` | 2,288 |
| 91.78% | `MetalEffectCreate` | `main/board/player` | 1,216 |
| 91.67% | `mbPlayerMoveMain` | `main/board/player` | 1,384 |
| 89.77% | `MgCallBattleCoinGet` | `main/board/mgcall` | 352 |
| 86.58% | `MasuFind` | `main/board/masu` | 1,580 |
| 86.11% | `ConfigPadMain` | `main/board/config` | 2,444 |
| 81.88% | `mbMathDistScale` | `main/board/math` | 596 |
| 79.46% | `GetBiriQEffectRadius` | `main/board/player` | 1,032 |
| 79.12% | `ObjectCullHook` | `main/board/math` | 728 |
| 75.45% | `ParticleDraw` | `main/board/effect` | 2,668 |
| 74.26% | `mbPlayerInit` | `main/board/player` | 2,284 |
| 71.14% | `ev_OpeningSingle` | `main/board/opening` | 4,244 |
| 62.86% | `mbBezierCalcV` | `main/board/math` | 140 |
| 62.86% | `mbBezierCalcVList` | `main/board/math` | 140 |
| 62.75% | `mbNormPosto3D` | `main/board/math` | 404 |
| 54.72% | `mbHermiteCalcV` | `main/board/math` | 212 |
| 15.25% | `ParticleColorCopy` | `main/board/effect` | 468 |

## By unit

Every unit with a function short of 100%, largest remaining code first.

| unit | source | open | functions | open bytes |
|---|---|--:|--:|--:|
| `main/board/capevent` | (no source yet) | 237 | 237 | 85,756 |
| `main/board/capsule` | (no source yet) | 165 | 165 | 83,660 |
| `main/board/capspecial` | (no source yet) | 44 | 44 | 59,204 |
| `main/board/single` | (no source yet) | 58 | 58 | 39,152 |
| `main/board/snpc` | (no source yet) | 85 | 85 | 38,292 |
| `main/board/capthrow` | (no source yet) | 19 | 19 | 36,016 |
| `main/dolphin/thp/THPDec` | (no source yet) | 19 | 19 | 30,016 |
| `main/board/capmove` | (no source yet) | 24 | 24 | 28,520 |
| `main/board/captrap` | (no source yet) | 20 | 20 | 27,200 |
| `main/gssdk_lib/asrpho/rec1600/spi1600` | (no source yet) | 57 | 57 | 22,404 |
| `bootDll/REL/bootDll/opening` | (no source yet) | 37 | 37 | 21,272 |
| `main/gssdk_lib/asrpho/common/blocks/nbestdp` | (no source yet) | 25 | 25 | 20,688 |
| `main/board/capselect` | (no source yet) | 32 | 32 | 13,408 |
| `main/msm/msmstream` | (no source yet) | 28 | 28 | 12,944 |
| `main/board/shopevent` | (no source yet) | 13 | 13 | 12,252 |
| `bootDll/REL/bootDll/boot` | (no source yet) | 12 | 12 | 11,684 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/dist16` | (no source yet) | 21 | 21 | 11,660 |
| `main/gssdk_lib/asrpho/rec1600/crsptrch` | (no source yet) | 11 | 11 | 10,332 |
| `main/game/colman` | (no source yet) | 2 | 43 | 10,128 |
| `main/dolphin/dvd/dvd` | (no source yet) | 47 | 47 | 10,120 |
| `main/board/player` | (no source yet) | 7 | 165 | 10,064 |
| `main/gssdk_lib/asrpho/rec1600/creaspch` | (no source yet) | 12 | 12 | 9,932 |
| `main/game/hsfdraw` | (no source yet) | 1 | 54 | 9,764 |
| `main/gssdk_lib/gsapi/gsapi` | (no source yet) | 30 | 30 | 9,516 |
| `main/gssdk_lib/asrpho/asrspi` | (no source yet) | 22 | 22 | 9,196 |
| `main/gssdk_lib/asrpho/common/blocks/isoword` | (no source yet) | 15 | 15 | 9,068 |
| `main/msm/msmsys` | (no source yet) | 23 | 23 | 8,832 |
| `main/dolphin/vi/vi` | (no source yet) | 23 | 23 | 8,660 |
| `main/board/mgcall` | (no source yet) | 4 | 38 | 8,524 |
| `main/dolphin/mic/mic` | (no source yet) | 41 | 41 | 8,224 |
| `main/gssdk_lib/asrpho/rec1600/userword` | (no source yet) | 21 | 21 | 8,064 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/vad` | (no source yet) | 11 | 11 | 7,844 |
| `main/gssdk_lib/asrpho/common/tos/tinyos` | (no source yet) | 25 | 25 | 7,568 |
| `main/dolphin/ar/ar` | (no source yet) | 12 | 12 | 7,064 |
| `main/dolphin/os/OSThread` | (no source yet) | 26 | 26 | 7,044 |
| `main/gssdk_lib/asrpho/rec1600/creatree` | (no source yet) | 15 | 15 | 6,948 |
| `main/dolphin/exi/EXIBios` | (no source yet) | 23 | 23 | 6,620 |
| `main/dolphin/si/SIBios` | (no source yet) | 25 | 25 | 6,584 |
| `main/dolphin/pad/Pad` | (no source yet) | 19 | 19 | 6,472 |
| `main/gssdk_lib/asrpho/rec1600/train` | (no source yet) | 9 | 9 | 6,216 |
| `main/gssdk_lib/asrpho/rec1600/creaspt` | (no source yet) | 4 | 4 | 5,868 |
| `main/msm/msmse` | (no source yet) | 18 | 18 | 5,720 |
| `main/gssdk_lib/asrpho/common/blocks/pitchdp` | (no source yet) | 11 | 11 | 5,568 |
| `main/dolphin/card/CARDBios` | (no source yet) | 27 | 27 | 5,496 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/shs_vuv` | (no source yet) | 8 | 8 | 5,404 |
| `main/dolphin/gx/GXAttr` | (no source yet) | 16 | 16 | 5,304 |
| `main/gssdk_lib/asrpho/rec1600/convert` | (no source yet) | 6 | 6 | 5,272 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/pitchco` | (no source yet) | 13 | 13 | 5,112 |
| `main/dolphin/gx/GXInit` | (no source yet) | 6 | 6 | 5,004 |
| `main/gssdk_lib/asrpho/common/blocks/dpgenuw` | (no source yet) | 12 | 12 | 4,912 |
| `main/dolphin/card/CARDUnlock` | (no source yet) | 6 | 6 | 4,704 |
| `main/gssdk_lib/asrpho/common/blocks/dpscruw` | (no source yet) | 14 | 14 | 4,532 |
| `main/board/masu` | (no source yet) | 2 | 119 | 4,480 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/specsub` | (no source yet) | 8 | 8 | 4,344 |
| `main/gssdk_lib/asrpho/rec1600/creasp` | (no source yet) | 3 | 3 | 4,316 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/trigglr` | (no source yet) | 5 | 5 | 4,292 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/gender` | (no source yet) | 12 | 12 | 4,264 |
| `main/board/math` | (no source yet) | 8 | 56 | 4,260 |
| `main/board/opening` | (no source yet) | 1 | 22 | 4,244 |
| `main/dolphin/gx/GXTexture` | (no source yet) | 20 | 20 | 4,196 |
| `main/gssdk_lib/asrpho/common/tos/mqueue` | (no source yet) | 16 | 16 | 4,140 |
| `main/dolphin/gx/GXPerf` | (no source yet) | 12 | 12 | 3,952 |
| `main/dolphin/card/CARDCheck` | (no source yet) | 7 | 7 | 3,944 |
| `main/msm/msmmus` | (no source yet) | 14 | 14 | 3,768 |
| `main/dolphin/dvd/dvdlow` | (no source yet) | 21 | 21 | 3,708 |
| `main/board/dice` | (no source yet) | 4 | 68 | 3,528 |
| `main/dolphin/demo/DEMOStats` | (no source yet) | 3 | 3 | 3,416 |
| `main/dolphin/mic/m2s` | (no source yet) | 14 | 14 | 3,408 |
| `main/dolphin/gx/GXFrameBuf` | (no source yet) | 15 | 15 | 3,296 |
| `main/gssdk_lib/asrpho/common/blocks/fftmod` | (no source yet) | 6 | 6 | 3,204 |
| `main/gssdk_lib/asrpho/common/ctxdata/langdata` | (no source yet) | 76 | 76 | 3,192 |
| `main/board/effect` | (no source yet) | 2 | 35 | 3,136 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/acne` | (no source yet) | 8 | 8 | 3,108 |
| `bootDll/REL/runtime` | (no source yet) | 109 | 109 | 3,100 |
| `main/dolphin/os/OSRtc` | (no source yet) | 18 | 18 | 3,068 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/window` | (no source yet) | 5 | 5 | 3,068 |
| `main/dolphin/os/OS` | (no source yet) | 17 | 17 | 2,832 |
| `main/dolphin/card/CARDMount` | (no source yet) | 8 | 8 | 2,820 |
| `main/dolphin/gx/GXDraw` | (no source yet) | 2 | 2 | 2,632 |
| `main/dolphin/os/OSAlloc` | (no source yet) | 9 | 9 | 2,552 |
| `main/dolphin/os/OSLink` | (no source yet) | 8 | 8 | 2,540 |
| `main/gssdk_lib/gsapi/ctxfuncs` | (no source yet) | 15 | 15 | 2,500 |
| `main/dolphin/gx/GXMisc` | (no source yet) | 23 | 23 | 2,492 |
| `main/board/config` | (no source yet) | 1 | 51 | 2,444 |
| `main/dolphin/mtx/mtx` | (no source yet) | 17 | 17 | 2,424 |
| `main/dolphin/os/OSExec` | (no source yet) | 8 | 8 | 2,400 |
| `main/dolphin/ai/ai` | (no source yet) | 20 | 20 | 2,304 |
| `main/board/branch` | (no source yet) | 1 | 17 | 2,288 |
| `main/gssdk_lib/asrpho/common/blocks/fft_maye` | (no source yet) | 2 | 2 | 2,288 |
| `main/dolphin/dvd/dvdfs` | (no source yet) | 11 | 11 | 2,268 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/mel` | (no source yet) | 5 | 5 | 2,244 |
| `main/dolphin/dsp/dsp_task` | (no source yet) | 5 | 5 | 2,180 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/voicing` | (no source yet) | 6 | 6 | 2,168 |
| `main/dolphin/os/OSContext` | (no source yet) | 13 | 13 | 2,160 |
| `main/dolphin/os/OSInterrupt` | (no source yet) | 11 | 11 | 2,156 |
| `main/dolphin/gx/GXFifo` | (no source yet) | 18 | 18 | 2,100 |
| `main/Runtime.PPCEABI.H/runtime` | (no source yet) | 81 | 81 | 2,044 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/mtx` | (no source yet) | 12 | 12 | 2,044 |
| `main/dolphin/card/CARDFormat` | (no source yet) | 3 | 3 | 2,032 |
| `main/gssdk_lib/gsapi/callbacks` | (no source yet) | 7 | 7 | 2,016 |
| `main/dolphin/os/OSAlarm` | (no source yet) | 9 | 9 | 2,008 |
| `main/dolphin/gx/GXTransform` | (no source yet) | 16 | 16 | 1,836 |
| `main/dolphin/gx/GXTev` | (no source yet) | 16 | 16 | 1,824 |
| `main/dolphin/gx/GXBump` | (no source yet) | 11 | 11 | 1,768 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/subsamp` | (no source yet) | 5 | 5 | 1,768 |
| `main/dolphin/gx/GXLight` | (no source yet) | 13 | 13 | 1,760 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/smoother` | (no source yet) | 3 | 3 | 1,684 |
| `main/board/wipe` | (no source yet) | 1 | 33 | 1,676 |
| `main/dolphin/os/OSCache` | (no source yet) | 19 | 19 | 1,656 |
| `main/board/audio` | (no source yet) | 2 | 49 | 1,640 |
| `main/dolphin/gx/GXPixel` | (no source yet) | 12 | 12 | 1,592 |
| `main/dolphin/demo/DEMOPuts` | (no source yet) | 3 | 3 | 1,584 |
| `main/dolphin/mtx/quat` | (no source yet) | 11 | 11 | 1,576 |
| `main/dolphin/card/CARDOpen` | (no source yet) | 7 | 7 | 1,508 |
| `main/gssdk_lib/asrpho/common/blocks/pitchwin` | (no source yet) | 5 | 5 | 1,476 |
| `main/dolphin/exi/EXIUart` | (no source yet) | 4 | 4 | 1,464 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/combiner` | (no source yet) | 4 | 4 | 1,440 |
| `main/dolphin/os/OSError` | (no source yet) | 3 | 3 | 1,408 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/slidhist` | (no source yet) | 8 | 8 | 1,340 |
| `main/dolphin/thp/THPAudio` | (no source yet) | 3 | 3 | 1,328 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/vq1500` | (no source yet) | 9 | 9 | 1,264 |
| `main/gssdk_lib/asrpho/common/blocks/exev_dp` | (no source yet) | 6 | 6 | 1,240 |
| `main/dolphin/card/CARDStat` | (no source yet) | 4 | 4 | 1,224 |
| `main/dolphin/os/OSReset` | (no source yet) | 7 | 7 | 1,156 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/dctlift` | (no source yet) | 4 | 4 | 1,156 |
| `main/dolphin/os/OSTime` | (no source yet) | 6 | 6 | 1,148 |
| `main/dolphin/card/CARDRead` | (no source yet) | 4 | 4 | 1,140 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/median` | (no source yet) | 4 | 4 | 1,136 |
| `main/dolphin/card/CARDBlock` | (no source yet) | 6 | 6 | 1,028 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/statio` | (no source yet) | 4 | 4 | 956 |
| `main/gssdk_lib/asrpho/common/blocks/delaybl` | (no source yet) | 4 | 4 | 940 |
| `main/dolphin/card/CARDCreate` | (no source yet) | 4 | 4 | 928 |
| `main/dolphin/ar/arq` | (no source yet) | 5 | 5 | 924 |
| `main/dolphin/os/OSResetSW` | (no source yet) | 2 | 2 | 908 |
| `main/dolphin/mtx/vec` | (no source yet) | 12 | 12 | 904 |
| `main/gssdk_lib/asrpho/common/blocks/stacker` | (no source yet) | 4 | 4 | 900 |
| `main/dolphin/card/CARDWrite` | (no source yet) | 4 | 4 | 892 |
| `main/dolphin/os/OSStopwatch` | (no source yet) | 6 | 6 | 880 |
| `main/gssdk_lib/common/rsrc/rsrc` | (no source yet) | 3 | 3 | 856 |
| `main/dolphin/gx/GXGeometry` | (no source yet) | 9 | 9 | 800 |
| `main/dolphin/os/OSFont` | (no source yet) | 3 | 3 | 788 |
| `main/dolphin/os/__start` | (no source yet) | 7 | 7 | 772 |
| `main/dolphin/os/OSMemory` | (no source yet) | 8 | 8 | 752 |
| `main/dolphin/pad/Padclamp` | (no source yet) | 2 | 2 | 692 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/lkahead` | (no source yet) | 4 | 4 | 684 |
| `main/board/last5` | (no source yet) | 1 | 10 | 680 |
| `main/dolphin/os/OSAudioSystem` | (no source yet) | 2 | 2 | 660 |
| `main/dolphin/card/CARDRdwr` | (no source yet) | 4 | 4 | 656 |
| `main/dolphin/card/CARDDir` | (no source yet) | 4 | 4 | 612 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/mtxopt` | (no source yet) | 3 | 3 | 600 |
| `main/gssdk_lib/asrpho/common/blocks/undersam` | (no source yet) | 4 | 4 | 600 |
| `main/dolphin/dvd/fstload` | (no source yet) | 2 | 2 | 576 |
| `main/msm/msmmem` | (no source yet) | 3 | 3 | 560 |
| `main/dolphin/os/OSMutex` | (no source yet) | 4 | 4 | 540 |
| `main/dolphin/dsp/dsp` | (no source yet) | 9 | 9 | 536 |
| `main/dolphin/os/OSMessage` | (no source yet) | 3 | 3 | 516 |
| `main/dolphin/gx/GXDisplayList` | (no source yet) | 3 | 3 | 512 |
| `main/dolphin/card/CARDDelete` | (no source yet) | 3 | 3 | 508 |
| `main/dolphin/dvd/dvdqueue` | (no source yet) | 5 | 5 | 504 |
| `main/gssdk_lib/asrpho/common/fastallo/fastallo` | (no source yet) | 3 | 3 | 492 |
| `main/gssdk_lib/asrpho/common/blocks/flfxblks/genfilt` | (no source yet) | 4 | 4 | 464 |
| `main/gssdk_lib/asrpho/common/ctxdata/ctxdata` | (no source yet) | 21 | 21 | 464 |
| `main/gssdk_lib/asrpho/common/blocks/flblocks/logexp` | (no source yet) | 2 | 2 | 456 |
| `main/gssdk_lib/gsapi/sid/sid` | (no source yet) | 8 | 8 | 452 |
| `main/dolphin/mtx/mtxvec` | (no source yet) | 4 | 4 | 444 |
| `main/dolphin/dvd/dvderror` | (no source yet) | 2 | 2 | 408 |
| `main/dolphin/mtx/mtx44` | (no source yet) | 2 | 2 | 360 |
| `main/dolphin/mtx/psmtx` | (no source yet) | 2 | 2 | 356 |
| `main/dolphin/os/OSSemaphore` | (no source yet) | 3 | 3 | 296 |
| `main/dolphin/base/PPCArch` | (no source yet) | 23 | 23 | 288 |
| `main/gssdk_lib/gsapi/wrddata` | (no source yet) | 2 | 2 | 280 |
| `main/dolphin/si/SISamplingRate` | (no source yet) | 2 | 2 | 264 |
| `main/dolphin/dvd/dvdidutils` | (no source yet) | 1 | 1 | 248 |
| `main/msm/msmfio` | (no source yet) | 4 | 4 | 240 |
| `main/dolphin/db/db` | (no source yet) | 5 | 5 | 236 |
| `main/dolphin/os/__ppc_eabi_init` | (no source yet) | 5 | 5 | 236 |
| `main/gssdk_lib/common/safeh/safeh` | (no source yet) | 2 | 2 | 212 |
| `main/gssdk_lib/gsapi/extaudio` | (no source yet) | 6 | 6 | 200 |
| `main/dolphin/card/CARDNet` | (no source yet) | 1 | 1 | 196 |
| `main/dolphin/os/OSSync` | (no source yet) | 3 | 3 | 136 |
| `main/dolphin/os/OSReboot` | (no source yet) | 2 | 2 | 132 |
| `main/dolphin/mtx/mtx44vec` | (no source yet) | 1 | 1 | 120 |
| `main/dolphin/dsp/dsp_debug` | (no source yet) | 1 | 1 | 80 |
| `main/dolphin/os/OSArena` | (no source yet) | 5 | 5 | 76 |
| `main/gssdk_lib/gsapi/mathusage` | (no source yet) | 1 | 1 | 60 |
| `main/gssdk_lib/common/osspi/osspi` | (no source yet) | 2 | 2 | 56 |
| `main/libhu/subvf` | (no source yet) | 1 | 1 | 52 |
| `main/dolphin/dvd/dvdFatal` | (no source yet) | 1 | 1 | 48 |
| `main/gssdk_lib/common/csspi/csspi` | (no source yet) | 2 | 2 | 16 |
| `main/libhu/setvf` | (no source yet) | 1 | 1 | 16 |
| `main/dolphin/demo/DEMOInit` | (no source yet) | 1 | 1 | 8 |
