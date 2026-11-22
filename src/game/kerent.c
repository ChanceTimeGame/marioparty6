// Jump table into the DOL: each entry is one branch.

void OSDumpStopwatch(void);
void PSVECDotProduct(void);
void PSVECDistance(void);
void PSVECSquareDistance(void);
void VIGetDTVStatus(void);
void OSGetProgressiveMode(void);
void OSSetProgressiveMode(void);
void PSVECSubtract(void);
void PSMTXTranspose(void);
void C_VECReflect(void);
void PSMTXInvXpose(void);
void PSMTXRotRad(void);
void PSMTXRotAxisRad(void);
void PSVECMag(void);
void PSVECScale(void);
void PSMTXMultVecSR(void);
void C_QUATRotAxisRad(void);
void C_QUATMtx(void);
void C_QUATAdd(void);
void C_QUATSlerp(void);
void PSQUATDotProduct(void);
void PSQUATAdd(void);
void PSMTXQuat(void);
void PSQUATMultiply(void);
void PSQUATInverse(void);
void PSQUATNormalize(void);
void PSQUATScale(void);
void PSQUATSubtract(void);
void OSTicksToCalendarTime(void);
void PSMTXTransApply(void);
void PSMTXScaleApply(void);
void OSGetLanguage(void);
void PSMTXMultVecArraySR(void);
void PSMTX44MultVec(void);
void SIGetTypeString(void);
void OSCheckActiveThreads(void);
void OSGetThreadPriority(void);
void OSInitSemaphore(void);
void OSIsThreadTerminated(void);
void OSSignalSemaphore(void);
void OSWaitSemaphore(void);
void log(void);
void exp(void);
void GXWaitDrawDone(void);
void GXProject(void);
void GXGetProjectionv(void);
void GXGetViewportv(void);
void GXPixModeSync(void);
void GXSetIndTexOrder(void);
void GXSetIndTexMtx(void);
void GXSetTevIndWarp(void);
void GXSetTexCoordScaleManually(void);
void GXSetTevIndTile(void);
void GXNtsc480Prog(void);
void GXNtsc480Int(void);
void GXNtsc240Int(void);
void GXSetVtxDescv(void);
void GXDrawSphere(void);
void GXDrawCylinder(void);
void omSetStatBit(void);
void GameMesKillCheck(void);
void WipeCheck(void);
void WipeTypeGet(void);
void omMakeGroupEx(void);
void omGetGroupMemberListEx(void);
void GameMesDispSet(void);
void GameMesKill(void);
void ClusterAdjustObject(void);
void printWin(void);
void print8(void);
void HuSysVWaitSet(void);
void GameMesGet(void);
void omObjManPause(void);
void GameMesInit(void);
void GameMesCreate(void);
void omDelObjEx(void);
void GameMesPosSet(void);
void atan2(void);
void atan2f(void);
void cos(void);
void GameMesStatGet(void);
void Center(void);
void CRotM(void);
void CenterM(void);
void CZoomM(void);
void omOutViewMulti(void);
void omOutView(void);
void omAddObjEx(void);
void omInitObjMan(void);
void rand8(void);
void frandmod(void);
void frandom(void);
void SetDefLight(void);
void HuSetVecF(void);
void HuSubVecF(void);
void frandf(void);
void GameMesSprKill(void);
void mtxRotCat(void);
void MgScoreBoxInit(void);
void MgScoreBoxCreate(void);
void MgScoreBoxCreateChar(void);
void MgScoreBoxCreateCharMulti(void);
void MgScoreBoxColorSet(void);
void MgScoreBoxTPLvlSet(void);
void MgScoreBoxPosSet(void);
void MgScoreBoxDispSet(void);
void GameMesStrWinCreate(void);
void Hu3DCameraLayerHookSet(void);
void Hu3DCameraLayerHookReset(void);
void Hu3DLayerHookSet(void);
void Hu3DLayerHookReset(void);
void Hu3DNoSyncSet(void);
void Hu3DModelCreate(void);
void Hu3DModelLink(void);
void Hu3DHookFuncCreate(void);
void Hu3DModelPosSet(void);
void Hu3DModelPosSetV(void);
void Hu3DModelRotSet(void);
void Hu3DModelRotSetV(void);
void Hu3DModelScaleSet(void);
void Hu3DModelScaleSetV(void);
void Hu3DModelPosGet(void);
void Hu3DModelRotGet(void);
void Hu3DModelScaleGet(void);
void Hu3DModelKill(void);
void Hu3DModelAllKill(void);
void Hu3DModelAttrSet(void);
void Hu3DModelAttrReset(void);
void Hu3DModelAttrGet(void);
void Hu3DModelMotionAttrGet(void);
void Hu3DBGColorSet(void);
void Hu3DModelCameraSet(void);
void Hu3DModelLayerSet(void);
void Hu3DModelTPLvlSet(void);
void Hu3DModelHookSet(void);
void Hu3DModelHookReset(void);
void Hu3DModelHookObjReset(void);
void Hu3DModelAmbSet(void);
void Hu3DModelShadowDispOn(void);
void Hu3DModelShadowDispOff(void);
void Hu3DModelProjectionSet(void);
void Hu3DModelProjectionReset(void);
void Hu3DModelObjPosGet(void);
void Hu3DModelObjMtxGet(void);
void Hu3DModelHiliteTypeSet(void);
void Hu3DModelReflectTypeSet(void);
void Hu3DModelHiliteMapSet(void);
void Hu3DReflectModelSet(void);
void Hu3DModelMatHookSet(void);
void Hu3DModelDebug(void);
void Hu3DModelMtxSet(void);
void Hu3DModelMtxGet(void);
void Hu3DModelShadowMapTPLvlSet(void);
void Hu3DModelShadowMapObjTPLvlSet(void);
void Hu3DModelDieCreate(void);
void Hu3DModelLinkDieCreate(void);
void HuPadInit(void);
void HuPadRead(void);
void HuPadRumbleSet(void);
void HuPadRumbleStop(void);
void HuPadRumbleAllStop(void);
void HuPadStatGet(void);
void HuPadRumbleGet(void);
void HuPadStkDirGet(void);
void HuPadSubStkDirGet(void);
void Hu3DCameraCreate(void);
void Hu3DCameraPerspectiveSet(void);
void Hu3DCameraViewportSet(void);
void Hu3DCameraScissorSet(void);
void Hu3DCameraPosSet(void);
void Hu3DCameraKill(void);
void Hu3DCameraPosGet(void);
void Hu3DCameraPosSetV(void);
void Hu3DCameraAllKill(void);
void Hu3DCameraSet(void);
void Hu3DCameraMotionOn(void);
void Hu3DCameraMotionStart(void);
void Hu3DCameraMotionOff(void);
void Hu3DModelCameraCreate(void);
void Hu3DModelCameraInfoSet(void);
void Hu3DCameraPerspectiveGet(void);
void Hu3DLighInit(void);
void Hu3DGLightCreate(void);
void Hu3DGLightCreateV(void);
void Hu3DGLightKill(void);
void Hu3DLightAllKill(void);
void Hu3DLightSet(void);
void Hu3DGLightSpotSet(void);
void Hu3DGLightInfinitytSet(void);
void Hu3DGLightPointSet(void);
void Hu3DGLightColorSet(void);
void Hu3DGLightPosSetV(void);
void Hu3DGLightPosSet(void);
void Hu3DGLightPosAimSetV(void);
void Hu3DGLightPosAimSet(void);
void Hu3DGLightStaticSet(void);
void Hu3DLLightParamGet(void);
void Hu3DGLightParamGet(void);
void Hu3DGLightPosAngleSet(void);
void Hu3DModelLightBitSet(void);
void Hu3DModelLightBitReset(void);
void Hu3DModelLightInfoSet(void);
void Hu3DModelLightIdGet(void);
void Hu3DReflectMapSet(void);
void Hu3DReflectNoSet(void);
void Hu3DFogSet(void);
void Hu3DFogClear(void);
void Hu3DModelObjPtrGet(void);
void Hu3DModelShadowSet(void);
void Hu3DModelShadowReset(void);
void Hu3DShadowCreate(void);
void Hu3DShadowPosSet(void);
void Hu3DShadowTPLvlSet(void);
void Hu3DShadowColSet(void);
void Hu3DShadowMultiCreate(void);
void Hu3DShadowMultiPosSet(void);
void Hu3DShadowMultiSizeSet(void);
void Hu3DShadowMultiColSet(void);
void Hu3DShadowMultiTPLvlSet(void);
void Hu3DModelShadowMapSet(void);
void Hu3DModelShadowMapObjSet(void);
void Hu3DModelShadowMapObjReset(void);
void Hu3DModelShadowMapReset(void);
void Hu3DShadowSizeSet(void);
void Hu3DMipMapSet(void);
void Hu3DModelClusterAttrSet(void);
void Hu3DModelClusterAttrReset(void);
void Hu3DLLightCreate(void);
void Hu3DLLightCreateV(void);
void Hu3DLLightSpotSet(void);
void Hu3DLLightInfinitytSet(void);
void Hu3DLLightPointSet(void);
void Hu3DLLightKill(void);
void Hu3DLLightColorSet(void);
void Hu3DLLightPosSetV(void);
void Hu3DLLightPosSet(void);
void Hu3DLLightPosAimSetV(void);
void Hu3DLLightPosAimSet(void);
void Hu3DLLightStaticSet(void);
void Hu3DLLightPosAngleSet(void);
void Hu3DShineSet(void);
void Hu3DObjHookSet(void);
void Hu3DAnimInit(void);
void Hu3DAnimCreate(void);
void Hu3DAnimAnimSet(void);
void Hu3DAnimLink(void);
void Hu3DAnimKill(void);
void Hu3DAnimAllKill(void);
void Hu3DAnimSet(void);
void Hu3DAnimExec(void);
void Hu3DAnimAttrSet(void);
void Hu3DAnimAttrReset(void);
void Hu3DAnimSpeedSet(void);
void Hu3DAnimBankSet(void);
void Hu3DAnmNoSet(void);
void Hu3DTexScrollCreate(void);
void Hu3DTexScrollPosSet(void);
void Hu3DTexScrollPosMoveSet(void);
void Hu3DTexScrollRotSet(void);
void Hu3DTexScrollRotMoveSet(void);
void Hu3DTexScrollKill(void);
void Hu3DTexScrollAllKill(void);
void Hu3DTexScrollPauseDisableSet(void);
void Hu3DParticleCreate(void);
void Hu3DParticleScaleSet(void);
void Hu3DParticleZRotSet(void);
void Hu3DParticleColSet(void);
void Hu3DParticleTPLvlSet(void);
void Hu3DParticleBlendModeSet(void);
void Hu3DParticleHookSet(void);
void Hu3DParticleCntSet(void);
void Hu3DParticleAttrSet(void);
void Hu3DParticleAttrReset(void);
void Hu3DParticleAnimModeSet(void);
void Hu3DParManInit(void);
void Hu3DParManCreate(void);
void Hu3DParManKill(void);
void Hu3DParManAllKill(void);
void Hu3DParManPtrGet(void);
void Hu3DParManAttrSet(void);
void Hu3DParManAttrReset(void);
void Hu3DParManPosSet(void);
void Hu3DParManVecSet(void);
void Hu3DParManRotSet(void);
void Hu3DParManModelIDGet(void);
void Hu3DParManLink(void);
void Hu3DParManTimeLimitSet(void);
void Hu3DParManColorSet(void);
void Hu3DParManLayerSet(void);
void Hu3DWaterCreate(void);
void Hu3DWaterTexPosSet(void);
void Hu3DWaterTexScaleSet(void);
void Hu3DWaterPadYSet(void);
void Hu3DWaterGlowSet(void);
void Hu3DWaterIndTexMtxSet(void);
void Hu3DWaterHiliteSet(void);
void Hu3DWaterHiliteTPLvlSet(void);
void Hu3DWaterCameraSet(void);
void Hu3DWaterWaveCreate(void);
void Hu3DMotionCreate(void);
void Hu3DMotionInit(void);
void Hu3DMotionModelCreate(void);
void Hu3DMotionKill(void);
void Hu3DMotionAllKill(void);
void Hu3DMotionSet(void);
void Hu3DMotionExec(void);
void Hu3DSubMotionExec(void);
void Hu3DMotionNext(void);
void Hu3DMotionTimeSet(void);
void Hu3DMotionTimeGet(void);
void Hu3DMotionMaxTimeGet(void);
void Hu3DMotionStartEndSet(void);
void Hu3DMotionMotionMaxTimeGet(void);
void Hu3DMotionEndCheck(void);
void Hu3DMotionShiftSet(void);
void Hu3DMotionIDGet(void);
void Hu3DMotionShiftIDGet(void);
void Hu3DMotionShiftTimeGet(void);
void Hu3DMotionShiftMaxTimeGet(void);
void Hu3DMotionShiftStartEndSet(void);
void Hu3DMotionShiftSpeedSet(void);
void Hu3DMotionSpeedSet(void);
void Hu3DMotionShapeSpeedSet(void);
void Hu3DMotionShapeTimeSet(void);
void Hu3DMotionShapeTimeGet(void);
void Hu3DMotionNoMotSet(void);
void Hu3DMotionNoMotReset(void);
void Hu3DMotionForceSet(void);
void Hu3DMotionShapeSet(void);
void Hu3DMotionShapeIDGet(void);
void Hu3DMotionShapeMaxTimeGet(void);
void Hu3DMotionClusterSet(void);
void Hu3DMotionClusterNoSet(void);
void Hu3DMotionShapeReset(void);
void Hu3DMotionClusterReset(void);
void Hu3DJointMotion(void);
void Hu3DMotionOverlaySet(void);
void Hu3DMotionOverlayReset(void);
void Hu3DMotionOverlayTimeGet(void);
void Hu3DMotionOverlayTimeSet(void);
void Hu3DMotionOverlaySpeedSet(void);
void Hu3DMotionOverlayMaxTimeGet(void);
void Hu3DMotionOverlayEndCheck(void);
void Hu3DModelObjDrawInit(void);
void Hu3DModelObjDraw(void);
void Hu3DModelObjPtrDraw(void);
void Hu3DMotionCalc(void);
void Hu3DMotionShapeStartEndSet(void);
void Hu3DSubMotionSet(void);
void Hu3DSubMotionReset(void);
void Hu3DSubMotionTimeSet(void);
void Hu3DMotionShiftTimeSet(void);
void Hu3DMotionClusterSpeedSet(void);
void Hu3DMotionClusterTimeSet(void);
void Hu3DMotionClusterMaxTimeGet(void);
void Hu3DMotionAttrSet(void);
void Hu3DMotionAttrReset(void);
void Hu3DMotionTimingHookSet(void);
void Hu3DMotionTimingHookReset(void);
void Hu3DMatLightSet(void);
void Hu3DFbCopyLayerSet(void);
void Hu3DFbCopyExec(void);
void Hu3DZClear(void);
void Hu3DZClearLayerSet(void);
void CamMotionEx(void);
void CamMotionEx2(void);
void CamMotionExPathGet(void);
void CamMotionExPath(void);
void Hu3D2Dto3D(void);
void Hu3D3Dto2D(void);
void Hu3DMtxRotGet(void);
void Hu3DMtxTransGet(void);
void Hu3DMtxScaleGet(void);
void Hu3DProjectionCreate(void);
void Hu3DProjectionPosSet(void);
void Hu3DProjectionTPLvlSet(void);
void Hu3DProjectionKill(void);
void HuAudInit(void);
void HuAudStreamPlay(void);
void HuAudAllStop(void);
void HuAudFadeOut(void);
void HuAudStreamPauseOn(void);
void HuAudStreamPauseOff(void);
void HuAudStreamFadeOut(void);
void HuAudStreamVolSet(void);
void HuAudFXPlay(void);
void HuAudFXPlayVol(void);
void HuAudFXPlayVolPan(void);
void HuAudFXPlayPan(void);
void HuAudFXPlayDelay(void);
void HuAudFXPlayDelayVol(void);
void HuAudFXPlayDelayPan(void);
void HuAudFXPlayDelayVolPan(void);
void HuAudFXStop(void);
void HuAudFXFadeOut(void);
void HuAudFXPanning(void);
void HuAudSeqPlay(void);
void HuAudSeqStop(void);
void HuAudSeqFadeOut(void);
void HuAudSeqMidiCtrlGet(void);
void HuAudFXListnerSet(void);
void HuAudFXListnerUpdate(void);
void HuAudFXEmiterPlay(void);
void HuAudFXEmiterUpDate(void);
void HuAudFXListnerKill(void);
void HuAudFXStatusGet(void);
void HuAudFXPitchSet(void);
void HuAudFXVolSet(void);
void HuAudDllSndGrpSet(void);
void HuAudSndGrpSetSet(void);
void HuAudSndGrpSet(void);
void HuAudSndCommonGrpSet(void);
void HuAudSStreamPlay(void);
void HuAudSStreamStop(void);
void HuAudSStreamFadeOut(void);
void HuAudSStreamAllFadeOut(void);
void HuAudSStreamAllStop(void);
void HuAudSStreamStatGet(void);
void HuAudSStreamPause(void);
void HuAudSStreamPauseFadeOut(void);
void HuAudSStreamParamSet(void);
void PlayerFXPlay(void);
void CharFXPlay(void);
void CharFXPlayVol(void);
void CharFXPlayPan(void);
void CharFXPlayVolPan(void);
void CharFXPlayDelayVolPan(void);
void CharFXPlayDelay(void);
void CharFXPlayDelayVol(void);
void CharFXPlayDelayPan(void);
void PlayerFXPlayPos(void);
void CharFXPlayPos(void);
void PlayerFXStop(void);
void CharFXStop(void);
void HuAudAUXSet(void);
void HuAudSStreamChanPlay(void);
void HuAudBGMPlay(void);
void HuAudJinglePlay(void);
void msmMusPlay(void);
void espInit(void);
void espEntry(void);
void espKill(void);
void espGrpIDGet(void);
void espDispOn(void);
void espDispOff(void);
void espAttrSet(void);
void espAttrReset(void);
void espPosSet(void);
void espScaleSet(void);
void espZRotSet(void);
void espTPLvlSet(void);
void espColorSet(void);
void espSpeedSet(void);
void espBankSet(void);
void espDrawNoSet(void);
void espPriSet(void);
void HuPrcInit(void);
void HuPrcCall(void);
void HuPrcCreate(void);
void HuPrcKill(void);
void HuPrcEnd(void);
void HuPrcWakeup(void);
void HuPrcSleep(void);
void HuPrcVSleep(void);
void HuPrcCurrentGet(void);
void HuPrcDestructorSet(void);
void HuPrcDestructorSet2(void);
void HuPrcChildCreate(void);
void HuPrcChildLink(void);
void HuPrcChildUnlink(void);
void HuPrcChildKill(void);
void HuPrcMemAlloc(void);
void HuPrcMemFree(void);
void Hu3DParManVacumeSet(void);
void HuDataInit(void);
void HuDataDirRead(void);
void HuDataRead(void);
void HuDataReadNum(void);
void HuDataReadMulti(void);
void HuDataDirReadAsync(void);
void HuDataDirReadNumAsync(void);
void HuDataSelHeapRead(void);
void HuDataSelHeapReadNum(void);
void HuDataGetSize(void);
void HuDataDirClose(void);
void HuDataDirCloseNum(void);
void HuDataDirCloseAll(void);
void HuDataClose(void);
void HuDataCloseMulti(void);
void HuDataReadNumHeapShortForce(void);
void HuDataDirCancel(void);
void ARRegisterDMACallback(void);
void ARGetDMAStatus(void);
void ARStartDMA(void);
void ARInit(void);
void ARAlloc(void);
void ARFree(void);
void ARGetSize(void);
void ARSetSize(void);
void HuSprBegin(void);
void HuSprCall(void);
void HuSprFinish(void);
void HuSprAnimRead(void);
void HuSprCreate(void);
void HuSprGrpCreate(void);
void HuSprGrpMemberSet(void);
void HuSprGrpMemberKill(void);
void HuSprGrpKill(void);
void HuSprKill(void);
void HuSprAnimKill(void);
void HuSprAttrSet(void);
void HuSprAttrReset(void);
void HuSprAttrGet(void);
void HuSprPosSet(void);
void HuSprScaleSet(void);
void HuSprZRotSet(void);
void HuSprGrpPosSet(void);
void HuSprGrpZRotSet(void);
void HuSprGrpScaleSet(void);
void HuSprTPLvlSet(void);
void HuSprColorSet(void);
void HuSprSpeedSet(void);
void HuSprBankSet(void);
void HuSprGrpDrawNoSet(void);
void HuSprDrawNoSet(void);
void HuSprPriSet(void);
void HuSprFuncCreate(void);
void HuSprAnimMake(void);
void HuSprGrpCenterSet(void);
void HuSprAnimLock(void);
void AnimDebug(void);
void HuSprExecLayerSet(void);
void HuSprSprBGSet(void);
void HuSprBGSet(void);
void HuSprGrpScissorSet(void);
void HuSprScissorSet(void);
void HuSprGrpTPLvlSet(void);
void HuSprExecLayerCameraSet(void);
void HuSprAnimNoSet(void);
void HuSprAnimNoSetPause(void);
void HuSpr3DHookSet(void);
void HuSpr3DDataCreate(void);
void HuSpr3DSet(void);
void HuSpr3DRotSet(void);
void HuSpr3DFovSet(void);
void HuSprVtxColorSet(void);
void HuSprVtxColorReset(void);
void HuMemMemoryNumSet(void);
void HuARMalloc(void);
void HuARFree(void);
void HuAR_DVDtoARAM(void);
void HuAR_MRAMtoARAM(void);
void HuAR_MRAMtoARAM2(void);
void HuAR_ARAMtoMRAM(void);
void HuAR_ARAMtoMRAMNum(void);
void HuARDMACheck(void);
void HuARDirCheck(void);
void HuARDirFree(void);
void HuAR_ARAMtoMRAMFileRead(void);
void HuAudFXListnerSetEX(void);
void HuWindowInit(void);
void HuWinInit(void);
void HuWinCreate(void);
void HuWinKill(void);
void HuWinAllKill(void);
void HuWinMesSet(void);
void HuWinInsertMesSet(void);
void HuWinChoiceGet(void);
void HuWinChoiceSet(void);
void HuWinChoiceNumGet(void);
void HuWinMesWait(void);
void HuWinSprKill(void);
void HuWinMesRead(void);
void HuWinAnimSet(void);
void HuWinSprSet(void);
void HuWinSprPosSet(void);
void HuWinSprIDGet(void);
void HuWinMesMaxSizeGet(void);
void HuWinInsertMesSizeGet(void);
void HuWinMesMaxSizeBetGet(void);
void HuWinExCreate(void);
void HuWinDispOn(void);
void HuWinDispOff(void);
void HuWinExOpen(void);
void HuWinExClose(void);
void HuWinExKill(void);
void HuWinExSpeakerSet(void);
void HuWinComKeyWait(void);
void HuWinComKeySet(void);
void HuWinComKeyReset(void);
void HuWinDrawNoSet(void);
void HuWinAttrSet(void);
void HuWinAttrReset(void);
void HuWinBGTPLvlSet(void);
void HuWinMesSpeedSet(void);
void HuWinBGColSet(void);
void HuWinChoiceDisable(void);
void HuWinKeyWaitEntry(void);
void HuWinCenterPosSet(void);
void HuWinPosSet(void);
void HuWinZRotSet(void);
void HuWinScaleSet(void);
void HuWinChoiceNowGet(void);
void HuWinStatGet(void);
void HuWinMesColSet(void);
void HuWinScissorSet(void);
void HuWinExCreateFrame(void);
void HuWinPriSet(void);
void HuWinSprPriSet(void);
void HuWinKeyWaitNumGet(void);
void HuWinMesSizeCancelCRSet(void);
void HuWinMesPalSet(void);
void HuWinHomeClear(void);
void HuWinPushKeySet(void);
void HuWinDisablePlayerSet(void);
void HuWinDisablePlayerReset(void);
void HuWinMesPtrGet(void);
void HuWinMesMaxDirGet(void);
void HuWinMesMaxNumGet(void);
void HuWinMesLanguageSet(void);
void HuWinWarningCreate(void);
void HuWinWarningKill(void);
void HuWinWarningOpen(void);
void HuWinWarningClose(void);
void HuWinFrameSet(void);
void HuWinCallbackStub(void);
void HuWinCallbackSet(void);
void HuWinPadMaskSet(void);
void CharInit(void);
void CharMotionInit(void);
void CharMotionInitAsync(void);
void CharDataClose(void);
void CharModelKill(void);
void CharClose(void);
void CharModelCreate(void);
void CharMotionCreate(void);
void CharMotionNoSet(void);
void CharMotionKill(void);
void CharMotionDataClose(void);
void CharModelDataClose(void);
void CharMotionAMemPGet(void);
void CharMotionSet(void);
void CharMotionTimeSet(void);
void CharMotionTimeGet(void);
void CharMotionMaxTimeGet(void);
void CharMotionEndCheck(void);
void CharMotionShiftIDGet(void);
void CharMotionShiftSet(void);
void CharMotionShiftTimeGet(void);
void CharMotionSpeedSet(void);
void CharModelHookDustCreate(void);
void CharEffectSmokeCreate(void);
void CharEffectSmokeCreateScale(void);
void CharEffectCoinGlowCreate(void);
void CharModelHitCreate(void);
void CharEffectHitCreate(void);
void CharEffectShoeHitCreate(void);
void CharModelShoeHitCreate(void);
void GWCharColorGet(void);
void CharEffectLayerSet(void);
void CharEyeBmpNameTbl(void);
void CharModelEyeBmpGet(void);
void CharMotionVoiceOnSet(void);
void CharNpcDustVoiceOffSet(void);
void CharModelStepSet(void);
void CharModelVoicePanAutoSet(void);
void CharModelVoiceFlagSet(void);
void CharModelLandDustCreate(void);
void CharModelAttrSet(void);
void CharModelAttrReset(void);
void CharMotionStartEndSet(void);
void CharModelHeightGet(void);
void CharModelMotListCreate(void);
void CharEffectDustCreate(void);
void CharEffectCryCreate(void);
void CharModelCryCreate(void);
void CharModelTimingHookNoGet(void);
void CharMotionUpdateSet(void);
void CharModelVoicePanSet(void);
void CharModelVoiceVolSet(void);
void CharWinLoseVoicePlay(void);
void CharLoseVoicePlay(void);
void CharMotionTotalTimeGet(void);
void CharMotionExtraTimeGet(void);
void CharLightCreate(void);
void CharLightCreateV(void);
void CharLightSpotSet(void);
void CharLightInfinitytSet(void);
void CharLightPointSet(void);
void CharLightColorSet(void);
void CharLightPosAimSetV(void);
void CharLightPosAimSet(void);
void CharLightStaticSet(void);
void CharEffectWarnCreate(void);
void CharMotionDefrag(void);
void CharNpcDustSet(void);
void HuPrcSetStat(void);
void HuPrcResetStat(void);
void omGameSysInit(void);
void omResetStatBit(void);
void omCurrentOvlGet(void);
void omPauseChk(void);
void omOvlHisChg(void);
void omOvlHisGet(void);
void omovlevtno(void);
void omovlstat(void);
void AddX(void);
void AddZ(void);
void MapObject(void);
void CharObject(void);
void nMap(void);
void nChar(void);
void MapWall(void);
void MapPos(void);
void HitFaceVec(void);
void HitFace(void);
void HitFaceCount(void);
void GwPlayer(void);
void GwSystem(void);
void GWSinglePrizeFlagSet(void);
void GWSinglePrizeFlagGet(void);
void GWSingleDataInit(void);
void GWSinglePrizeSaveFlagSet(void);
void GWSinglePrizeSaveFlagGet(void);
void GWBankStarAdd(void);
void GWBankStarSub(void);
void GWBankStarGet(void);
void GWBankStarAwardGet(void);
void GWBankStarAwardReset(void);
void GWBankFlagSet(void);
void GWBankFlagReset(void);
void GWBankFlagGet(void);
void GWSingleMgFlagGet(void);
void GWSingleMgWinNumGet(void);
void GWSingleMgWinNumSet(void);
void GWSingleMgRecordNumGet(void);
void GWSingleMgRecordNumSet(void);
void HuTHPSprCreateVol(void);
void HuTHPSprCreate(void);
void HuTHP3DCreateVol(void);
void HuTHP3DCreate(void);
void HuTHPStop(void);
void HuTHPClose(void);
void HuTHPRestart(void);
void HuTHPEndCheck(void);
void HuTHPFrameGet(void);
void HuTHPTotalFrameGet(void);
void HuTHPSetVolume(void);
void THPSimpleGetFrameRate(void);
void HuTHPMatHook(void);
void HuTHPStartCheck(void);
void HuCardCheck(void);
void HuCardMount(void);
void HuCardFormat(void);
void HuCardOpen(void);
void HuCardRead(void);
void HuCardCreate(void);
void HuCardClose(void);
void HuCardWrite(void);
void HuCardDelete(void);
void HuCardSectorSizeGet(void);
void HuCardFreeSpaceGet(void);
void HuCardSlotCheck(void);
void HuCardWriteAsync(void);
void HuCardWriteCheck(void);
void HuCardWriteIdle(void);
void SLFileOpen(void);
void SLFileCreate(void);
void SLFileWrite(void);
void SLFileClose(void);
void SLFileRead(void);
void SLSaveEmptySet(void);
void SLCurSlotNoSet(void);
void SLCurSlotNoGet(void);
void SLSaveDataMake(void);
void SLSaveDataSlotMake(void);
void SLCommonSet(void);
void SLBoardSave(void);
void SLSave(void);
void SLCommonLoad(void);
void SLBoardLoad(void);
void SLLoad(void);
void SLCurBoxNoSet(void);
void SLCurBoxNoGet(void);
void SLSaveFlagSet(void);
void SLSaveFlagGet(void);
void SLCheckSumCheck(void);
void SLCheckSumBoxSlotCheck(void);
void SLCheckSumGet(void);
void SLCheckSumSlotGet(void);
void SLSerialNoGet(void);
void SLSerialNoCheck(void);
void saveExecF(void);
void SLWinIdSet(void);
void SLBoxBackupLoad(void);
void SLBoxBackupSlotLoad(void);
void SLCheckSumBoxSet(void);
void SLBoxDataOffsetGet(void);
void SLSaveBoardTurnExec(void);
void SLSaveBoardEndExec(void);
void SLSaveModeExec(void);
void curSlotNo(void);
void UnMountCnt(void);
void SLCommonSaveCopy(void);
void SLCommonLoadCopy(void);
void _CheckFlag(void);
void _SetFlag(void);
void _ClearFlag(void);
void MgDataTbl(void);
void MgModeSubMode(void);
void MgModeScore(void);
void MgModePlayNum(void);
void MgModeWork(void);
void GwCommonBackup(void);
void lbl_802C0438(void);
void MgExitReq(void);
void MgPauseExitF(void);
void MgBoardE3F(void);
void MgNoGet(void);
void MBookEvtNo(void);
void MgSubMode(void);
void MgSubModeGet(void);
void MgBattleOrder(void);
void MgDecaScoreCalc(void);
void MgInstExitF(void);
void GWRecordSet(void);
void GWRecordGet(void);
void GWBoardPlayNumAdd(void);
void GWBoardPlayNumSet(void);
void GWBoardPlayNumGet(void);
void GWCharPlayNumInc(void);
void GWCharPlayNumGet(void);
void GWCharPlayNumSet(void);
void GWBoardMaxStarSet(void);
void GWBoardMaxStarGet(void);
void GWBoardMaxCoinSet(void);
void GWBoardMaxCoinGet(void);
void GWPlayerStarAdd(void);
void GWPlayerStarGet(void);
void GWPlayerStarSet(void);
void GWPlayerCoinAdd(void);
void GWPlayerCoinSet(void);
void GWPlayerCoinGet(void);
void GWMgCustomReset(void);
void GWMgCustomSet(void);
void GWMgCustomGet(void);
void GWMgUnlockGet(void);
void GWMgPlayNumSet(void);
void GWMgPlayNumGet(void);
void GWMgPlayNumAdd(void);
void GWMiracleBookFlagSet(void);
void GWMiracleBookFlagGet(void);
void GWMikeActRecordSet(void);
void GWMikeActRecordGet(void);
void GWSaveDebugSet(void);
void MgModeWorkInt(void);
void MgModeWorkFloat(void);
void GwMgDecaScore(void);
void GwLanguage(void);
void fmod(void);
void _savegpr_15(void);
void _restgpr_15(void);
void _savegpr_16(void);
void _restgpr_16(void);
void _savegpr_14(void);
void _restgpr_14(void);
void omDBGMenuButton(void);
void omDBGSystemKeyCheckSetup(void);
void msmSeSetListener(void);
void msmSeUpdataListener(void);
void msmSeGetIndexPtr(void);
void msmMusSetMasterVolume(void);
void msmSysGetOutputMode(void);
void msmSysSetGroupLoadMode(void);
void WipeCreate(void);
void WipeColorSet(void);
void omOvlReturnEx(void);
void HuDataGetAsyncStat(void);
void omVibrate(void);
void GameMesOvlPrev(void);
void MgTimerCreate(void);
void MgTimerKill(void);
void MgTimerModeGet(void);
void MgTimerModeOnSet(void);
void MgTimerModeOffSet(void);
void MgTimerParamSet(void);
void MgTimerValueGet(void);
void MgTimerDoneCheck(void);
void MgTimerPosSet(void);
void MgTimerPosGet(void);
void MgTimerBackColorSet(void);
void MgTimerColorSet(void);
void MgTimerRecordSet(void);
void MgTimerRecordDispOn(void);
void MgTimerRecordDispOff(void);
void MgScoreInit(void);
void MgScoreKill(void);
void MgScoreModeGet(void);
void MgScoreValueSet(void);
void MgScoreValueGet(void);
void MgScoreMaxDigitSet(void);
void MgScoreMaxDigitGet(void);
void MgScorePosSet(void);
void MgScorePosGet(void);
void MgScoreScaleSet(void);
void MgScoreScaleGet(void);
void MgScoreZRotSet(void);
void MgScoreZRotGet(void);
void MgScoreDigitWidthSet(void);
void MgScoreDigitWidthGet(void);
void MgScoreDispOn(void);
void MgScoreDispOff(void);
void MgScoreTPLvlSet(void);
void MgScoreTPLvlGet(void);
void MgScoreModeDefaultSet(void);
void MgScoreCreate(void);
void MgScoreColorSet(void);
void MgScoreDigitScaleSet(void);
void MgScorePriSet(void);
void MgScoreUnitBankSet(void);
void MgSeqCreate(void);
void MgSeqCreatePrio(void);
void MgSeqKill(void);
void MgSeqModeGet(void);
void MgSeqModeNext(void);
void MgSeqModeSet(void);
void MgSeqRecordSet(void);
void MgSeqModeChangeOff(void);
void MgSeqModeChangeOn(void);
void MgSeqStatGet(void);
void MgSeqWinnerSet(void);
void MgSeqDrawSet(void);
void fn_80071CCC(void);
void MgSeqModeDelaySet(void);
void MgSeqModeHookAdd(void);
void MgSeqModeHookReset(void);
void MgSeqTimerValueGet(void);
void MgSeqFrameNoGet(void);
void MgSeqFrameNoSet(void);
void MgSeqGameMesIdGet(void);
void MgSeqTimerKill(void);
void MgSeqStatBitSet(void);
void MgSeqStatBitReset(void);
void omCameraViewInit(void);
void omCameraViewSetMulti(void);
void omCameraViewSet(void);
void omCameraViewMoveSimpleMulti(void);
void omCameraViewMoveSimple(void);
void omCameraViewMoveMulti(void);
void omCameraViewMove(void);
void omCameraViewCheck(void);
void MgActorObjectSetup(void);
void MgActorColMapInit(void);
void MgPlayerPadSet(void);
void MgPlayerModeFuncSet(void);
void MgPlayerCreate(void);
void MgPlayerCreateJumpAlt(void);
void MgActorCreate(void);
void MgActorKill(void);
void MgActorExec(void);
void MgPlayerModeLandSet(void);
void MgPlayerModeSet(void);
void MgPlayerComStkOn(void);
void MgPlayerComStkOff(void);
void MgPlayerVecChase(void);
void MgActorVecChase(void);
void MgActorColAttrSet(void);
void MgActorColAttrReset(void);
void MgActorColAttrGet(void);
void MgActorColMapPolyGet(void);
void MgActorPosSet(void);
void MgActorPosSetRaw(void);
void MgActorPosGet(void);
void MgPlayerDespawn(void);
void MgPlayerSpawn(void);
void MgPlayerColDisable(void);
void MgPlayerColEnable(void);
void MgPlayerVibrateCreate(void);
void MgPlayerVibrateKill(void);
void MgActorColAttrParamSet(void);
void MgActorColAttrParamGet(void);
void MgActorColMapMaskSet(void);
void MgActorColMapMaskGet(void);
void MgActorColMaskSet(void);
void MgActorColMaskGet(void);
void MgPlayerAttrCheck(void);
void MgPlayerAttrSet(void);
void MgPlayerAttrReset(void);
void MgPlayerModeAttrCheck(void);
void MgPlayerModeAttrReset(void);
void MgPlayerModeAttrSet(void);
void MgPlayerVibAttrCheck(void);
void MgPlayerVibAttrSet(void);
void MgPlayerStunSet(void);
void MgActorColCylReset(void);
void MgActorColCylSet(void);
void MgActorColCodeGet(void);
void MgActorColNormalGet(void);
void MgActorRotYSet(void);
void MgActorRotYGet(void);
void MgActorPushSet(void);
void MgPlayerPosSet(void);
void MgActorGravitySet(void);
void MgActorVelYSet(void);
void MgActorColMeshGet(void);
void MgActorVelSet(void);
void MgActorColBounceSet(void);
void CharModelFileNumGet(void);
void CharMotionFileNumGet(void);
void lbl_802BF860(void);
void lbl_802BF864(void);
void lbl_802BF868(void);
void _savefpr_22(void);
void _restfpr_22(void);
void _savefpr_24(void);
void _restfpr_24(void);
void _savefpr_27(void);
void _restfpr_27(void);
void _savefpr_28(void);
void _restfpr_28(void);
void __construct_array(void);
void __destroy_arr(void);
void __msl_assertion_failed(void);
void __nw__FUl(void);
void __nwa__FUl(void);
void __ptmf_test(void);
void __ptmf_scall(void);
void __dla__FPv(void);
void lbl_802166D8(void);
void __ptmf_cmpr(void);
void HuDataDirCancelAsync(void);
void HuDataDirCloseAsync(void);
void HuTHPProcCheck(void);
void mbSaveInit(void);
void mbSavePartyInit(void);
void mbSaveStoryInit(void);
void fn_80146BA0(void);
void fn_80146C88(void);
void HuLoadProcStart(void);
void GwMgTime(void);
void HuMCSysInit(void);
void HuMCInit(void);
void HuMCClose(void);
void HuMCDelayContextStart(void);
void HuMCDelayResponseResultGet(void);
void HuMCDelayResultGet(void);
void HuMCContextStart(void);
void HuMCResponseResultGet(void);
void HuMCContextCreate(void);
void HuMCContextKill(void);
void HuMCThresholdSet(void);
void HuMCCurResponseGet(void);
void HuMCButtonGet(void);
void HuMCButtonDownGet(void);
void HuMCProbe(void);
void HuMCMount(void);
void HuMCMicSprCreate(void);
void HuMCMicSprKill(void);
void HuMCMicGet(void);
void HuMCMicSet(void);
void HuMCSelWinCreate(void);
void HuMCSelWinKill(void);
void HuMCSelWinItemSet(void);
void HuMCSelWinItemRandSet(void);
void HuMCSelWinCheck(void);
void HuMCSelModeGet(void);
void HuMCListenerMQCreate(void);
void HuMCListenerMQWait(void);
void HuMCListenerMQKill(void);
void HuMCListenerMQMaxTimeSet(void);
void HuMCListenerMQMaxTimeGet(void);
void HuMCListenerCreate(void);
void HuMCListenerKill(void);
void HuMCSessionSet(void);
void HuMCSessionKill(void);
void HuMCSessionClose(void);
void HuMCRetypeResponseCheck(void);
void HuMCRetypeResponse(void);
void HuMCLevelInit(void);
void HuMCLevelGet(void);
void HuMCShiftsSet(void);
void fn_8014C818(void);
void fn_8014C81C(void);
void mbBranchInit(void);
void mbev_Branch(void);
void mbev_BranchDebug(void);
void mbBranchAttrSet(void);
void mbBranchAttrReset(void);
void mbBranchAttrGet(void);
void mbBranchMAttrSet(void);
void mbBranchMAttrReset(void);
void mbBranchMAttrGet(void);
void mbBranchAttrCheck(void);
void mbBranchComStarHookSet(void);
void mbCameraInit(void);
void mbCameraZoomSet(void);
void mbCameraZoomGet(void);
void mbCameraRotSetV(void);
void mbCameraRotSet(void);
void mbCameraRotGet(void);
void mbCameraCenterSetV(void);
void mbCameraCenterSet(void);
void mbCameraCenterGet(void);
void mbCameraEyeSetV(void);
void mbCameraEyeSet(void);
void mbCameraEyeGet(void);
void mbCameraOffsetSetV(void);
void mbCameraOffsetSet(void);
void mbCameraOffsetGet(void);
void mbCameraDirGet(void);
void mbCameraPosDirGet(void);
void mbCameraLookAtFuncSet(void);
void mbCameraBitSet(void);
void mbCameraFovSet(void);
void mbCameraScissorSet(void);
void mbCameraNearFarSet(void);
void mbCameraNearFarGet(void);
void mbCameraGet(void);
void mbCameraStackPush(void);
void mbCameraStackIdxSet(void);
void mbCameraStackPop(void);
void mbCameraLookAtGet(void);
void mbCameraLookAtInvGet(void);
void mbCameraCurveTypeSet(void);
void mbCameraCurveTypeGet(void);
void mbCameraCullCheck(void);
void mbCameraPlayerViewSet(void);
void mbCameraPlayerViewSetFast(void);
void mbCameraPlayerViewGet(void);
void mbCameraPlayerViewNoGet(void);
void mbCameraPlayerViewZoomGet(void);
void mbCameraPlayerViewRotXGet(void);
void mbCameraFocusReset(void);
void mbCameraFocusPosSet(void);
void mbCameraFocusPosAdd(void);
void mbCameraFocusPlayerSet(void);
void mbCameraFocusPlayerAdd(void);
void mbCameraFocusObjSet(void);
void mbCameraFocusObjAdd(void);
void mbCameraFocusMasuSet(void);
void mbCameraFocusMasuAdd(void);
void mbCameraMoveOnSet(void);
void mbCameraSpeedSet(void);
void mbCameraMovePos(void);
void mbCameraMovePlayer(void);
void mbCameraMoveObj(void);
void mbCameraMoveMasu(void);
void mbCameraShakeSet(void);
void mbCameraShakeReset(void);
void mbCameraMoveStop(void);
void mbCameraMoveCheck(void);
void mbCameraMoveWait(void);
void mbCapSelect(void);
void mbCapDelete(void);
void fn_8019A618(void);
void fn_8019A61C(void);
void fn_8019A620(void);
void fn_8019A624(void);
void fn_8019A628(void);
void fn_8019A62C(void);
void mbCapSelectGrow(void);
void mbCapSelectShrinkCheck(void);
void mbCapSelectResultSet(void);
void mbCapSelectResultGet(void);
void mbCapSelectResultReset(void);
void mbCapMasuExec(void);
void mbCapMasuObjInit(void);
void mbCapMasuObjClose(void);
void mbCapMasuObjCreateAll(void);
void mbCapMasuObjCreate(void);
void mbCapMasuDispSet(void);
void mbCapCapsuleGet(void);
void mbev_CapKinoko(void);
void mbev_CapKinokoKill(void);
void mbev_CapSKinoko(void);
void mbev_CapSKinokoKill(void);
void mbev_CapPKinoko(void);
void mbev_CapPKinokoKill(void);
void mbev_CapMKinoko(void);
void mbev_CapMKinokoKill(void);
void mbev_CapKiller(void);
void mbev_CapKillerKill(void);
void mbev_CapDokan(void);
void mbev_CapDokanKill(void);
void mbev_CapHanachan(void);
void mbev_CapHanachanKill(void);
void mbev_CapNKinoko(void);
void mbev_CapNKinokoKill(void);
void mbev_CapKillerMove(void);
void mbev_CapKillerMoveKill(void);
void mbev_CapTogezo(void);
void mbev_CapTogezoKill(void);
void mbev_CapKuribo(void);
void mbev_CapKuriboKill(void);
void mbev_CapPakkun(void);
void mbev_CapPakkunKill(void);
void mbev_CapJango(void);
void mbev_CapJangoKill(void);
void mbev_CapPatapata(void);
void mbev_CapPatapataKill(void);
void mbev_CapKokamekku(void);
void mbev_CapKokamekkuKill(void);
void mbev_CapKamekku(void);
void mbev_CapKamekkuKill(void);
void mbev_CapThrowman(void);
void mbev_CapThrowmanKill(void);
void mbev_CapBoble(void);
void mbev_CapBobleKill(void);
void mbev_CapBobleTrap(void);
void mbev_CapBobleMove(void);
void mbev_CapBiriQ(void);
void mbev_CapBiriQKill(void);
void mbev_CapBiriQTrap(void);
void mbev_CapBiriQMetalShock(void);
void mbev_CapTumujikun(void);
void mbev_CapTumujikunKill(void);
void mbev_CapTumujikunTrap(void);
void mbev_CapDossun(void);
void mbev_CapDossunKill(void);
void mbev_CapDossunTrap(void);
void mbev_CapBomhei(void);
void mbev_CapBomheiKill(void);
void mbev_CapBomheiTrap(void);
void mbev_CapBomheiMove(void);
void mbev_CapTeresa(void);
void mbev_CapTeresaKill(void);
void mbev_CapTeresaFadeCreate(void);
void mbev_CapTeresaFadeKill(void);
void mbev_CapTeresaFadeSet(void);
void mbev_CapMiracle(void);
void mbev_CapMiracleKill(void);
void mbev_CapKettou(void);
void mbev_CapKettouKill(void);
void mbev_CapDonkey(void);
void mbev_CapDonkeyKill(void);
void mbev_CapKoopa(void);
void mbev_CapKoopaKill(void);
void mbev_CapCall(void);
void mbev_CapCallTrap(void);
void mbev_CapCapGet(void);
void mbev_CapWait(void);
void mbev_CapCallKettou(void);
void mbev_CapCircuitCallKettou(void);
void mbev_CapCallDonkey(void);
void mbev_CapCallKoopa(void);
void MBCapsuleStub5(void);
void MBCapsuleStub6(void);
void MBCapsuleStub7(void);
void mbev_CapCallTeresa(void);
void mbev_CapCallMiracle(void);
void mbev_CapBiriQShockCreate(void);
void mbev_CapBiriQShockDelayGet(void);
void mbev_CapBiriQMetalShockCreate(void);
void mbev_CapRandomBonusCoin(void);
void mbev_CapBonusCoinCall(void);
void mbev_CapBonusCoin(void);
void mbev_CapBonusCoinCheck(void);
void mbev_CapMoveMasuSet(void);
void mbev_CapStopMasuSet(void);
void mbev_CapKettouEndCall(void);
void mbev_CapDonkeyEndCall(void);
void mbev_CapKoopaEndCall(void);
void mbev_CapVsEndCall(void);
void mbev_CapKillerMoveCall(void);
void MBCapsuleStub11(void);
void MBCapsuleStub12(void);
void mbev_CapKillerMoveCheck(void);
void mbev_CapKillerMoveCheckAll(void);
void mbev_CapBankCoinInit(void);
void mbev_CapInit(void);
void mbev_CapKillerCall(void);
void mbev_CapKillerMultiCall(void);
void mbev_CapOpeningAdd(void);
void mbev_CapKoopaAdd(void);
void mbev_CapBankCoinGet(void);
void mbev_CapBubbleHookSet(void);
void mbev_CapBubbleHookCall(void);
void mbev_CapBubbleHookCallStory(void);
void mbev_CapKettouCoinLoseGet(void);
void mbev_CapKettouOppCoinLoseGet(void);
void mbev_CapKettouCoinLoseGet2(void);
void mbev_CapKettouOppCoinLoseGet2(void);
void mbev_CapPlayerMotionCreate(void);
void mbev_CapObjCreate(void);
void mbev_CapObjPosSet(void);
void mbev_CapPlayerPosSet(void);
void mbev_CapSprCreate(void);
void mbev_CapMalloc(void);
void mbev_CapObjClose(void);
void mbev_CapSprClose(void);
void mbev_CapMallocClose(void);
void mbev_CapEffExplodeCreate(void);
void mbev_CapEffExhaustCreate(void);
void mbev_CapEffExplodeOMExec(void);
void mbev_CapEffExplodeKill(void);
void mbev_CapEffExplodeAnimGet(void);
void mbev_CapEffExplodeAnimSet(void);
void mbev_CapEffExplodeAdd(void);
void mbev_CapEffExplodeKillerAdd(void);
void mbev_CapEffExplodeCircleAdd(void);
void mbev_CapEffDustCloudAdd(void);
void mbev_CapEffDustExplodeAdd(void);
void mbev_CapEffDustHeavyAdd(void);
void mbev_CapEffDustMultiAdd(void);
void mbev_CapEffBoostCreate(void);
void mbev_CapEffBoostOMExec(void);
void mbev_CapEffBoostKill(void);
void mbev_CapEffBoostTimeGet(void);
void mbev_CapEffBoostBlendModeSet(void);
void mbev_CapEffBoostAdd(void);
void mbev_CapEffSnowCreate(void);
void mbev_CapEffSnowOMExec(void);
void mbev_CapEffSnowKill(void);
void mbev_CapEffSnowDispGet(void);
void mbev_CapEffSnowAdd(void);
void mbev_CapEffGlowCreate(void);
void mbev_CapEffGlowFireCreate(void);
void mbev_CapEffGlowOMExec(void);
void mbev_CapEffGlowKill(void);
void mbev_CapEffGlowDispGet(void);
void mbev_CapEffGlowPatSet(void);
void mbev_CapEffGlowBlendModeSet(void);
void mbev_CapEffGlowAnimSet(void);
void mbev_CapEffGlowAdd(void);
void mbev_CapEffGlowKinokoAdd(void);
void mbev_CapEffGlowKinokoAddAlt(void);
void mbev_CapEffGlowKinokoTimeSet(void);
void mbev_CapEffGlowCoinAdd(void);
void mbev_CapEffRingCreate(void);
void mbev_CapEffRingHitCreate(void);
void mbev_CapEffRingOMExec(void);
void mbev_CapEffRingKill(void);
void mbev_CapEffRingDispGet(void);
void mbev_CapEffRingAdd(void);
void mbev_CapEffRingHitAdd(void);
void mbev_CapEffRingAnimSet(void);
void mbev_CapEffElectricCreate(void);
void mbev_CapEffElectricOMExec(void);
void mbev_CapEffElectricKill(void);
void mbev_CapEffElectricDispGet(void);
void mbev_CapEffElectricAdd(void);
void mbev_CapEffElectricModelSet(void);
void mbev_CapEffRayCreate(void);
void mbev_CapEffRayOMExec(void);
void mbev_CapEffRayKill(void);
void mbev_CapEffRayDraw(void);
void mbev_CapEffRayAdd(void);
void mbev_CapEffRayAlphaSet(void);
void mbev_CapEffRayTransformSet(void);
void mbev_CapEffMasuHitCreate(void);
void mbev_CapEffMasuHitOMExec(void);
void mbev_CapEffMasuHitKill(void);
void mbev_CapEffMasuHitAdd(void);
void mbev_CapEffMasuHitTransformSet(void);
void mbev_CapEffCoinCreate(void);
void mbev_CapEffCoinOMExec(void);
void mbev_CapEffCoinKill(void);
void mbev_CapEffCoinNumGet(void);
void mbev_CapEffCoinAdd(void);
void mbev_CapEffCoinMaxYSet(void);
void mbev_CapEffCoinMultiAdd(void);
void mbev_CapEffCoinGlowSet(void);
void mbev_CapCoinAdd(void);
void mbev_CapEffCapLoseCreate(void);
void mbev_CapEffCapLoseOMExec(void);
void mbev_CapEffCapLoseKill(void);
void mbev_CapEffCapLoseNumGet(void);
void mbev_CapEffCapLoseObjAdd(void);
void mbev_CapEffCapLoseAdd(void);
void mbev_CapCoinManCreate(void);
void mbev_CapCoinManOMExec(void);
void mbev_CapCoinManKill(void);
void mbev_CapCoinManNumGet(void);
void mbev_CapCoinManObjAdd(void);
void mbev_CapCoinManAdd2(void);
void mbev_CapCoinManAdd(void);
void mbev_CapStarManCreate(void);
void mbev_CapStarManOMExec(void);
void mbev_CapStarManKill(void);
void mbev_CapStarManNumGet(void);
void mbev_CapStarManObjAdd(void);
void mbev_CapStarManAdd2(void);
void mbev_CapStarManAdd(void);
void mbev_CapPlayerMoveObjInit(void);
void mbev_CapPlayerMoveHitCreate(void);
void mbev_CapPlayerMoveEjectCreate(void);
void mbev_CapPlayerMoveIdleCreate(void);
void mbev_CapPlayerMoveMinYSet(void);
void mbev_CapPlayerMoveVelSet(void);
void mbev_CapPlayerMoveObjExec(void);
void mbev_CapPlayerMoveObjCheck(void);
void mbev_CapPlayerMoveObjClose(void);
void mbev_CapPlayerMoveObjKill(void);
void mbev_CapPlayerSquishSet(void);
void mbev_CapPlayerSquishVoiceSet(void);
void mbev_CapPlayerStunSet(void);
void mbev_CapPlayerShockSet(void);
void mbev_CapPlayerCheck(void);
void mbev_CapCullPlayerCheck(void);
void mbev_CapCullCheck(void);
void mbev_CapPointCullCheck(void);
void mbev_CapPlayerMasuNumGet(void);
void mbev_CapPlayerNoSearch(void);
void mbev_CapPlayerOrderGet(void);
void mbev_CapMasuLinkNextGet(void);
void mbev_CapMasuLinkNextRandomGet(void);
void mbev_CapMasuValidPrevGet(void);
void mbev_CapMasuPrevGet(void);
void mbev_CapPlayerComSelGet(void);
void mbev_CapPlayerComSelSameGet(void);
void mbev_CapPlayerComSelRandomGet(void);
void mbev_CapPlayerComSelKettouGet(void);
void mbev_CapEffColorSet(void);
void mbev_CapPlayerMotShiftCheck(void);
void mbev_CapPlayerMotShiftSet(void);
void mbev_CapPlayerMotShiftWait(void);
void mbev_CapObjMotionSet(void);
void mbev_CapObjMotionOMExec(void);
void mbev_CapPlayerMotionSet(void);
void mbev_CapPlayerMotionOMExec(void);
void mbev_CapPlayerRotate(void);
void mbev_CapPlayerIdleWait(void);
void mbev_CapCoinDisp(void);
void mbev_CapMasuMoveCheck(void);
void mbev_CapVibrate(void);
void mbev_CapStatusDispSet(void);
void mbev_CapDuelStatusOffSet(void);
void mbev_CapDuelStatusOnSet(void);
void mbev_CapDuelStatusDispSet(void);
void mbev_CapStatusDispSetAll(void);
void mbev_CapStatusDispCheck(void);
void mbev_CapChoiceSet(void);
void mbev_CapAngleWrap(void);
void mbev_CapAngleLerp(void);
void mbev_CapAngleSumLerp(void);
void mbev_CapVecChase(void);
void mbev_CapVecRotGet(void);
void mbev_CapColorLerp(void);
void mbev_CapHermiteConstGet(void);
void mbev_CapHermiteConstGet2(void);
void mbev_CapHermiteGetV(void);
void mbev_CapBezierGet(void);
void mbev_CapBezierGetV(void);
void mbev_CapBezierSlopeGet(void);
void mbev_CapBezierNormGetV(void);
void mbev_ShopEnableSet(void);
void mbev_ShopInit(void);
void mbev_ShopCreate(void);
void mbev_ShopExInit(void);
void mbev_Shop(void);
void mbev_ShopExObjHookSet(void);
void mbev_ShopBackCreate(void);
void mbev_ShopBackMotCreate(void);
void mbCapUse(void);
void MBCapsuleStub1(void);
void MBCapsuleStub2(void);
void mbCapEffUseCreate(void);
void mbCapEffUseModeGet(void);
void mbCapEffUsePosGet(void);
void mbCapEffUseWanWanCreate(void);
void mbCapPlayerThrow(void);
void mbCapPlayerThrowCheck(void);
void mbCapThrowColCreate(void);
void mbCapThrowColCheck(void);
void mbCapEffThrowCreate(void);
void mbCapEffThrowCheck(void);
void mbCapAutoThrow(void);
void mbCapThrowHookSet(void);
void mbCapSelectMasu(void);
void mbCapSelectMasuInit(void);
void mbCapListInit(void);
void mbCapListRead(void);
void mbCapListCopy(void);
void mbCapListDebug(void);
void mbCapNumInc(void);
void mbCapNumDebug(void);
void mbCapValueTypeGet(void);
void mbCapMasuTypeGet(void);
void mbCapValuePlayerGet(void);
void mbCapMasuPlayerGet2(void);
void mbCapMasuPlayerSet(void);
void mbCapMasuPlayerTypeSet(void);
void mbCapMasuPlayerGet(void);
void mbCapUseModeGet(void);
void mbCapMasuDispTypeGet(void);
void mbCapUseTrapCheck(void);
void mbCapBuyCostGet(void);
void mbCapSellCostGet(void);
void mbCapCostGet(void);
void mbCapThrowMasuCheck(void);
void mbCapComChanceGet(void);
void mbCapSelectComGet(void);
void mbCapSelectDeleteComGet(void);
void mbCapRandomThrowAdd(void);
void mbCapRandomTrapAdd(void);
void mbCapMasuCapsuleSet(void);
void mbCapFileGet(void);
void mbCapDescMesGet(void);
void mbCapUseMesGet(void);
void mbCapDebugNameGet(void);
void mbCapMasuPatGet(void);
void mbCapColorGet(void);
void mbCapUseCostGet(void);
void mbCapUseCheck(void);
void mbCapValidCheck(void);
void mbCapSelectMasuNum(void);
void mbCapSelectMasuFrontNum(void);
void mbCapSelectMasuBackNum(void);
void mbCapValidListGet(void);
void mbCapNextGet(void);
void mbCapMasuNextGet(void);
void mbCapShopListGet(void);
void mbCapRandomListGet(void);
void mbCapBonusCoinNumGet(void);
void mbCapDescWinCreate(void);
void mbCapInit(void);
void mbCapObjCreate(void);
void mbCapObjBorderCreate(void);
void mbCapObjKill(void);
void mbCapObjBorderKill(void);
void mbCapObjColorCreate(void);
void mbCapObjColorKill(void);
void mbCapObjColorPosSet(void);
void mbCapObjColorRotSet(void);
void mbCapObjColorScaleSet(void);
void mbCapObjColorPosSetV(void);
void mbCapObjColorRotSetV(void);
void mbCapObjColorScaleSetV(void);
void mbCapObjColorPosGet(void);
void mbCapObjColorRotGet(void);
void mbCapObjColorScaleGet(void);
void mbCapObjColorLayerSet(void);
void mbCapObjColorLayerGet(void);
void mbCapObjColorAttrSet(void);
void mbCapObjColorAttrReset(void);
void mbCapObjColorDispSet(void);
void mbCapObjColorAlphaSet(void);
void mbCapObjColorMtxSet(void);
void mbCapObjColorMtxGet(void);
void mbCoinInit(void);
void mbCoinClose(void);
void mbCoinEffObjCreate(void);
void mbCoinEffCreate(void);
void mbCoinDispKill(void);
void mbCoinDispKillCheck(void);
void mbCoinDispCapsuleCreate(void);
void mbCoinDispMasuCreate(void);
void mbCoinDispCreate(void);
void mbCoinAddProcExec(void);
void mbCoinAddDispExec(void);
void mbCoinAddExec(void);
void mbCoinAddAllProcExecV(void);
void mbCoinAddAllProcExec(void);
void mbCoinAddAllExec(void);
void mbStatTeamMinValGet(void);
void mbCoinCreate(void);
void mbCoinCreate2(void);
void mbCoinObjCreate(void);
void mbCoinObjCreate2(void);
void mbCoinObjNumDec(void);
void mbCoinObjKill(void);
void mbCoinObjGet(void);
void mbCoinObjPosSet(void);
void mbCoinObjPosSetV(void);
void mbCoinObjRotSet(void);
void mbCoinObjRotSetV(void);
void mbCoinObjScaleSet(void);
void mbCoinObjScaleSetV(void);
void mbCoinObjAlphaSet(void);
void mbCoinObjDispSet(void);
void mbCoinObjLayerSet(void);
void mbCoinObjMotSet(void);
void mbCoinObjPosGet(void);
void mbCoinObjRotGet(void);
void mbCoinObjScaleGet(void);
void mbCoinObjAlphaGet(void);
void mbCoinObjDispGet(void);
void mbComChoiceLeftSet(void);
void mbComChoiceUpSet(void);
void mbComChoiceRightSet(void);
void mbComChoiceDownSet(void);
void mbComChoiceListDownSet(void);
void mbComChoiceListRightSet(void);
void mbComChoiceListSet(void);
void mbConfigExec(void);
void mbPauseDispCopyKill(void);
void mbPauseDispCopyCreate(void);
void mbConfigPadDisableSet(void);
void mbPauseGuideCreate(void);
void mbPauseGuideKill(void);
void mbPausePanelCreate(void);
void mbPausePanelKill(void);
void mbPausePanelPosSet(void);
void mbPausePanelRotSet(void);
void mbPausePanelScaleSet(void);
void mbPausePanelBankSet(void);
void mbPausePanelBatsuSet(void);
void mbPausePanelPosGet(void);
void mbPausePanelScaleGet(void);
void mbPausePanelFreezeGet(void);
void mbPausePanelUnlockSet(void);
void mbPausePanelSlideSet(void);
void mbPausePanelSizeSet(void);
void mbPausePanelAnmNoSet(void);
void mbPausePanelGrowSet(void);
void mbPausePanelShrinkSet(void);
void mbPauseGuideMoveSet(void);
void mbPadDStkRepGetAll(void);
void mbPadBtnDownGetAll(void);
void mbev_Last5(void);
void mbDiceInit(void);
void mbDicePlayerExec(void);
void mbDiceExec(void);
void mbDiceProcExec(void);
void mbDiceChanceTradeExec(void);
void mbDiceChanceCharExec(void);
void mbDiceFadeSet(void);
void mbDiceKill(void);
void mbDiceClose(void);
void mbDiceKillCheck(void);
void mbDiceKillCheckAll(void);
void mbDiceResultGet(void);
void mbDiceHitHookSet(void);
void mbDiceTypeGet(void);
void mbDiceTutorialNumSet(void);
void mbDiceTutorialNumGet(void);
void mbDiceValueNoGet(void);
void mbDiceNumKill(void);
void mbDiceNumShrinkSet(void);
void mbDiceNumStopCheck(void);
void mbDiceStub(void);
void mbDiceSNpcNumCreate(void);
void mbDiceNumObjCreate(void);
void mbDiceMaxGet(void);
void mbDiceValueMaxGet(void);
void mbDicePadBtnHookSet(void);
void mbDiceMotHookSet(void);
void mbDiceObjHit(void);
void mbDiceSNpcNumObjCreate(void);
void mbDiceSNpcNumKill(void);
void mbDiceSNpcNumDispSet(void);
void mbDiceSNpcNumSet(void);
void mbDiceSNpcNumPosSet(void);
void mbDiceSNpcNumOfsSet(void);
void mbDiceSNpcNumOfsGet(void);
void mbEffInit(void);
void mbEffClose(void);
void mbEffConfettiCreate(void);
void mbEffConfettiKill(void);
void mbEffConfettiReset(void);
void mbEffFadeOutSet(void);
void mbEffFadeCreate(void);
void mbEffFadeDoneCheck(void);
void mbEffFadeCheck(void);
void mbEffFadeCameraSet(void);
void mbParticleCreate(void);
void mbParticleKill(void);
void mbParticleHookSet(void);
void mbParticleAttrSet(void);
void mbParticleAttrReset(void);
void mbParticleUnkTotalGet(void);
void mbParticleDataCreate(void);
void mbParticleColorCreate(void);
void mbParManCreate(void);
void mbParManKill(void);
void mbParManPosSet(void);
void mbParManVecSet(void);
void mbParManRotSet(void);
void mbParManAttrSet(void);
void mbParManAttrReset(void);
void mbParticleBlendModeSet(void);
void mbGateInit(void);
void mbGateClose(void);
void mbGateCreate(void);
void mbev_GateMasu(void);
void mbev_GateBattanMasuFind(void);
void mbev_GateBattan(void);
void mbGateBattanKill(void);
void mbGuideInit(void);
void mbGuideCreateIn(void);
void mbGuideCreateFlag(void);
void mbGuideCreate(void);
void mbGuideKill(void);
void mbGuideEnd(void);
void mbGuideFadeIn(void);
void mbGuideFadeOut(void);
void mbGuideModelGet(void);
void mbGuideIdleCheck(void);
void mbGuideScreenSet(void);
void mbGuideMotionNextSet(void);
void mbGuideMotionSet(void);
void mbGuideMotionShiftSet(void);
void mbGuideMotionStop(void);
void mbGuideMotionCheck(void);
void mbGuideNoGet(void);
void mbGuideSpeakerNoGet(void);
void mbExitWatchCreate(void);
void mbExitReq(void);
void mbExitCheck(void);
void mbExitWatchProcGet(void);
void mbChangeTimeSet(void);
void mbChangeTime(void);
void mbev_NextTimeSet(void);
void mbev_LoadTimeSet(void);
void mbObjectSetup(void);
void mbPauseEnableSet(void);
void mbPauseEnableReset(void);
void mbPauseEnableCheck(void);
void mbLightSet(void);
void mbLightReset(void);
void mbLightFuncSet(void);
void mbBGRead(void);
void mbBGReadWait(void);
void mbReturnMgCheck(void);
void mbOvlCall(void);
void mbDirClose(void);
void mbev_TurnStartSet(void);
void mbev_TurnEndSet(void);
void fn_8014C3B4(void);
void fn_8014C3BC(void);
void fn_8014C3C4(void);
void fn_8014C3CC(void);
void mbScrollInit(void);
void mbScrollClose(void);
void mbev_Scroll(void);
void mbev_ScrollCapsule(void);
void mbScrollStarFindFuncSet(void);
void mbScrollHookSet(void);
void mbMapCameraSet(void);
void mbMapHookSet(void);
void mbMapSprAdd(void);
void mbev_StarScroll(void);
void mbMasuInit(void);
void mbMasuClose(void);
void mbev_MasuStop(void);
void mbMasuDataRead(void);
void mbev_MasuMasuEnd(void);
void mbev_MasuMasuStart(void);
void mbev_MasuCapStop(void);
void mbev_MasuMove(void);
void mbMasuNextSet(void);
void mbMasuNextDispSet(void);
void mbMasuNumGet(void);
void mbMasuRawNumGet(void);
void mbMasuGet(void);
void mbMasuLayerSet(void);
void mbMasuLayerGet(void);
void mbMasuAttrGet(void);
void mbMasuAttrSet(void);
void mbMasuMAttrGet(void);
void mbMasuMAttrSet(void);
void mbev_MasuBitGet(void);
void mbev_MasuAttrGet(void);
void mbMasuTypeGet(void);
void mbMasuTypeSet(void);
void mbMasuTypeChange(void);
void mbMasuDispCheck(void);
void mbMasuCapsuleReset(void);
void mbMasuCapsuleSet(void);
void mbMasuCapsuleGet(void);
void mbMasuPosGet(void);
void mbMasuPosSet(void);
void mbMasuPosSetV(void);
void mbMasuCornerRotPosGet(void);
void mbMasuCornerPosGet(void);
void mbMasuRotGet(void);
void mbMasuRotSet(void);
void mbMasuRotSetV(void);
void mbMasuMtxGet(void);
void mbMasuMtxSet(void);
void mbMasuDispGet(void);
void mbMasuDispSet(void);
void mbMasuDispAttrReset(void);
void mbMasuDispAttrSet(void);
void mbMasuDispAttrGet(void);
void mbMasuDispMAttrReset(void);
void mbMasuDispMAttrSet(void);
void mbMasuDispMAttrGet(void);
void mbMasuModelDispSet(void);
void mbMasuLinkGet(void);
void mbMasuLinkNumGet(void);
void mbMasuLinkTblGet(void);
void mbMasuLinkTblGet2(void);
void mbMasuAttrFindLink(void);
void mbMasuAttrMatchFindLink(void);
void mbMasuMAttrFindLink(void);
void mbMasuMAttrMatchFindLink(void);
void mbMasuTypeFindLink(void);
void mbMasuLinkParentGet(void);
void mbMasuFind_TypeStepGet(void);
void mbMasuFind_TypeStepGet2(void);
void mbMasuFind_AttrStepGet(void);
void mbMasuFind_AttrMatchStepGet(void);
void mbMasuFind_AttrStepGet2(void);
void mbMasuFind_MAttrStepGet(void);
void mbMasuFind_MAttrMatchStepGet(void);
void mbMasuFind_MAttrStepGet2(void);
void mbMasuFind_IdStepGet(void);
void mbMasuFind_IdStepGet2(void);
void mbMasuFind_TypeSearch(void);
void mbMasuFind_TypeIdGet(void);
void mbMasuFind_AttrIdGet(void);
void mbMasuFind_AttrMatchIdGet(void);
void mbMasuFind_AttrMatchIdGet2(void);
void mbMasuFind_MAttrIdGet(void);
void mbMasuFind_MAttrMatchIdGet(void);
void mbMasuFind_MAttrMatchIdGet2(void);
void mbMasuTypeListGet(void);
void mbMasuAttrListGet(void);
void mbMasuAttrMatchListGet(void);
void mbMasuMAttrListGet(void);
void mbMasuMAttrMatchTblGet(void);
void mbMasuFind_TypeListGet(void);
void mbMasuFind_TypeListGet2(void);
void mbMasuFind_AttrNumGet(void);
void mbMasuFind_AttrMatchNumGet(void);
void mbMasuFind_AttrMatchNumGet2(void);
void mbMasuFind_MAttrNumGet(void);
void mbMasuFind_MAttrMatchListGet(void);
void mbMasuFind_MAttrMatchListGet2(void);
void mbMasuFind_IdListGet(void);
void mbMasuFind_IdListGet2(void);
void mbev_MasuMoveEndSet(void);
void mbev_MasuHatenaSet(void);
void mbev_MasuMoveStartSet(void);
void mbev_MasuLinkTblHookSet(void);
void mbMasuPlayerDispSet(void);
void mbMasuPlayerFadeSet(void);
void mbMasuPlayerCapMoveCheck(void);
void mbMasuPlayerCapStopCheck(void);
void mbMasuStub(void);
void mbMasuPlayerColorSet(void);
void mbMasuPlayerPrizeReset(void);
void mbMasuPKinokoValueGet(void);
void mbMathInit(void);
void mbMathClose(void);
void mbRandMod(void);
void mbVecMagCheck(void);
void mbAngleAdd(void);
void mbAngleMoveTo(void);
void mbAngleWrap2(void);
void mbVecMagXZ(void);
void mbMtxLookAtCalc(void);
void mbAngleWrapV(void);
void mbAngleWrap(void);
void mbVecMagXZCheck(void);
void mbBezierCalc(void);
void mbBezierCalcV(void);
void mbBezierCalcVList(void);
void mbBezierCalcSlope(void);
void mbBezierCalcSlopeV(void);
void mbHermiteCalc(void);
void mbHermiteCalcV(void);
void mbHermiteCalcSlope(void);
void mbPos3Dto2D(void);
void mbPos2Dto3D(void);
void mbPos3DtoNorm(void);
void mbNormPosto3D(void);
void mbNormPosto2D(void);
void mbAngleLerp(void);
void mbAngleEaseOut(void);
void mbAngleEaseIn(void);
void mbMathDistScale(void);
void mbCosDeg(void);
void mbSinDeg(void);
void mbCosRad(void);
void mbSinRad(void);
void mbMtxRot(void);
void mbMtxRotAxisDeg(void);
void mbMtxRotAxisRad(void);
void mbMtxRotXDeg(void);
void mbMtxRotXRad(void);
void mbMtxRotYDeg(void);
void mbMtxRotYRad(void);
void mbMtxRotZDeg(void);
void mbMtxRotZRad(void);
void mbMtxScaleRotXDeg(void);
void mbMtxScaleRotYDeg(void);
void mbMtxScaleRotZDeg(void);
void mbMtxRotTrigX(void);
void mbMtxRotTrigY(void);
void mbMtxRotTrigZ(void);
void mbMtxRotTrigScaleX(void);
void mbMtxRotTrigScaleY(void);
void mbMtxRotTrigScaleZ(void);
void mbMtxTransCat(void);
void mbObjCullInit(void);
void mbTelopPlayerCreate(void);
void mbTelopPlayerSkipCreate(void);
void mbTelopCreate(void);
void mbTelopCheck(void);
void mbTelopLastTurnCreate(void);
void mbPadStkXGet(void);
void mbPadStkYGet(void);
void mbTauntInit(void);
void mbTauntClose(void);
void mbLanguageGet(void);
void mbLanguageSet(void);
void mbBoardDataNumGet(void);
void mbBoardDataDirRead(void);
void mbTelopTimeChangeCreate(void);
void mbTelopTimeChangeCheck(void);
void mbTelopTimeChangeKill(void);
void mbTelopTimeCreate(void);
void mbTelopTimeSprCreate(void);
void mbTelopTimeSprKill(void);
void mbTelopTimeDispSet(void);
void mbTelopTimeSprRotSet(void);
void mbTelopTimeStarSet(void);
void mbTelopTimeTPLvlSet(void);
void mbObjInit(void);
void mbObjClose(void);
void mbObjKill(void);
void mbObjDirSet(void);
void mbObjDataNumGet(void);
void mbObjCreate(void);
void mbObjCharCreate(void);
void mbObjGet(void);
void mbObjDispGet(void);
void mbObjDispSet(void);
void mbObjCameraSet(void);
void mbObjLayerSet(void);
void mbObjLayerGet(void);
void mbObjLayerSetAll(void);
void mbObjModelIDGet(void);
void mbObjPosGet(void);
void mbObjPosSetV(void);
void mbObjPosSet(void);
void mbObjOffsetGet(void);
void mbObjOffsetSetV(void);
void mbObjOffsetSet(void);
void mbObjRotGet(void);
void mbObjRotSetV(void);
void mbObjRotSet(void);
void mbObjRotYGet(void);
void mbObjRotYSet(void);
void mbObjScaleGet(void);
void mbObjScaleSetV(void);
void mbObjScaleSet(void);
void mbObjMtxGet(void);
void mbObjMtxSet(void);
void mbObjAttrSet(void);
void mbObjAttrReset(void);
void mbObjAmbSet(void);
void mbObjAmbSetAll(void);
void mbObjAlphaGet(void);
void mbObjAlphaSet(void);
void mbObjFadeSet(void);
void mbObjZWriteOffSet(void);
void mbObjSePlay(void);
void mbObjMotionVoiceOnSet(void);
void mbObjCullRadiusSet(void);
void mbObjHookSet(void);
void mbObjHookReset(void);
void mbObjHookObjReset(void);
void mbObjMotionGet(void);
void mbObjMotionSet(void);
void mbObjMotionIDCurGet(void);
void mbObjMotionIDGet(void);
void mbObjMotionShiftSet(void);
void mbObjMotionShiftIDGet(void);
void mbObjMotionNoCreate(void);
void mbObjMotionCreate(void);
void mbObjMotionKill(void);
void mbObjMotionTimeGet(void);
void mbObjMotionTimeSet(void);
void mbObjMotionMaxTimeGet(void);
void mbObjMotionSpeedGet(void);
void mbObjMotionSpeedSet(void);
void mbObjMotionEndCheck(void);
void mbObjMotionStartEndSet(void);
void mbObjMotionLoopSet(void);
void mbObjMotionShapeSet(void);
void mbObjMotionShapeTimeGet(void);
void mbObjMotionShapeTimeSet(void);
void mbObjMotionShapeMaxTimeGet(void);
void mbObjMotionShapeSpeedGet(void);
void mbObjMotionShapeSpeedSet(void);
void mbObjMotionShapeEndCheck(void);
void mbObjMotionShapeStartEndSet(void);
void mbObjShadowSet(void);
void mbObjShadowReset(void);
void mbObjShadowMapSet(void);
void mbObjShadowMapReset(void);
void mbObjStub0(void);
void mbev_OpeningParty(void);
void mbev_OpeningSingle(void);
void mbev_Opening(void);
void mbOpeningViewSet(void);
void mbOpeningZoomGet(void);
void mbOpeningRotGet(void);
void mbOpeningPosGet(void);
void mbOpeningInstHookSet(void);
void mbOpeningStarInstHookSet(void);
void mbOpeningGuideObjIdGet(void);
void mbOpeningGuidePosRestore(void);
void mbOpeningCameraPosRestore(void);
void mbPauseInit(void);
void mbPauseCreate(void);
void mbPauseStartCheck(void);
void mbPauseProcCheck(void);
void mbPauseDisableSet(void);
void mbPauseDisableGet(void);
void mbPauseHookPush(void);
void mbPauseHookPop(void);
void mbPauseSet(void);
void mbPauseDataDirGet(void);
void mbPauseDataNumGet(void);
void mbPlayerMetalSet(void);
void mbPlayerEffectSet(void);
void mbPlayerMetalColorSet(void);
void mbPlayerBiriQSet(void);
void mbPlayerInit(void);
void mbPlayerClose(void);
void mbTurnExec(void);
void mbSingleTurnExec(void);
void mbPlayerSwap(void);
void mbPlayerNameMesGet(void);
void mbPlayerNameGet(void);
void mbPlayerTagNameMesGet(void);
void mbPlayerColorGet(void);
void mbPlayerAmbSet(void);
void mbPlayerWorkGet(void);
void mbPlayerAllComCheck(void);
void mbPlayerObjIDGet(void);
void mbPlayerModelIDGet(void);
void mbPlayerDispSet(void);
void mbPlayerDispGet(void);
void mbPlayerLayerSet(void);
void mbPlayerCameraSet(void);
void mbPlayerCullRadiusSet(void);
void mbPlayerStubValSet(void);
void mbPlayerMtxSet(void);
void mbPlayerMtxGet(void);
void mbPlayerPosSet(void);
void mbPlayerPosSetV(void);
void mbPlayerPosGet(void);
void mbPlayerRotSet(void);
void mbPlayerRotSetV(void);
void mbPlayerRotGet(void);
void mbPlayerRotYSet(void);
void mbPlayerRotYGet(void);
void mbPlayerScaleSet(void);
void mbPlayerScaleSetV(void);
void mbPlayerScaleGet(void);
void mbPlayerMotionGet(void);
void mbPlayerMotionEndCheck(void);
void mbPlayerMotionEndCheckAll(void);
void mbPlayerMotionEndWait(void);
void mbPlayerMotionMaxTimeGet(void);
void mbPlayerMotionTimeGet(void);
void mbPlayerMotionSet(void);
void mbPlayerMotionVoiceOnSet(void);
void mbPlayerMotionShiftSet(void);
void mbPlayerMotionCreate(void);
void mbPlayerMotionKill(void);
void mbPlayerMotionSpeedSet(void);
void mbPlayerMotionTimeSet(void);
void mbPlayerMotionStartEndSet(void);
void mbPlayerAttrSet(void);
void mbPlayerAttrReset(void);
void mbPlayerMotIdleSet(void);
void mbPlayerRotateStart(void);
void mbPlayerRotateCheck(void);
void mbPlayerRotateCheckAll(void);
void mbPlayerEyeMatDarkSet(void);
void mbPlayerRankGet(void);
void mbPlayerCapsuleUseGet(void);
void mbPlayerCapsuleUseSet(void);
void mbPlayerCapsuleRemove(void);
void mbPlayerCapsuleFind(void);
void mbPlayerCapsuleAdd(void);
void mbPlayerCapsuleNumGet(void);
void mbPlayerCapsuleGet(void);
void mbPlayerCapsuleMaxGet(void);
void mbPlayerCoinAdd(void);
void mbPlayerCoinSet(void);
void mbPlayerCoinGet(void);
void mbPlayerMaxCoinGet(void);
void mbPlayerStarAdd(void);
void mbPlayerStarSet(void);
void mbPlayerStarGet(void);
void mbPlayerTeamFind(void);
void mbPlayerTeamFindOther(void);
void mbPlayerTeamCoinSet(void);
void mbPlayerTeamCoinGet(void);
void mbPlayerGrpStarSet(void);
void mbPlayerGrpStarGet(void);
void mbPlayerTeamRankGet(void);
void mbPlayerTeamFindPlayer(void);
void mbPlayerTeamCapsuleGet(void);
void mbPlayerTeamCapsuleNumGet(void);
void mbPlayerTeamFindOpp(void);
void mbPlayerTeamGet(void);
void mbPlayerTeamCheckSame(void);
void mbPlayerWalkSpeedGet(void);
void mbPlayerMoveExec(void);
void mbPlayerMoveMain(void);
void mbev_PlayerColReserve(void);
void mbev_PlayerColSet(void);
void mbMoveNumCreateColor(void);
void mbMoveNumCreate(void);
void mbMoveNumKill(void);
void mbMoveNumDispSet(void);
void mbPlayerMasuMoveTo(void);
void mbPlayerMasuMove(void);
void mbPlayerMasuMovePos(void);
void mbPlayerMasuMoveSpeed(void);
void mbPlayerColSnapSet(void);
void mbev_PlayerColMasuSet(void);
void mbev_PlayerColMasuAllSet(void);
void mbev_PlayerColMasuAdd(void);
void mbev_PlayerColMasu(void);
void mbev_PlayerColCircleAdd(void);
void mbev_PlayerColBall(void);
void mbPlayerColSnapPlayerSet(void);
void mbPlayerColSnapGet(void);
void mbPlayerColRestSet(void);
void mbPlayerColCheck(void);
void mbPlayerBestPathGet(void);
void mbPlayerPosReset(void);
void mbPlayerPosResetAll(void);
void mbPlayerMasuCornerSet(void);
void mbPlayerMasuCornerGet(void);
void mbPlayerDiceMotExec(void);
void mbPlayerKettouCheck(void);
void mbPlayerBlackoutSet(void);
void mbPlayerBlackoutGet(void);
void mbPlayerWinLoseVoicePlay(void);
void mbPlayerVoicePlay(void);
void mbPlayerVoicePanPlay(void);
void mbPlayerStartTurnHookSet(void);
void mbPlayerEndTurnHookSet(void);
void mbPlayerMoveHookSet(void);
void mbPlayerDiceTypeGet(void);
void mbPlayerTurnInitHookSet(void);
void mbPlayerTurnCloseHookSet(void);
void mbPlayerColFirstSet(void);
void mbPlayerColOrderReset(void);
void mbPlayerPlusMasuExec(void);
void mbPlayerMinusMasuExec(void);
void mbPlayerCapCoinMasuExec(void);
void mbRouletteCreate(void);
void mbRouletteKaneCreate(void);
void mbRouletteCheck(void);
void mbRouletteWait(void);
void mbRouletteResultGet(void);
void mbRouletteMaxSpeedSet(void);
void mbRouletteComValueSet(void);
void mbSingleInit(void);
void mbSingleClose(void);
void mbSingleReturn(void);
void mbSingleReturnWrite(void);
void mbSingleGameEnd(void);
void mbSingleSaveInit(void);
void mbSingleMgUnlockWrite(void);
void mbSingleMgUnlockInit(void);
void mbSingleMgUnlockSet(void);
void mbSingleMgUnlockReset(void);
void mbSingleMgUnlockGet(void);
void mbSingleMgUnlockCheckAny(void);
void mbSingleMgUnlockNumGet(void);
void mbev_SingleMg(void);
void mbev_SingleMgEnd(void);
void mbSingleStepGet(void);
void mbSingleCall(void);
void mbSingleOppCharGet(void);
void mbSingleTeamCharSet(void);
void mbSingleTeamCharGet(void);
void mbSingleSaveFlush(void);
void mbSingleMgUnlockCheck(void);
void mbMusPlay(void);
void mbMusFadeOut(void);
void mbMusFadeOutSpeed(void);
void mbMusStub(void);
void mbMusParamSet(void);
void mbMusPauseFadeOut(void);
void mbMusStatGet(void);
void mbAudClose(void);
void mbAudInit(void);
void mbAudStub1(void);
void mbAudStub2(void);
void mbMusBoardPlay(void);
void mbMusCheck(void);
void mbMusEndCheck(void);
void mbMusStopCheck(void);
void mbMusFadeCheck(void);
void mbMusPauseSet(void);
void mbMusJinglePlay(void);
void mbMusJingleWait(void);
void mbMusJingleStatGet(void);
void mbAudFXObjSet(void);
void mbAudFXObjCreate(void);
void mbAudFXObjVolSet(void);
void mbAudFXObjKill(void);
void mbAudFXObjClose(void);
void mbAudFXObjEmitterSet(void);
void mbAudFXObjEmitterCreate(void);
void mbAudFXObjEmitterUpdate(void);
void mbAudFXObjEmiterPosSet(void);
void mbAudFXObjRefSet(void);
void mbAudFXPlay(void);
void mbAudFXStop(void);
void mbAudFXStopAll(void);
void mbAudFXPosPlay(void);
void mbAudFXPosPanGet(void);
void mbAudFXEmitterPlay(void);
void mbAudFXVolSet(void);
void mbAudFXPanning(void);
void mbAudFXPosPanning(void);
void mbAudFXDelaySet(void);
void mbAudGuidePlay(void);
void mbev_StarMasu(void);
void mbev_StarFreeMasu(void);
void mbStarGetMain(void);
void mbStarGetPosExec(void);
void mbStarMapViewExec(void);
void mbStarStub2(void);
void mbStarInit(void);
void mbStarClose(void);
void mbStarStub(void);
void mbStarNumSet(void);
void mbStarNumGet(void);
void mbStarFlagInit(void);
void mbStarFlagSet(void);
void mbStarFlagReset(void);
void mbStarFlagGet(void);
void mbStarNextNoSet(void);
void mbStarNextNoGet(void);
void mbStarNoSet(void);
void mbStarNoRandGet(void);
void mbStarReset(void);
void mbStarNoDispSet(void);
void mbStarMasuDispSet(void);
void mbStarDispSetAll(void);
void mbStarObjDispSetAll(void);
void mbStarMasuNextSet(void);
void mbStarGetExec(void);
void mbStarMoveHookSet(void);
void mbStarMasuFuncSet(void);
void mbStarAddNumSet(void);
void mbStarDispCreate(void);
void mbStarDispPlayerCreate(void);
void mbStarDispCheck(void);
void mbStarObjCreate(void);
void mbStarObjPosSet(void);
void mbStarObjRotSet(void);
void mbStarObjScaleSet(void);
void mbStarObjPosSetV(void);
void mbStarObjRotSetV(void);
void mbStarObjScaleSetV(void);
void mbStarObjPosGet(void);
void mbStarObjRotGet(void);
void mbStarObjScaleGet(void);
void mbStarObjDispSet(void);
void mbStarObjDispFlagSet(void);
void mbStarObjKill(void);
void mbStarChestCreate(void);
void mbStarAddProcExec(void);
void mbStarAddDispExec(void);
void mbStarAddExec(void);
void mbStarAddAllProcExecV(void);
void mbStarAddAllProcExec(void);
void mbStarAddAllExec(void);
void mbZtarObjInit(void);
void mbZtarObjClose(void);
void mbZtarMasuNextSet(void);
void mbZtarGetExec(void);
void mbZtarGetMain(void);
void mbZtarObjDispFlagSet(void);
void mbStarMapViewProcExec(void);
void mbStatusInit(void);
void mbStatusClose(void);
void mbStatusReset(void);
void mbStatusDispGet(void);
void mbStatusDispSet(void);
void mbStatusDispFocusSet(void);
void mbStatusDispSetAll(void);
void mbStatusDispForceSet(void);
void mbStatusDispForceSetAll(void);
void mbStatusMoveCheck(void);
void mbStatusOffCheckAll(void);
void mbSprCreate(void);
void mbStatusPosGet(void);
void mbStatusNoPosGet(void);
void mbStatusCapsuleDispSet(void);
void mbStatusKaoNoSet(void);
void mbSprNumSet(void);
void mbStatusMoveTo(void);
void mbStatusMoveSet(void);
void mbStatusPosOnGet(void);
void mbStatusPosOffGet(void);
void mbStatusRainbowSet(void);
void mbStatusColorSet(void);
void mbStatusColorAllSet(void);
void mbStatusColorGet(void);
void mbStatusLayoutSet(void);
void mbStatusCapsuleFocusSet(void);
void mbStatusNoGet(void);
void mbStatusNoMoveSet(void);
void mbStatusDispBackup(void);
void mbStatusDispRestoreForce(void);
void mbStatusDispRestore(void);
void mbStatusMasuDispSet(void);
void mbSNpcInit(void);
void mbSNpcCreate(void);
void mbSNpcKill(void);
void mbSNpcMotReset(void);
void mbSNpcPlayerWalkSet(void);
void mbSNpcStarExec(void);
void mbSNpcMasuGet(void);
void mbSNpcMotWinSet(void);
void mbSNpcMotIdleSet(void);
void mbSNpcDispSet(void);
void mbMasuChanceCreate(void);
void mbMasuChanceTypeSet(void);
void mbMasuChanceFlagSet(void);
void mbMasuChancePlayerSet(void);
void mbMasuChanceSet(void);
void mbMasuChanceKill(void);
void mbSNpcMasuStarNextGet(void);
void mbObjStarTevStageSet(void);
void mbObjFadeCreate(void);
void mbObjFadeKill(void);
void mbObjFadeTexRotSet(void);
void mbObjFadeTexColorSet(void);
void mbObjMetalCreate(void);
void mbObjMetalKill(void);
void mbObjMetalTPLvlSet(void);
void mbObjMetalColorSet(void);
void mbObjBiriQCreate(void);
void mbObjBiriQKill(void);
void mbObjBiriQColorSet(void);
void mbTutorialInit(void);
void mbTutorialCreate(void);
void mbTutorialExitReqGet(void);
void mbTutorialExitSet(void);
void mbTutorialExitOnSet(void);
void mbTutorialCall(void);
void mbTutorialMainFuncSet(void);
void mbTutorialSprCreate(void);
void mbTutorialSprDispOn(void);
void mbTutorialSprDispOff(void);
void mbTutorialSprKill(void);
void mbTutorialSprGrpSet(void);
void mbTutorialSprGrpKill(void);
void mbTutorialModelCreate(void);
void mbTutorialModelKill(void);
void mbTutorialGuideCreate(void);
void mbTutorialGuideGet(void);
void mbTutorialGuideClose(void);
void mbTutorialWinWait(void);
void mbTutorialWinClose(void);
void mbTutorialWinMesExec(void);
void mbTutorialWinCreate(void);
void mbTutorialWinMesMasuExec(void);
void mbTutorialWinKeyWait(void);
void mbTutorialViewSet(void);
void mbTutorialViewMasuSet(void);
void mbTutorialVSleep(void);
void mbTutorialCallEnd(void);
void mbTutorialCallWait(void);
void mbTutorialMultiCall(void);
void mbTutorialCallResult(void);
void mbTutorialMesCall(void);
void mbTutorialMesMasuCall(void);
void mbTutorialModeSet(void);
void mbTutorialModeGet(void);
void mbTutorialResultSet(void);
void mbTutorialSceneGet(void);
void mbTutorialTurnCall(void);
void mbTutorialGuideCall(void);
void mbTutorialCapsuleUseCall(void);
void mbTutorialMgCallExec(void);
void mbTutorialMgCallClose(void);
void mbWinInit(void);
void mbWinClose(void);
void mbWinCreate(void);
void mbWinCreateChoice(void);
void mbWinCreateHelp(void);
void mbWinCreateFrame(void);
void mbWinCreateTime(void);
void mbWinCreateBlank(void);
void mbWinCreateBlankFrame(void);
void mbWinKillAll(void);
void mbWinKill(void);
void mbWinTopKill(void);
void mbWinPosSet(void);
void mbWinTopPosSet(void);
void mbWinPosGet(void);
void mbWinTopPosGet(void);
void mbWinSizeSet(void);
void mbWinTopSizeSet(void);
void mbWinMesMaxSizeGet(void);
void mbWinTopMesMaxSizeGet(void);
void mbWinScaleSet(void);
void mbWinTopScaleSet(void);
void mbWinScaleGet(void);
void mbWinTopScaleGet(void);
void mbWinChoiceGet(void);
void mbWinTopChoiceGet(void);
void mbWinPause(void);
void mbWinTopPause(void);
void mbWinInsertMesSet(void);
void mbWinTopInsertMesSet(void);
void mbWinDoneCheck(void);
void mbWinTopDoneCheck(void);
void mbWinWait(void);
void mbWinTopWait(void);
void mbWinAttrSet(void);
void mbWinTopAttrSet(void);
void mbWinAttrReset(void);
void mbWinTopAttrReset(void);
void mbWinChoiceDisable(void);
void mbWinTopChoiceDisable(void);
void mbWinMesSpeedSet(void);
void mbWinTopMesSpeedSet(void);
void mbWinMesColSet(void);
void mbWinTopMesColSet(void);
void mbWinChoiceNowGet(void);
void mbWinTopChoiceNowGet(void);
void mbWinPriSet(void);
void mbWinTopPriSet(void);
void mbWinChoiceNowGet2(void);
void mbWinTopChoiceNowGet2(void);
void mbWinPlayerDisable(void);
void mbWinTopPlayerDisable(void);
void mbWinIDGet(void);
void mbWinTopIDGet(void);
void mbWinDispSet(void);
void mbWinTopDispSet(void);
void mbWinPauseHook(void);
void mbWinGet(void);
void mbWinCenterGet(void);
void mbWinCenterInsertGet(void);
void mbWinTopCenterInsertGet(void);
void mbWipeSpecialInit(void);
void mbWipeSpecialClose(void);
void mbWipeSpecialKill(void);
void mbWipeWait(void);
void mbWipeCreate(void);
void mbWipeFadeOut(void);
void mbWipeFadeIn(void);
void mbWipeWhiteFadeOut(void);
void mbWipeWhiteFadeIn(void);
void mbWipeDissolveFadeOut(void);
void mbWipeDissolveFadeIn(void);
void mbWipeFadeOutTime(void);
void mbWipeFadeInTime(void);
void mbWipeWhiteFadeOutTime(void);
void mbWipeWhiteFadeInTime(void);
void mbWipeDissolveFadeOutTime(void);
void mbWipeDissolveFadeInTime(void);
void mbWipeSpecialCreate(void);
void mbWipeSpecialWait(void);
void mbWipeSpecialFadeOutCreate(void);
void mbWipeSpecialFadeInCreate(void);
void mbWipeSpecialStatGet(void);
void mbMgCallInit(void);
void mbev_MgCall(void);
void mbev_MgCallKettou(void);
void mbev_MgCallDonkey(void);
void mbev_MgCallKoopa(void);
void mbev_MgCallSingle(void);
void mbev_MgCallSingleKoopa(void);
void mbMgCallDataClose(void);
void mbev_MgCallTutorial(void);
void mbMgRouletteFocusKill(void);
void mbMgRouletteNumGet(void);
void mbMgCallSingleOnCheck(void);
void mbMgCallVsEffCreate(void);

// Only the RELs call these; keep the linker from dead-stripping them.
#ifdef __MWERKS__
#pragma force_active on
#endif

asm void fn_80213974(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSDumpStopwatch
#endif // clang-format on
}

asm void fn_80213978(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSVECDotProduct
#endif // clang-format on
}

asm void fn_8021397C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSVECDistance
#endif // clang-format on
}

asm void fn_80213980(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSVECSquareDistance
#endif // clang-format on
}

asm void fn_80213984(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b VIGetDTVStatus
#endif // clang-format on
}

asm void fn_80213988(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSGetProgressiveMode
#endif // clang-format on
}

asm void fn_8021398C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSSetProgressiveMode
#endif // clang-format on
}

asm void fn_80213990(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSVECSubtract
#endif // clang-format on
}

asm void fn_80213994(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSMTXTranspose
#endif // clang-format on
}

asm void fn_80213998(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b C_VECReflect
#endif // clang-format on
}

asm void fn_8021399C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSMTXInvXpose
#endif // clang-format on
}

asm void fn_802139A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSMTXRotRad
#endif // clang-format on
}

asm void fn_802139A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSMTXRotAxisRad
#endif // clang-format on
}

asm void fn_802139A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSVECMag
#endif // clang-format on
}

asm void fn_802139AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSVECScale
#endif // clang-format on
}

asm void fn_802139B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSMTXMultVecSR
#endif // clang-format on
}

asm void fn_802139B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b C_QUATRotAxisRad
#endif // clang-format on
}

asm void fn_802139B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b C_QUATMtx
#endif // clang-format on
}

asm void fn_802139BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b C_QUATAdd
#endif // clang-format on
}

asm void fn_802139C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b C_QUATSlerp
#endif // clang-format on
}

asm void fn_802139C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSQUATDotProduct
#endif // clang-format on
}

asm void fn_802139C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSQUATAdd
#endif // clang-format on
}

asm void fn_802139CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSMTXQuat
#endif // clang-format on
}

asm void fn_802139D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSQUATMultiply
#endif // clang-format on
}

asm void fn_802139D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSQUATInverse
#endif // clang-format on
}

asm void fn_802139D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSQUATNormalize
#endif // clang-format on
}

asm void fn_802139DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSQUATScale
#endif // clang-format on
}

asm void fn_802139E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSQUATSubtract
#endif // clang-format on
}

asm void fn_802139E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSTicksToCalendarTime
#endif // clang-format on
}

asm void fn_802139E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSMTXTransApply
#endif // clang-format on
}

asm void fn_802139EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSMTXScaleApply
#endif // clang-format on
}

asm void fn_802139F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSGetLanguage
#endif // clang-format on
}

asm void fn_802139F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSMTXMultVecArraySR
#endif // clang-format on
}

asm void fn_802139F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PSMTX44MultVec
#endif // clang-format on
}

asm void fn_802139FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SIGetTypeString
#endif // clang-format on
}

asm void fn_80213A00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSCheckActiveThreads
#endif // clang-format on
}

asm void fn_80213A04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSGetThreadPriority
#endif // clang-format on
}

asm void fn_80213A08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSInitSemaphore
#endif // clang-format on
}

asm void fn_80213A0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSIsThreadTerminated
#endif // clang-format on
}

asm void fn_80213A10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSSignalSemaphore
#endif // clang-format on
}

asm void fn_80213A14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b OSWaitSemaphore
#endif // clang-format on
}

asm void fn_80213A18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b log
#endif // clang-format on
}

asm void fn_80213A1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b exp
#endif // clang-format on
}

asm void fn_80213A20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXWaitDrawDone
#endif // clang-format on
}

asm void fn_80213A24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXProject
#endif // clang-format on
}

asm void fn_80213A28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXGetProjectionv
#endif // clang-format on
}

asm void fn_80213A2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXGetViewportv
#endif // clang-format on
}

asm void fn_80213A30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXPixModeSync
#endif // clang-format on
}

asm void fn_80213A34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXSetIndTexOrder
#endif // clang-format on
}

asm void fn_80213A38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXSetIndTexMtx
#endif // clang-format on
}

asm void fn_80213A3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXSetTevIndWarp
#endif // clang-format on
}

asm void fn_80213A40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXSetTexCoordScaleManually
#endif // clang-format on
}

asm void fn_80213A44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXSetTevIndTile
#endif // clang-format on
}

asm void fn_80213A48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXNtsc480Prog
#endif // clang-format on
}

asm void fn_80213A4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXNtsc480Int
#endif // clang-format on
}

asm void fn_80213A50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXNtsc240Int
#endif // clang-format on
}

asm void fn_80213A54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXSetVtxDescv
#endif // clang-format on
}

asm void fn_80213A58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXDrawSphere
#endif // clang-format on
}

asm void fn_80213A5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GXDrawCylinder
#endif // clang-format on
}

asm void fn_80213A60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omSetStatBit
#endif // clang-format on
}

asm void fn_80213A64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesKillCheck
#endif // clang-format on
}

asm void fn_80213A68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b WipeCheck
#endif // clang-format on
}

asm void fn_80213A6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b WipeTypeGet
#endif // clang-format on
}

asm void fn_80213A70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omMakeGroupEx
#endif // clang-format on
}

asm void fn_80213A74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omGetGroupMemberListEx
#endif // clang-format on
}

asm void fn_80213A78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesDispSet
#endif // clang-format on
}

asm void fn_80213A7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesKill
#endif // clang-format on
}

asm void fn_80213A80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b ClusterAdjustObject
#endif // clang-format on
}

asm void fn_80213A84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b printWin
#endif // clang-format on
}

asm void fn_80213A88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b print8
#endif // clang-format on
}

asm void fn_80213A8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSysVWaitSet
#endif // clang-format on
}

asm void fn_80213A90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesGet
#endif // clang-format on
}

asm void fn_80213A94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omObjManPause
#endif // clang-format on
}

asm void fn_80213A98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesInit
#endif // clang-format on
}

asm void fn_80213A9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesCreate
#endif // clang-format on
}

asm void fn_80213AA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omDelObjEx
#endif // clang-format on
}

asm void fn_80213AA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesPosSet
#endif // clang-format on
}

asm void fn_80213AA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b atan2
#endif // clang-format on
}

asm void fn_80213AAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b atan2f
#endif // clang-format on
}

asm void fn_80213AB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b cos
#endif // clang-format on
}

asm void fn_80213AB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesStatGet
#endif // clang-format on
}

asm void fn_80213AB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Center
#endif // clang-format on
}

asm void fn_80213ABC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CRotM
#endif // clang-format on
}

asm void fn_80213AC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CenterM
#endif // clang-format on
}

asm void fn_80213AC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CZoomM
#endif // clang-format on
}

asm void fn_80213AC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omOutViewMulti
#endif // clang-format on
}

asm void fn_80213ACC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omOutView
#endif // clang-format on
}

asm void fn_80213AD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omAddObjEx
#endif // clang-format on
}

asm void fn_80213AD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omInitObjMan
#endif // clang-format on
}

asm void fn_80213AD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b rand8
#endif // clang-format on
}

asm void fn_80213ADC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b frandmod
#endif // clang-format on
}

asm void fn_80213AE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b frandom
#endif // clang-format on
}

asm void fn_80213AE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SetDefLight
#endif // clang-format on
}

asm void fn_80213AE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSetVecF
#endif // clang-format on
}

asm void fn_80213AEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSubVecF
#endif // clang-format on
}

asm void fn_80213AF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b frandf
#endif // clang-format on
}

asm void fn_80213AF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesSprKill
#endif // clang-format on
}

asm void fn_80213AF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mtxRotCat
#endif // clang-format on
}

asm void fn_80213AFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreBoxInit
#endif // clang-format on
}

asm void fn_80213B00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreBoxCreate
#endif // clang-format on
}

asm void fn_80213B04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreBoxCreateChar
#endif // clang-format on
}

asm void fn_80213B08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreBoxCreateCharMulti
#endif // clang-format on
}

asm void fn_80213B0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreBoxColorSet
#endif // clang-format on
}

asm void fn_80213B10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreBoxTPLvlSet
#endif // clang-format on
}

asm void fn_80213B14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreBoxPosSet
#endif // clang-format on
}

asm void fn_80213B18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreBoxDispSet
#endif // clang-format on
}

asm void fn_80213B1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesStrWinCreate
#endif // clang-format on
}

asm void fn_80213B20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraLayerHookSet
#endif // clang-format on
}

asm void fn_80213B24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraLayerHookReset
#endif // clang-format on
}

asm void fn_80213B28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLayerHookSet
#endif // clang-format on
}

asm void fn_80213B2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLayerHookReset
#endif // clang-format on
}

asm void fn_80213B30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DNoSyncSet
#endif // clang-format on
}

asm void fn_80213B34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelCreate
#endif // clang-format on
}

asm void fn_80213B38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelLink
#endif // clang-format on
}

asm void fn_80213B3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DHookFuncCreate
#endif // clang-format on
}

asm void fn_80213B40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelPosSet
#endif // clang-format on
}

asm void fn_80213B44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelPosSetV
#endif // clang-format on
}

asm void fn_80213B48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelRotSet
#endif // clang-format on
}

asm void fn_80213B4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelRotSetV
#endif // clang-format on
}

asm void fn_80213B50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelScaleSet
#endif // clang-format on
}

asm void fn_80213B54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelScaleSetV
#endif // clang-format on
}

asm void fn_80213B58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelPosGet
#endif // clang-format on
}

asm void fn_80213B5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelRotGet
#endif // clang-format on
}

asm void fn_80213B60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelScaleGet
#endif // clang-format on
}

asm void fn_80213B64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelKill
#endif // clang-format on
}

asm void fn_80213B68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelAllKill
#endif // clang-format on
}

asm void fn_80213B6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelAttrSet
#endif // clang-format on
}

asm void fn_80213B70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelAttrReset
#endif // clang-format on
}

asm void fn_80213B74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelAttrGet
#endif // clang-format on
}

asm void fn_80213B78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelMotionAttrGet
#endif // clang-format on
}

asm void fn_80213B7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DBGColorSet
#endif // clang-format on
}

asm void fn_80213B80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelCameraSet
#endif // clang-format on
}

asm void fn_80213B84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelLayerSet
#endif // clang-format on
}

asm void fn_80213B88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelTPLvlSet
#endif // clang-format on
}

asm void fn_80213B8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelHookSet
#endif // clang-format on
}

asm void fn_80213B90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelHookReset
#endif // clang-format on
}

asm void fn_80213B94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelHookObjReset
#endif // clang-format on
}

asm void fn_80213B98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelAmbSet
#endif // clang-format on
}

asm void fn_80213B9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelShadowDispOn
#endif // clang-format on
}

asm void fn_80213BA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelShadowDispOff
#endif // clang-format on
}

asm void fn_80213BA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelProjectionSet
#endif // clang-format on
}

asm void fn_80213BA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelProjectionReset
#endif // clang-format on
}

asm void fn_80213BAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelObjPosGet
#endif // clang-format on
}

asm void fn_80213BB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelObjMtxGet
#endif // clang-format on
}

asm void fn_80213BB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelHiliteTypeSet
#endif // clang-format on
}

asm void fn_80213BB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelReflectTypeSet
#endif // clang-format on
}

asm void fn_80213BBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelHiliteMapSet
#endif // clang-format on
}

asm void fn_80213BC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DReflectModelSet
#endif // clang-format on
}

asm void fn_80213BC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelMatHookSet
#endif // clang-format on
}

asm void fn_80213BC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelDebug
#endif // clang-format on
}

asm void fn_80213BCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelMtxSet
#endif // clang-format on
}

asm void fn_80213BD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelMtxGet
#endif // clang-format on
}

asm void fn_80213BD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelShadowMapTPLvlSet
#endif // clang-format on
}

asm void fn_80213BD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelShadowMapObjTPLvlSet
#endif // clang-format on
}

asm void fn_80213BDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelDieCreate
#endif // clang-format on
}

asm void fn_80213BE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelLinkDieCreate
#endif // clang-format on
}

asm void fn_80213BE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPadInit
#endif // clang-format on
}

asm void fn_80213BE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPadRead
#endif // clang-format on
}

asm void fn_80213BEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPadRumbleSet
#endif // clang-format on
}

asm void fn_80213BF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPadRumbleStop
#endif // clang-format on
}

asm void fn_80213BF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPadRumbleAllStop
#endif // clang-format on
}

asm void fn_80213BF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPadStatGet
#endif // clang-format on
}

asm void fn_80213BFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPadRumbleGet
#endif // clang-format on
}

asm void fn_80213C00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPadStkDirGet
#endif // clang-format on
}

asm void fn_80213C04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPadSubStkDirGet
#endif // clang-format on
}

asm void fn_80213C08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraCreate
#endif // clang-format on
}

asm void fn_80213C0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraPerspectiveSet
#endif // clang-format on
}

asm void fn_80213C10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraViewportSet
#endif // clang-format on
}

asm void fn_80213C14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraScissorSet
#endif // clang-format on
}

asm void fn_80213C18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraPosSet
#endif // clang-format on
}

asm void fn_80213C1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraKill
#endif // clang-format on
}

asm void fn_80213C20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraPosGet
#endif // clang-format on
}

asm void fn_80213C24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraPosSetV
#endif // clang-format on
}

asm void fn_80213C28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraAllKill
#endif // clang-format on
}

asm void fn_80213C2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraSet
#endif // clang-format on
}

asm void fn_80213C30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraMotionOn
#endif // clang-format on
}

asm void fn_80213C34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraMotionStart
#endif // clang-format on
}

asm void fn_80213C38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraMotionOff
#endif // clang-format on
}

asm void fn_80213C3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelCameraCreate
#endif // clang-format on
}

asm void fn_80213C40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelCameraInfoSet
#endif // clang-format on
}

asm void fn_80213C44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DCameraPerspectiveGet
#endif // clang-format on
}

asm void fn_80213C48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLighInit
#endif // clang-format on
}

asm void fn_80213C4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightCreate
#endif // clang-format on
}

asm void fn_80213C50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightCreateV
#endif // clang-format on
}

asm void fn_80213C54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightKill
#endif // clang-format on
}

asm void fn_80213C58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLightAllKill
#endif // clang-format on
}

asm void fn_80213C5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLightSet
#endif // clang-format on
}

asm void fn_80213C60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightSpotSet
#endif // clang-format on
}

asm void fn_80213C64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightInfinitytSet
#endif // clang-format on
}

asm void fn_80213C68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightPointSet
#endif // clang-format on
}

asm void fn_80213C6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightColorSet
#endif // clang-format on
}

asm void fn_80213C70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightPosSetV
#endif // clang-format on
}

asm void fn_80213C74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightPosSet
#endif // clang-format on
}

asm void fn_80213C78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightPosAimSetV
#endif // clang-format on
}

asm void fn_80213C7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightPosAimSet
#endif // clang-format on
}

asm void fn_80213C80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightStaticSet
#endif // clang-format on
}

asm void fn_80213C84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightParamGet
#endif // clang-format on
}

asm void fn_80213C88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightParamGet
#endif // clang-format on
}

asm void fn_80213C8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DGLightPosAngleSet
#endif // clang-format on
}

asm void fn_80213C90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelLightBitSet
#endif // clang-format on
}

asm void fn_80213C94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelLightBitReset
#endif // clang-format on
}

asm void fn_80213C98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelLightInfoSet
#endif // clang-format on
}

asm void fn_80213C9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelLightIdGet
#endif // clang-format on
}

asm void fn_80213CA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DReflectMapSet
#endif // clang-format on
}

asm void fn_80213CA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DReflectNoSet
#endif // clang-format on
}

asm void fn_80213CA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DFogSet
#endif // clang-format on
}

asm void fn_80213CAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DFogClear
#endif // clang-format on
}

asm void fn_80213CB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelObjPtrGet
#endif // clang-format on
}

asm void fn_80213CB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelShadowSet
#endif // clang-format on
}

asm void fn_80213CB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelShadowReset
#endif // clang-format on
}

asm void fn_80213CBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShadowCreate
#endif // clang-format on
}

asm void fn_80213CC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShadowPosSet
#endif // clang-format on
}

asm void fn_80213CC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShadowTPLvlSet
#endif // clang-format on
}

asm void fn_80213CC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShadowColSet
#endif // clang-format on
}

asm void fn_80213CCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShadowMultiCreate
#endif // clang-format on
}

asm void fn_80213CD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShadowMultiPosSet
#endif // clang-format on
}

asm void fn_80213CD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShadowMultiSizeSet
#endif // clang-format on
}

asm void fn_80213CD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShadowMultiColSet
#endif // clang-format on
}

asm void fn_80213CDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShadowMultiTPLvlSet
#endif // clang-format on
}

asm void fn_80213CE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelShadowMapSet
#endif // clang-format on
}

asm void fn_80213CE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelShadowMapObjSet
#endif // clang-format on
}

asm void fn_80213CE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelShadowMapObjReset
#endif // clang-format on
}

asm void fn_80213CEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelShadowMapReset
#endif // clang-format on
}

asm void fn_80213CF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShadowSizeSet
#endif // clang-format on
}

asm void fn_80213CF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMipMapSet
#endif // clang-format on
}

asm void fn_80213CF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelClusterAttrSet
#endif // clang-format on
}

asm void fn_80213CFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelClusterAttrReset
#endif // clang-format on
}

asm void fn_80213D00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightCreate
#endif // clang-format on
}

asm void fn_80213D04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightCreateV
#endif // clang-format on
}

asm void fn_80213D08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightSpotSet
#endif // clang-format on
}

asm void fn_80213D0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightInfinitytSet
#endif // clang-format on
}

asm void fn_80213D10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightPointSet
#endif // clang-format on
}

asm void fn_80213D14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightKill
#endif // clang-format on
}

asm void fn_80213D18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightColorSet
#endif // clang-format on
}

asm void fn_80213D1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightPosSetV
#endif // clang-format on
}

asm void fn_80213D20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightPosSet
#endif // clang-format on
}

asm void fn_80213D24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightPosAimSetV
#endif // clang-format on
}

asm void fn_80213D28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightPosAimSet
#endif // clang-format on
}

asm void fn_80213D2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightStaticSet
#endif // clang-format on
}

asm void fn_80213D30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DLLightPosAngleSet
#endif // clang-format on
}

asm void fn_80213D34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DShineSet
#endif // clang-format on
}

asm void fn_80213D38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DObjHookSet
#endif // clang-format on
}

asm void fn_80213D3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimInit
#endif // clang-format on
}

asm void fn_80213D40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimCreate
#endif // clang-format on
}

asm void fn_80213D44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimAnimSet
#endif // clang-format on
}

asm void fn_80213D48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimLink
#endif // clang-format on
}

asm void fn_80213D4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimKill
#endif // clang-format on
}

asm void fn_80213D50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimAllKill
#endif // clang-format on
}

asm void fn_80213D54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimSet
#endif // clang-format on
}

asm void fn_80213D58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimExec
#endif // clang-format on
}

asm void fn_80213D5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimAttrSet
#endif // clang-format on
}

asm void fn_80213D60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimAttrReset
#endif // clang-format on
}

asm void fn_80213D64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimSpeedSet
#endif // clang-format on
}

asm void fn_80213D68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnimBankSet
#endif // clang-format on
}

asm void fn_80213D6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DAnmNoSet
#endif // clang-format on
}

asm void fn_80213D70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DTexScrollCreate
#endif // clang-format on
}

asm void fn_80213D74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DTexScrollPosSet
#endif // clang-format on
}

asm void fn_80213D78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DTexScrollPosMoveSet
#endif // clang-format on
}

asm void fn_80213D7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DTexScrollRotSet
#endif // clang-format on
}

asm void fn_80213D80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DTexScrollRotMoveSet
#endif // clang-format on
}

asm void fn_80213D84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DTexScrollKill
#endif // clang-format on
}

asm void fn_80213D88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DTexScrollAllKill
#endif // clang-format on
}

asm void fn_80213D8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DTexScrollPauseDisableSet
#endif // clang-format on
}

asm void fn_80213D90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleCreate
#endif // clang-format on
}

asm void fn_80213D94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleScaleSet
#endif // clang-format on
}

asm void fn_80213D98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleZRotSet
#endif // clang-format on
}

asm void fn_80213D9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleColSet
#endif // clang-format on
}

asm void fn_80213DA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleTPLvlSet
#endif // clang-format on
}

asm void fn_80213DA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleBlendModeSet
#endif // clang-format on
}

asm void fn_80213DA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleHookSet
#endif // clang-format on
}

asm void fn_80213DAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleCntSet
#endif // clang-format on
}

asm void fn_80213DB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleAttrSet
#endif // clang-format on
}

asm void fn_80213DB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleAttrReset
#endif // clang-format on
}

asm void fn_80213DB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParticleAnimModeSet
#endif // clang-format on
}

asm void fn_80213DBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManInit
#endif // clang-format on
}

asm void fn_80213DC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManCreate
#endif // clang-format on
}

asm void fn_80213DC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManKill
#endif // clang-format on
}

asm void fn_80213DC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManAllKill
#endif // clang-format on
}

asm void fn_80213DCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManPtrGet
#endif // clang-format on
}

asm void fn_80213DD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManAttrSet
#endif // clang-format on
}

asm void fn_80213DD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManAttrReset
#endif // clang-format on
}

asm void fn_80213DD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManPosSet
#endif // clang-format on
}

asm void fn_80213DDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManVecSet
#endif // clang-format on
}

asm void fn_80213DE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManRotSet
#endif // clang-format on
}

asm void fn_80213DE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManModelIDGet
#endif // clang-format on
}

asm void fn_80213DE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManLink
#endif // clang-format on
}

asm void fn_80213DEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManTimeLimitSet
#endif // clang-format on
}

asm void fn_80213DF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManColorSet
#endif // clang-format on
}

asm void fn_80213DF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManLayerSet
#endif // clang-format on
}

asm void fn_80213DF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DWaterCreate
#endif // clang-format on
}

asm void fn_80213DFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DWaterTexPosSet
#endif // clang-format on
}

asm void fn_80213E00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DWaterTexScaleSet
#endif // clang-format on
}

asm void fn_80213E04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DWaterPadYSet
#endif // clang-format on
}

asm void fn_80213E08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DWaterGlowSet
#endif // clang-format on
}

asm void fn_80213E0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DWaterIndTexMtxSet
#endif // clang-format on
}

asm void fn_80213E10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DWaterHiliteSet
#endif // clang-format on
}

asm void fn_80213E14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DWaterHiliteTPLvlSet
#endif // clang-format on
}

asm void fn_80213E18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DWaterCameraSet
#endif // clang-format on
}

asm void fn_80213E1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DWaterWaveCreate
#endif // clang-format on
}

asm void fn_80213E20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionCreate
#endif // clang-format on
}

asm void fn_80213E24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionInit
#endif // clang-format on
}

asm void fn_80213E28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionModelCreate
#endif // clang-format on
}

asm void fn_80213E2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionKill
#endif // clang-format on
}

asm void fn_80213E30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionAllKill
#endif // clang-format on
}

asm void fn_80213E34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionSet
#endif // clang-format on
}

asm void fn_80213E38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionExec
#endif // clang-format on
}

asm void fn_80213E3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DSubMotionExec
#endif // clang-format on
}

asm void fn_80213E40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionNext
#endif // clang-format on
}

asm void fn_80213E44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionTimeSet
#endif // clang-format on
}

asm void fn_80213E48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionTimeGet
#endif // clang-format on
}

asm void fn_80213E4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionMaxTimeGet
#endif // clang-format on
}

asm void fn_80213E50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionStartEndSet
#endif // clang-format on
}

asm void fn_80213E54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionMotionMaxTimeGet
#endif // clang-format on
}

asm void fn_80213E58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionEndCheck
#endif // clang-format on
}

asm void fn_80213E5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShiftSet
#endif // clang-format on
}

asm void fn_80213E60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionIDGet
#endif // clang-format on
}

asm void fn_80213E64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShiftIDGet
#endif // clang-format on
}

asm void fn_80213E68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShiftTimeGet
#endif // clang-format on
}

asm void fn_80213E6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShiftMaxTimeGet
#endif // clang-format on
}

asm void fn_80213E70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShiftStartEndSet
#endif // clang-format on
}

asm void fn_80213E74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShiftSpeedSet
#endif // clang-format on
}

asm void fn_80213E78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionSpeedSet
#endif // clang-format on
}

asm void fn_80213E7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShapeSpeedSet
#endif // clang-format on
}

asm void fn_80213E80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShapeTimeSet
#endif // clang-format on
}

asm void fn_80213E84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShapeTimeGet
#endif // clang-format on
}

asm void fn_80213E88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionNoMotSet
#endif // clang-format on
}

asm void fn_80213E8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionNoMotReset
#endif // clang-format on
}

asm void fn_80213E90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionForceSet
#endif // clang-format on
}

asm void fn_80213E94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShapeSet
#endif // clang-format on
}

asm void fn_80213E98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShapeIDGet
#endif // clang-format on
}

asm void fn_80213E9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShapeMaxTimeGet
#endif // clang-format on
}

asm void fn_80213EA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionClusterSet
#endif // clang-format on
}

asm void fn_80213EA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionClusterNoSet
#endif // clang-format on
}

asm void fn_80213EA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShapeReset
#endif // clang-format on
}

asm void fn_80213EAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionClusterReset
#endif // clang-format on
}

asm void fn_80213EB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DJointMotion
#endif // clang-format on
}

asm void fn_80213EB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionOverlaySet
#endif // clang-format on
}

asm void fn_80213EB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionOverlayReset
#endif // clang-format on
}

asm void fn_80213EBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionOverlayTimeGet
#endif // clang-format on
}

asm void fn_80213EC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionOverlayTimeSet
#endif // clang-format on
}

asm void fn_80213EC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionOverlaySpeedSet
#endif // clang-format on
}

asm void fn_80213EC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionOverlayMaxTimeGet
#endif // clang-format on
}

asm void fn_80213ECC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionOverlayEndCheck
#endif // clang-format on
}

asm void fn_80213ED0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelObjDrawInit
#endif // clang-format on
}

asm void fn_80213ED4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelObjDraw
#endif // clang-format on
}

asm void fn_80213ED8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DModelObjPtrDraw
#endif // clang-format on
}

asm void fn_80213EDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionCalc
#endif // clang-format on
}

asm void fn_80213EE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShapeStartEndSet
#endif // clang-format on
}

asm void fn_80213EE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DSubMotionSet
#endif // clang-format on
}

asm void fn_80213EE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DSubMotionReset
#endif // clang-format on
}

asm void fn_80213EEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DSubMotionTimeSet
#endif // clang-format on
}

asm void fn_80213EF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionShiftTimeSet
#endif // clang-format on
}

asm void fn_80213EF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionClusterSpeedSet
#endif // clang-format on
}

asm void fn_80213EF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionClusterTimeSet
#endif // clang-format on
}

asm void fn_80213EFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionClusterMaxTimeGet
#endif // clang-format on
}

asm void fn_80213F00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionAttrSet
#endif // clang-format on
}

asm void fn_80213F04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionAttrReset
#endif // clang-format on
}

asm void fn_80213F08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionTimingHookSet
#endif // clang-format on
}

asm void fn_80213F0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMotionTimingHookReset
#endif // clang-format on
}

asm void fn_80213F10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMatLightSet
#endif // clang-format on
}

asm void fn_80213F14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DFbCopyLayerSet
#endif // clang-format on
}

asm void fn_80213F18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DFbCopyExec
#endif // clang-format on
}

asm void fn_80213F1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DZClear
#endif // clang-format on
}

asm void fn_80213F20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DZClearLayerSet
#endif // clang-format on
}

asm void fn_80213F24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CamMotionEx
#endif // clang-format on
}

asm void fn_80213F28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CamMotionEx2
#endif // clang-format on
}

asm void fn_80213F2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CamMotionExPathGet
#endif // clang-format on
}

asm void fn_80213F30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CamMotionExPath
#endif // clang-format on
}

asm void fn_80213F34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3D2Dto3D
#endif // clang-format on
}

asm void fn_80213F38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3D3Dto2D
#endif // clang-format on
}

asm void fn_80213F3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMtxRotGet
#endif // clang-format on
}

asm void fn_80213F40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMtxTransGet
#endif // clang-format on
}

asm void fn_80213F44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DMtxScaleGet
#endif // clang-format on
}

asm void fn_80213F48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DProjectionCreate
#endif // clang-format on
}

asm void fn_80213F4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DProjectionPosSet
#endif // clang-format on
}

asm void fn_80213F50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DProjectionTPLvlSet
#endif // clang-format on
}

asm void fn_80213F54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DProjectionKill
#endif // clang-format on
}

asm void fn_80213F58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudInit
#endif // clang-format on
}

asm void fn_80213F5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudStreamPlay
#endif // clang-format on
}

asm void fn_80213F60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudAllStop
#endif // clang-format on
}

asm void fn_80213F64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFadeOut
#endif // clang-format on
}

asm void fn_80213F68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudStreamPauseOn
#endif // clang-format on
}

asm void fn_80213F6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudStreamPauseOff
#endif // clang-format on
}

asm void fn_80213F70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudStreamFadeOut
#endif // clang-format on
}

asm void fn_80213F74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudStreamVolSet
#endif // clang-format on
}

asm void fn_80213F78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXPlay
#endif // clang-format on
}

asm void fn_80213F7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXPlayVol
#endif // clang-format on
}

asm void fn_80213F80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXPlayVolPan
#endif // clang-format on
}

asm void fn_80213F84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXPlayPan
#endif // clang-format on
}

asm void fn_80213F88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXPlayDelay
#endif // clang-format on
}

asm void fn_80213F8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXPlayDelayVol
#endif // clang-format on
}

asm void fn_80213F90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXPlayDelayPan
#endif // clang-format on
}

asm void fn_80213F94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXPlayDelayVolPan
#endif // clang-format on
}

asm void fn_80213F98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXStop
#endif // clang-format on
}

asm void fn_80213F9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXFadeOut
#endif // clang-format on
}

asm void fn_80213FA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXPanning
#endif // clang-format on
}

asm void fn_80213FA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSeqPlay
#endif // clang-format on
}

asm void fn_80213FA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSeqStop
#endif // clang-format on
}

asm void fn_80213FAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSeqFadeOut
#endif // clang-format on
}

asm void fn_80213FB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSeqMidiCtrlGet
#endif // clang-format on
}

asm void fn_80213FB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXListnerSet
#endif // clang-format on
}

asm void fn_80213FB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXListnerUpdate
#endif // clang-format on
}

asm void fn_80213FBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXEmiterPlay
#endif // clang-format on
}

asm void fn_80213FC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXEmiterUpDate
#endif // clang-format on
}

asm void fn_80213FC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXListnerKill
#endif // clang-format on
}

asm void fn_80213FC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXStatusGet
#endif // clang-format on
}

asm void fn_80213FCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXPitchSet
#endif // clang-format on
}

asm void fn_80213FD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXVolSet
#endif // clang-format on
}

asm void fn_80213FD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudDllSndGrpSet
#endif // clang-format on
}

asm void fn_80213FD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSndGrpSetSet
#endif // clang-format on
}

asm void fn_80213FDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSndGrpSet
#endif // clang-format on
}

asm void fn_80213FE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSndCommonGrpSet
#endif // clang-format on
}

asm void fn_80213FE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSStreamPlay
#endif // clang-format on
}

asm void fn_80213FE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSStreamStop
#endif // clang-format on
}

asm void fn_80213FEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSStreamFadeOut
#endif // clang-format on
}

asm void fn_80213FF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSStreamAllFadeOut
#endif // clang-format on
}

asm void fn_80213FF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSStreamAllStop
#endif // clang-format on
}

asm void fn_80213FF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSStreamStatGet
#endif // clang-format on
}

asm void fn_80213FFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSStreamPause
#endif // clang-format on
}

asm void fn_80214000(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSStreamPauseFadeOut
#endif // clang-format on
}

asm void fn_80214004(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSStreamParamSet
#endif // clang-format on
}

asm void fn_80214008(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PlayerFXPlay
#endif // clang-format on
}

asm void fn_8021400C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharFXPlay
#endif // clang-format on
}

asm void fn_80214010(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharFXPlayVol
#endif // clang-format on
}

asm void fn_80214014(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharFXPlayPan
#endif // clang-format on
}

asm void fn_80214018(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharFXPlayVolPan
#endif // clang-format on
}

asm void fn_8021401C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharFXPlayDelayVolPan
#endif // clang-format on
}

asm void fn_80214020(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharFXPlayDelay
#endif // clang-format on
}

asm void fn_80214024(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharFXPlayDelayVol
#endif // clang-format on
}

asm void fn_80214028(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharFXPlayDelayPan
#endif // clang-format on
}

asm void fn_8021402C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PlayerFXPlayPos
#endif // clang-format on
}

asm void fn_80214030(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharFXPlayPos
#endif // clang-format on
}

asm void fn_80214034(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b PlayerFXStop
#endif // clang-format on
}

asm void fn_80214038(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharFXStop
#endif // clang-format on
}

asm void fn_8021403C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudAUXSet
#endif // clang-format on
}

asm void fn_80214040(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudSStreamChanPlay
#endif // clang-format on
}

asm void fn_80214044(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudBGMPlay
#endif // clang-format on
}

asm void fn_80214048(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudJinglePlay
#endif // clang-format on
}

asm void fn_8021404C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b msmMusPlay
#endif // clang-format on
}

asm void fn_80214050(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espInit
#endif // clang-format on
}

asm void fn_80214054(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espEntry
#endif // clang-format on
}

asm void fn_80214058(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espKill
#endif // clang-format on
}

asm void fn_8021405C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espGrpIDGet
#endif // clang-format on
}

asm void fn_80214060(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espDispOn
#endif // clang-format on
}

asm void fn_80214064(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espDispOff
#endif // clang-format on
}

asm void fn_80214068(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espAttrSet
#endif // clang-format on
}

asm void fn_8021406C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espAttrReset
#endif // clang-format on
}

asm void fn_80214070(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espPosSet
#endif // clang-format on
}

asm void fn_80214074(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espScaleSet
#endif // clang-format on
}

asm void fn_80214078(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espZRotSet
#endif // clang-format on
}

asm void fn_8021407C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espTPLvlSet
#endif // clang-format on
}

asm void fn_80214080(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espColorSet
#endif // clang-format on
}

asm void fn_80214084(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espSpeedSet
#endif // clang-format on
}

asm void fn_80214088(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espBankSet
#endif // clang-format on
}

asm void fn_8021408C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espDrawNoSet
#endif // clang-format on
}

asm void fn_80214090(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b espPriSet
#endif // clang-format on
}

asm void fn_80214094(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcInit
#endif // clang-format on
}

asm void fn_80214098(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcCall
#endif // clang-format on
}

asm void fn_8021409C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcCreate
#endif // clang-format on
}

asm void fn_802140A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcKill
#endif // clang-format on
}

asm void fn_802140A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcEnd
#endif // clang-format on
}

asm void fn_802140A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcWakeup
#endif // clang-format on
}

asm void fn_802140AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcSleep
#endif // clang-format on
}

asm void fn_802140B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcVSleep
#endif // clang-format on
}

asm void fn_802140B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcCurrentGet
#endif // clang-format on
}

asm void fn_802140B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcDestructorSet
#endif // clang-format on
}

asm void fn_802140BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcDestructorSet2
#endif // clang-format on
}

asm void fn_802140C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcChildCreate
#endif // clang-format on
}

asm void fn_802140C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcChildLink
#endif // clang-format on
}

asm void fn_802140C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcChildUnlink
#endif // clang-format on
}

asm void fn_802140CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcChildKill
#endif // clang-format on
}

asm void fn_802140D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcMemAlloc
#endif // clang-format on
}

asm void fn_802140D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcMemFree
#endif // clang-format on
}

asm void fn_802140D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b Hu3DParManVacumeSet
#endif // clang-format on
}

asm void fn_802140DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataInit
#endif // clang-format on
}

asm void fn_802140E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataDirRead
#endif // clang-format on
}

asm void fn_802140E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataRead
#endif // clang-format on
}

asm void fn_802140E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataReadNum
#endif // clang-format on
}

asm void fn_802140EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataReadMulti
#endif // clang-format on
}

asm void fn_802140F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataDirReadAsync
#endif // clang-format on
}

asm void fn_802140F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataDirReadNumAsync
#endif // clang-format on
}

asm void fn_802140F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataSelHeapRead
#endif // clang-format on
}

asm void fn_802140FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataSelHeapReadNum
#endif // clang-format on
}

asm void fn_80214100(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataGetSize
#endif // clang-format on
}

asm void fn_80214104(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataDirClose
#endif // clang-format on
}

asm void fn_80214108(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataDirCloseNum
#endif // clang-format on
}

asm void fn_8021410C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataDirCloseAll
#endif // clang-format on
}

asm void fn_80214110(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataClose
#endif // clang-format on
}

asm void fn_80214114(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataCloseMulti
#endif // clang-format on
}

asm void fn_80214118(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataReadNumHeapShortForce
#endif // clang-format on
}

asm void fn_8021411C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataDirCancel
#endif // clang-format on
}

asm void fn_80214120(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b ARRegisterDMACallback
#endif // clang-format on
}

asm void fn_80214124(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b ARGetDMAStatus
#endif // clang-format on
}

asm void fn_80214128(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b ARStartDMA
#endif // clang-format on
}

asm void fn_8021412C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b ARInit
#endif // clang-format on
}

asm void fn_80214130(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b ARAlloc
#endif // clang-format on
}

asm void fn_80214134(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b ARFree
#endif // clang-format on
}

asm void fn_80214138(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b ARGetSize
#endif // clang-format on
}

asm void fn_8021413C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b ARSetSize
#endif // clang-format on
}

asm void fn_80214140(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprBegin
#endif // clang-format on
}

asm void fn_80214144(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprCall
#endif // clang-format on
}

asm void fn_80214148(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprFinish
#endif // clang-format on
}

asm void fn_8021414C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprAnimRead
#endif // clang-format on
}

asm void fn_80214150(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprCreate
#endif // clang-format on
}

asm void fn_80214154(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpCreate
#endif // clang-format on
}

asm void fn_80214158(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpMemberSet
#endif // clang-format on
}

asm void fn_8021415C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpMemberKill
#endif // clang-format on
}

asm void fn_80214160(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpKill
#endif // clang-format on
}

asm void fn_80214164(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprKill
#endif // clang-format on
}

asm void fn_80214168(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprAnimKill
#endif // clang-format on
}

asm void fn_8021416C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprAttrSet
#endif // clang-format on
}

asm void fn_80214170(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprAttrReset
#endif // clang-format on
}

asm void fn_80214174(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprAttrGet
#endif // clang-format on
}

asm void fn_80214178(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprPosSet
#endif // clang-format on
}

asm void fn_8021417C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprScaleSet
#endif // clang-format on
}

asm void fn_80214180(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprZRotSet
#endif // clang-format on
}

asm void fn_80214184(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpPosSet
#endif // clang-format on
}

asm void fn_80214188(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpZRotSet
#endif // clang-format on
}

asm void fn_8021418C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpScaleSet
#endif // clang-format on
}

asm void fn_80214190(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprTPLvlSet
#endif // clang-format on
}

asm void fn_80214194(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprColorSet
#endif // clang-format on
}

asm void fn_80214198(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprSpeedSet
#endif // clang-format on
}

asm void fn_8021419C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprBankSet
#endif // clang-format on
}

asm void fn_802141A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpDrawNoSet
#endif // clang-format on
}

asm void fn_802141A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprDrawNoSet
#endif // clang-format on
}

asm void fn_802141A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprPriSet
#endif // clang-format on
}

asm void fn_802141AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprFuncCreate
#endif // clang-format on
}

asm void fn_802141B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprAnimMake
#endif // clang-format on
}

asm void fn_802141B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpCenterSet
#endif // clang-format on
}

asm void fn_802141B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprAnimLock
#endif // clang-format on
}

asm void fn_802141BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b AnimDebug
#endif // clang-format on
}

asm void fn_802141C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprExecLayerSet
#endif // clang-format on
}

asm void fn_802141C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprSprBGSet
#endif // clang-format on
}

asm void fn_802141C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprBGSet
#endif // clang-format on
}

asm void fn_802141CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpScissorSet
#endif // clang-format on
}

asm void fn_802141D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprScissorSet
#endif // clang-format on
}

asm void fn_802141D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprGrpTPLvlSet
#endif // clang-format on
}

asm void fn_802141D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprExecLayerCameraSet
#endif // clang-format on
}

asm void fn_802141DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprAnimNoSet
#endif // clang-format on
}

asm void fn_802141E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprAnimNoSetPause
#endif // clang-format on
}

asm void fn_802141E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSpr3DHookSet
#endif // clang-format on
}

asm void fn_802141E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSpr3DDataCreate
#endif // clang-format on
}

asm void fn_802141EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSpr3DSet
#endif // clang-format on
}

asm void fn_802141F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSpr3DRotSet
#endif // clang-format on
}

asm void fn_802141F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSpr3DFovSet
#endif // clang-format on
}

asm void fn_802141F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprVtxColorSet
#endif // clang-format on
}

asm void fn_802141FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuSprVtxColorReset
#endif // clang-format on
}

asm void fn_80214200(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMemMemoryNumSet
#endif // clang-format on
}

asm void fn_80214204(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuARMalloc
#endif // clang-format on
}

asm void fn_80214208(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuARFree
#endif // clang-format on
}

asm void fn_8021420C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAR_DVDtoARAM
#endif // clang-format on
}

asm void fn_80214210(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAR_MRAMtoARAM
#endif // clang-format on
}

asm void fn_80214214(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAR_MRAMtoARAM2
#endif // clang-format on
}

asm void fn_80214218(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAR_ARAMtoMRAM
#endif // clang-format on
}

asm void fn_8021421C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAR_ARAMtoMRAMNum
#endif // clang-format on
}

asm void fn_80214220(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuARDMACheck
#endif // clang-format on
}

asm void fn_80214224(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuARDirCheck
#endif // clang-format on
}

asm void fn_80214228(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuARDirFree
#endif // clang-format on
}

asm void fn_8021422C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAR_ARAMtoMRAMFileRead
#endif // clang-format on
}

asm void fn_80214230(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuAudFXListnerSetEX
#endif // clang-format on
}

asm void fn_80214234(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWindowInit
#endif // clang-format on
}

asm void fn_80214238(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinInit
#endif // clang-format on
}

asm void fn_8021423C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinCreate
#endif // clang-format on
}

asm void fn_80214240(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinKill
#endif // clang-format on
}

asm void fn_80214244(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinAllKill
#endif // clang-format on
}

asm void fn_80214248(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesSet
#endif // clang-format on
}

asm void fn_8021424C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinInsertMesSet
#endif // clang-format on
}

asm void fn_80214250(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinChoiceGet
#endif // clang-format on
}

asm void fn_80214254(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinChoiceSet
#endif // clang-format on
}

asm void fn_80214258(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinChoiceNumGet
#endif // clang-format on
}

asm void fn_8021425C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesWait
#endif // clang-format on
}

asm void fn_80214260(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinSprKill
#endif // clang-format on
}

asm void fn_80214264(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesRead
#endif // clang-format on
}

asm void fn_80214268(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinAnimSet
#endif // clang-format on
}

asm void fn_8021426C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinSprSet
#endif // clang-format on
}

asm void fn_80214270(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinSprPosSet
#endif // clang-format on
}

asm void fn_80214274(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinSprIDGet
#endif // clang-format on
}

asm void fn_80214278(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesMaxSizeGet
#endif // clang-format on
}

asm void fn_8021427C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinInsertMesSizeGet
#endif // clang-format on
}

asm void fn_80214280(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesMaxSizeBetGet
#endif // clang-format on
}

asm void fn_80214284(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinExCreate
#endif // clang-format on
}

asm void fn_80214288(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinDispOn
#endif // clang-format on
}

asm void fn_8021428C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinDispOff
#endif // clang-format on
}

asm void fn_80214290(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinExOpen
#endif // clang-format on
}

asm void fn_80214294(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinExClose
#endif // clang-format on
}

asm void fn_80214298(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinExKill
#endif // clang-format on
}

asm void fn_8021429C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinExSpeakerSet
#endif // clang-format on
}

asm void fn_802142A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinComKeyWait
#endif // clang-format on
}

asm void fn_802142A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinComKeySet
#endif // clang-format on
}

asm void fn_802142A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinComKeyReset
#endif // clang-format on
}

asm void fn_802142AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinDrawNoSet
#endif // clang-format on
}

asm void fn_802142B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinAttrSet
#endif // clang-format on
}

asm void fn_802142B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinAttrReset
#endif // clang-format on
}

asm void fn_802142B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinBGTPLvlSet
#endif // clang-format on
}

asm void fn_802142BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesSpeedSet
#endif // clang-format on
}

asm void fn_802142C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinBGColSet
#endif // clang-format on
}

asm void fn_802142C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinChoiceDisable
#endif // clang-format on
}

asm void fn_802142C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinKeyWaitEntry
#endif // clang-format on
}

asm void fn_802142CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinCenterPosSet
#endif // clang-format on
}

asm void fn_802142D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinPosSet
#endif // clang-format on
}

asm void fn_802142D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinZRotSet
#endif // clang-format on
}

asm void fn_802142D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinScaleSet
#endif // clang-format on
}

asm void fn_802142DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinChoiceNowGet
#endif // clang-format on
}

asm void fn_802142E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinStatGet
#endif // clang-format on
}

asm void fn_802142E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesColSet
#endif // clang-format on
}

asm void fn_802142E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinScissorSet
#endif // clang-format on
}

asm void fn_802142EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinExCreateFrame
#endif // clang-format on
}

asm void fn_802142F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinPriSet
#endif // clang-format on
}

asm void fn_802142F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinSprPriSet
#endif // clang-format on
}

asm void fn_802142F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinKeyWaitNumGet
#endif // clang-format on
}

asm void fn_802142FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesSizeCancelCRSet
#endif // clang-format on
}

asm void fn_80214300(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesPalSet
#endif // clang-format on
}

asm void fn_80214304(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinHomeClear
#endif // clang-format on
}

asm void fn_80214308(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinPushKeySet
#endif // clang-format on
}

asm void fn_8021430C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinDisablePlayerSet
#endif // clang-format on
}

asm void fn_80214310(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinDisablePlayerReset
#endif // clang-format on
}

asm void fn_80214314(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesPtrGet
#endif // clang-format on
}

asm void fn_80214318(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesMaxDirGet
#endif // clang-format on
}

asm void fn_8021431C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesMaxNumGet
#endif // clang-format on
}

asm void fn_80214320(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinMesLanguageSet
#endif // clang-format on
}

asm void fn_80214324(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinWarningCreate
#endif // clang-format on
}

asm void fn_80214328(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinWarningKill
#endif // clang-format on
}

asm void fn_8021432C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinWarningOpen
#endif // clang-format on
}

asm void fn_80214330(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinWarningClose
#endif // clang-format on
}

asm void fn_80214334(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinFrameSet
#endif // clang-format on
}

asm void fn_80214338(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinCallbackStub
#endif // clang-format on
}

asm void fn_8021433C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinCallbackSet
#endif // clang-format on
}

asm void fn_80214340(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuWinPadMaskSet
#endif // clang-format on
}

asm void fn_80214344(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharInit
#endif // clang-format on
}

asm void fn_80214348(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionInit
#endif // clang-format on
}

asm void fn_8021434C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionInitAsync
#endif // clang-format on
}

asm void fn_80214350(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharDataClose
#endif // clang-format on
}

asm void fn_80214354(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelKill
#endif // clang-format on
}

asm void fn_80214358(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharClose
#endif // clang-format on
}

asm void fn_8021435C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelCreate
#endif // clang-format on
}

asm void fn_80214360(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionCreate
#endif // clang-format on
}

asm void fn_80214364(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionNoSet
#endif // clang-format on
}

asm void fn_80214368(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionKill
#endif // clang-format on
}

asm void fn_8021436C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionDataClose
#endif // clang-format on
}

asm void fn_80214370(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelDataClose
#endif // clang-format on
}

asm void fn_80214374(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionAMemPGet
#endif // clang-format on
}

asm void fn_80214378(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionSet
#endif // clang-format on
}

asm void fn_8021437C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionTimeSet
#endif // clang-format on
}

asm void fn_80214380(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionTimeGet
#endif // clang-format on
}

asm void fn_80214384(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionMaxTimeGet
#endif // clang-format on
}

asm void fn_80214388(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionEndCheck
#endif // clang-format on
}

asm void fn_8021438C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionShiftIDGet
#endif // clang-format on
}

asm void fn_80214390(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionShiftSet
#endif // clang-format on
}

asm void fn_80214394(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionShiftTimeGet
#endif // clang-format on
}

asm void fn_80214398(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionSpeedSet
#endif // clang-format on
}

asm void fn_8021439C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelHookDustCreate
#endif // clang-format on
}

asm void fn_802143A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharEffectSmokeCreate
#endif // clang-format on
}

asm void fn_802143A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharEffectSmokeCreateScale
#endif // clang-format on
}

asm void fn_802143A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharEffectCoinGlowCreate
#endif // clang-format on
}

asm void fn_802143AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelHitCreate
#endif // clang-format on
}

asm void fn_802143B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharEffectHitCreate
#endif // clang-format on
}

asm void fn_802143B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharEffectShoeHitCreate
#endif // clang-format on
}

asm void fn_802143B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelShoeHitCreate
#endif // clang-format on
}

asm void fn_802143BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWCharColorGet
#endif // clang-format on
}

asm void fn_802143C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharEffectLayerSet
#endif // clang-format on
}

asm void fn_802143C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharEyeBmpNameTbl
#endif // clang-format on
}

asm void fn_802143C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelEyeBmpGet
#endif // clang-format on
}

asm void fn_802143CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionVoiceOnSet
#endif // clang-format on
}

asm void fn_802143D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharNpcDustVoiceOffSet
#endif // clang-format on
}

asm void fn_802143D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelStepSet
#endif // clang-format on
}

asm void fn_802143D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelVoicePanAutoSet
#endif // clang-format on
}

asm void fn_802143DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelVoiceFlagSet
#endif // clang-format on
}

asm void fn_802143E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelLandDustCreate
#endif // clang-format on
}

asm void fn_802143E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelAttrSet
#endif // clang-format on
}

asm void fn_802143E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelAttrReset
#endif // clang-format on
}

asm void fn_802143EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionStartEndSet
#endif // clang-format on
}

asm void fn_802143F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelHeightGet
#endif // clang-format on
}

asm void fn_802143F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelMotListCreate
#endif // clang-format on
}

asm void fn_802143F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharEffectDustCreate
#endif // clang-format on
}

asm void fn_802143FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharEffectCryCreate
#endif // clang-format on
}

asm void fn_80214400(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelCryCreate
#endif // clang-format on
}

asm void fn_80214404(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelTimingHookNoGet
#endif // clang-format on
}

asm void fn_80214408(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionUpdateSet
#endif // clang-format on
}

asm void fn_8021440C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelVoicePanSet
#endif // clang-format on
}

asm void fn_80214410(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelVoiceVolSet
#endif // clang-format on
}

asm void fn_80214414(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharWinLoseVoicePlay
#endif // clang-format on
}

asm void fn_80214418(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharLoseVoicePlay
#endif // clang-format on
}

asm void fn_8021441C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionTotalTimeGet
#endif // clang-format on
}

asm void fn_80214420(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionExtraTimeGet
#endif // clang-format on
}

asm void fn_80214424(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharLightCreate
#endif // clang-format on
}

asm void fn_80214428(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharLightCreateV
#endif // clang-format on
}

asm void fn_8021442C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharLightSpotSet
#endif // clang-format on
}

asm void fn_80214430(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharLightInfinitytSet
#endif // clang-format on
}

asm void fn_80214434(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharLightPointSet
#endif // clang-format on
}

asm void fn_80214438(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharLightColorSet
#endif // clang-format on
}

asm void fn_8021443C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharLightPosAimSetV
#endif // clang-format on
}

asm void fn_80214440(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharLightPosAimSet
#endif // clang-format on
}

asm void fn_80214444(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharLightStaticSet
#endif // clang-format on
}

asm void fn_80214448(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharEffectWarnCreate
#endif // clang-format on
}

asm void fn_8021444C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionDefrag
#endif // clang-format on
}

asm void fn_80214450(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharNpcDustSet
#endif // clang-format on
}

asm void fn_80214454(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcSetStat
#endif // clang-format on
}

asm void fn_80214458(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuPrcResetStat
#endif // clang-format on
}

asm void fn_8021445C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omGameSysInit
#endif // clang-format on
}

asm void fn_80214460(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omResetStatBit
#endif // clang-format on
}

asm void fn_80214464(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omCurrentOvlGet
#endif // clang-format on
}

asm void fn_80214468(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omPauseChk
#endif // clang-format on
}

asm void fn_8021446C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omOvlHisChg
#endif // clang-format on
}

asm void fn_80214470(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omOvlHisGet
#endif // clang-format on
}

asm void fn_80214474(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omovlevtno
#endif // clang-format on
}

asm void fn_80214478(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omovlstat
#endif // clang-format on
}

asm void fn_8021447C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b AddX
#endif // clang-format on
}

asm void fn_80214480(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b AddZ
#endif // clang-format on
}

asm void fn_80214484(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MapObject
#endif // clang-format on
}

asm void fn_80214488(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharObject
#endif // clang-format on
}

asm void fn_8021448C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b nMap
#endif // clang-format on
}

asm void fn_80214490(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b nChar
#endif // clang-format on
}

asm void fn_80214494(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MapWall
#endif // clang-format on
}

asm void fn_80214498(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MapPos
#endif // clang-format on
}

asm void fn_8021449C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HitFaceVec
#endif // clang-format on
}

asm void fn_802144A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HitFace
#endif // clang-format on
}

asm void fn_802144A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HitFaceCount
#endif // clang-format on
}

asm void fn_802144A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GwPlayer
#endif // clang-format on
}

asm void fn_802144AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GwSystem
#endif // clang-format on
}

asm void fn_802144B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSinglePrizeFlagSet
#endif // clang-format on
}

asm void fn_802144B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSinglePrizeFlagGet
#endif // clang-format on
}

asm void fn_802144B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSingleDataInit
#endif // clang-format on
}

asm void fn_802144BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSinglePrizeSaveFlagSet
#endif // clang-format on
}

asm void fn_802144C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSinglePrizeSaveFlagGet
#endif // clang-format on
}

asm void fn_802144C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBankStarAdd
#endif // clang-format on
}

asm void fn_802144C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBankStarSub
#endif // clang-format on
}

asm void fn_802144CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBankStarGet
#endif // clang-format on
}

asm void fn_802144D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBankStarAwardGet
#endif // clang-format on
}

asm void fn_802144D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBankStarAwardReset
#endif // clang-format on
}

asm void fn_802144D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBankFlagSet
#endif // clang-format on
}

asm void fn_802144DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBankFlagReset
#endif // clang-format on
}

asm void fn_802144E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBankFlagGet
#endif // clang-format on
}

asm void fn_802144E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSingleMgFlagGet
#endif // clang-format on
}

asm void fn_802144E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSingleMgWinNumGet
#endif // clang-format on
}

asm void fn_802144EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSingleMgWinNumSet
#endif // clang-format on
}

asm void fn_802144F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSingleMgRecordNumGet
#endif // clang-format on
}

asm void fn_802144F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSingleMgRecordNumSet
#endif // clang-format on
}

asm void fn_802144F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPSprCreateVol
#endif // clang-format on
}

asm void fn_802144FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPSprCreate
#endif // clang-format on
}

asm void fn_80214500(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHP3DCreateVol
#endif // clang-format on
}

asm void fn_80214504(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHP3DCreate
#endif // clang-format on
}

asm void fn_80214508(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPStop
#endif // clang-format on
}

asm void fn_8021450C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPClose
#endif // clang-format on
}

asm void fn_80214510(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPRestart
#endif // clang-format on
}

asm void fn_80214514(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPEndCheck
#endif // clang-format on
}

asm void fn_80214518(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPFrameGet
#endif // clang-format on
}

asm void fn_8021451C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPTotalFrameGet
#endif // clang-format on
}

asm void fn_80214520(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPSetVolume
#endif // clang-format on
}

asm void fn_80214524(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b THPSimpleGetFrameRate
#endif // clang-format on
}

asm void fn_80214528(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPMatHook
#endif // clang-format on
}

asm void fn_8021452C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPStartCheck
#endif // clang-format on
}

asm void fn_80214530(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardCheck
#endif // clang-format on
}

asm void fn_80214534(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardMount
#endif // clang-format on
}

asm void fn_80214538(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardFormat
#endif // clang-format on
}

asm void fn_8021453C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardOpen
#endif // clang-format on
}

asm void fn_80214540(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardRead
#endif // clang-format on
}

asm void fn_80214544(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardCreate
#endif // clang-format on
}

asm void fn_80214548(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardClose
#endif // clang-format on
}

asm void fn_8021454C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardWrite
#endif // clang-format on
}

asm void fn_80214550(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardDelete
#endif // clang-format on
}

asm void fn_80214554(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardSectorSizeGet
#endif // clang-format on
}

asm void fn_80214558(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardFreeSpaceGet
#endif // clang-format on
}

asm void fn_8021455C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardSlotCheck
#endif // clang-format on
}

asm void fn_80214560(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardWriteAsync
#endif // clang-format on
}

asm void fn_80214564(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardWriteCheck
#endif // clang-format on
}

asm void fn_80214568(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuCardWriteIdle
#endif // clang-format on
}

asm void fn_8021456C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLFileOpen
#endif // clang-format on
}

asm void fn_80214570(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLFileCreate
#endif // clang-format on
}

asm void fn_80214574(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLFileWrite
#endif // clang-format on
}

asm void fn_80214578(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLFileClose
#endif // clang-format on
}

asm void fn_8021457C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLFileRead
#endif // clang-format on
}

asm void fn_80214580(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSaveEmptySet
#endif // clang-format on
}

asm void fn_80214584(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCurSlotNoSet
#endif // clang-format on
}

asm void fn_80214588(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCurSlotNoGet
#endif // clang-format on
}

asm void fn_8021458C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSaveDataMake
#endif // clang-format on
}

asm void fn_80214590(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSaveDataSlotMake
#endif // clang-format on
}

asm void fn_80214594(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCommonSet
#endif // clang-format on
}

asm void fn_80214598(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLBoardSave
#endif // clang-format on
}

asm void fn_8021459C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSave
#endif // clang-format on
}

asm void fn_802145A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCommonLoad
#endif // clang-format on
}

asm void fn_802145A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLBoardLoad
#endif // clang-format on
}

asm void fn_802145A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLLoad
#endif // clang-format on
}

asm void fn_802145AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCurBoxNoSet
#endif // clang-format on
}

asm void fn_802145B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCurBoxNoGet
#endif // clang-format on
}

asm void fn_802145B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSaveFlagSet
#endif // clang-format on
}

asm void fn_802145B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSaveFlagGet
#endif // clang-format on
}

asm void fn_802145BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCheckSumCheck
#endif // clang-format on
}

asm void fn_802145C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCheckSumBoxSlotCheck
#endif // clang-format on
}

asm void fn_802145C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCheckSumGet
#endif // clang-format on
}

asm void fn_802145C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCheckSumSlotGet
#endif // clang-format on
}

asm void fn_802145CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSerialNoGet
#endif // clang-format on
}

asm void fn_802145D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSerialNoCheck
#endif // clang-format on
}

asm void fn_802145D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b saveExecF
#endif // clang-format on
}

asm void fn_802145D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLWinIdSet
#endif // clang-format on
}

asm void fn_802145DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLBoxBackupLoad
#endif // clang-format on
}

asm void fn_802145E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLBoxBackupSlotLoad
#endif // clang-format on
}

asm void fn_802145E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCheckSumBoxSet
#endif // clang-format on
}

asm void fn_802145E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLBoxDataOffsetGet
#endif // clang-format on
}

asm void fn_802145EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSaveBoardTurnExec
#endif // clang-format on
}

asm void fn_802145F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSaveBoardEndExec
#endif // clang-format on
}

asm void fn_802145F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLSaveModeExec
#endif // clang-format on
}

asm void fn_802145F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b curSlotNo
#endif // clang-format on
}

asm void fn_802145FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b UnMountCnt
#endif // clang-format on
}

asm void fn_80214600(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCommonSaveCopy
#endif // clang-format on
}

asm void fn_80214604(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b SLCommonLoadCopy
#endif // clang-format on
}

asm void fn_80214608(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _CheckFlag
#endif // clang-format on
}

asm void fn_8021460C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _SetFlag
#endif // clang-format on
}

asm void fn_80214610(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _ClearFlag
#endif // clang-format on
}

asm void fn_80214614(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgDataTbl
#endif // clang-format on
}

asm void fn_80214618(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgModeSubMode
#endif // clang-format on
}

asm void fn_8021461C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgModeScore
#endif // clang-format on
}

asm void fn_80214620(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgModePlayNum
#endif // clang-format on
}

asm void fn_80214624(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgModeWork
#endif // clang-format on
}

asm void fn_80214628(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgModePlayNum
#endif // clang-format on
}

asm void fn_8021462C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GwCommonBackup
#endif // clang-format on
}

asm void fn_80214630(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b lbl_802C0438
#endif // clang-format on
}

asm void fn_80214634(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgExitReq
#endif // clang-format on
}

asm void fn_80214638(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPauseExitF
#endif // clang-format on
}

asm void fn_8021463C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgBoardE3F
#endif // clang-format on
}

asm void fn_80214640(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgNoGet
#endif // clang-format on
}

asm void fn_80214644(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MBookEvtNo
#endif // clang-format on
}

asm void fn_80214648(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSubMode
#endif // clang-format on
}

asm void fn_8021464C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSubModeGet
#endif // clang-format on
}

asm void fn_80214650(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgBattleOrder
#endif // clang-format on
}

asm void fn_80214654(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgDecaScoreCalc
#endif // clang-format on
}

asm void fn_80214658(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgInstExitF
#endif // clang-format on
}

asm void fn_8021465C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWRecordSet
#endif // clang-format on
}

asm void fn_80214660(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWRecordGet
#endif // clang-format on
}

asm void fn_80214664(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBoardPlayNumAdd
#endif // clang-format on
}

asm void fn_80214668(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBoardPlayNumSet
#endif // clang-format on
}

asm void fn_8021466C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBoardPlayNumGet
#endif // clang-format on
}

asm void fn_80214670(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWCharPlayNumInc
#endif // clang-format on
}

asm void fn_80214674(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWCharPlayNumGet
#endif // clang-format on
}

asm void fn_80214678(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWCharPlayNumSet
#endif // clang-format on
}

asm void fn_8021467C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBoardMaxStarSet
#endif // clang-format on
}

asm void fn_80214680(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBoardMaxStarGet
#endif // clang-format on
}

asm void fn_80214684(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBoardMaxCoinSet
#endif // clang-format on
}

asm void fn_80214688(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWBoardMaxCoinGet
#endif // clang-format on
}

asm void fn_8021468C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWPlayerStarAdd
#endif // clang-format on
}

asm void fn_80214690(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWPlayerStarGet
#endif // clang-format on
}

asm void fn_80214694(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWPlayerStarSet
#endif // clang-format on
}

asm void fn_80214698(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWPlayerCoinAdd
#endif // clang-format on
}

asm void fn_8021469C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWPlayerCoinSet
#endif // clang-format on
}

asm void fn_802146A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWPlayerCoinGet
#endif // clang-format on
}

asm void fn_802146A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMgCustomReset
#endif // clang-format on
}

asm void fn_802146A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMgCustomSet
#endif // clang-format on
}

asm void fn_802146AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMgCustomGet
#endif // clang-format on
}

asm void fn_802146B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMgUnlockGet
#endif // clang-format on
}

asm void fn_802146B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMgPlayNumSet
#endif // clang-format on
}

asm void fn_802146B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMgPlayNumGet
#endif // clang-format on
}

asm void fn_802146BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMgPlayNumAdd
#endif // clang-format on
}

asm void fn_802146C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMiracleBookFlagSet
#endif // clang-format on
}

asm void fn_802146C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMiracleBookFlagGet
#endif // clang-format on
}

asm void fn_802146C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMikeActRecordSet
#endif // clang-format on
}

asm void fn_802146CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWMikeActRecordGet
#endif // clang-format on
}

asm void fn_802146D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GWSaveDebugSet
#endif // clang-format on
}

asm void fn_802146D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgModeWorkInt
#endif // clang-format on
}

asm void fn_802146D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgModeWorkFloat
#endif // clang-format on
}

asm void fn_802146DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GwMgDecaScore
#endif // clang-format on
}

asm void fn_802146E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GwLanguage
#endif // clang-format on
}

asm void fn_802146E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fmod
#endif // clang-format on
}

asm void fn_802146E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _savegpr_15
#endif // clang-format on
}

asm void fn_802146EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _restgpr_15
#endif // clang-format on
}

asm void fn_802146F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _savegpr_16
#endif // clang-format on
}

asm void fn_802146F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _restgpr_16
#endif // clang-format on
}

asm void fn_802146F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _savegpr_14
#endif // clang-format on
}

asm void fn_802146FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _restgpr_14
#endif // clang-format on
}

asm void fn_80214700(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omDBGMenuButton
#endif // clang-format on
}

asm void fn_80214704(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omDBGSystemKeyCheckSetup
#endif // clang-format on
}

asm void fn_80214708(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b msmSeSetListener
#endif // clang-format on
}

asm void fn_8021470C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b msmSeUpdataListener
#endif // clang-format on
}

asm void fn_80214710(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b msmSeGetIndexPtr
#endif // clang-format on
}

asm void fn_80214714(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b msmMusSetMasterVolume
#endif // clang-format on
}

asm void fn_80214718(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b msmSysGetOutputMode
#endif // clang-format on
}

asm void fn_8021471C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b msmSysSetGroupLoadMode
#endif // clang-format on
}

asm void fn_80214720(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b WipeCreate
#endif // clang-format on
}

asm void fn_80214724(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b WipeColorSet
#endif // clang-format on
}

asm void fn_80214728(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omOvlReturnEx
#endif // clang-format on
}

asm void fn_8021472C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataGetAsyncStat
#endif // clang-format on
}

asm void fn_80214730(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omVibrate
#endif // clang-format on
}

asm void fn_80214734(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GameMesOvlPrev
#endif // clang-format on
}

asm void fn_80214738(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerCreate
#endif // clang-format on
}

asm void fn_8021473C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerKill
#endif // clang-format on
}

asm void fn_80214740(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerModeGet
#endif // clang-format on
}

asm void fn_80214744(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerModeOnSet
#endif // clang-format on
}

asm void fn_80214748(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerModeOffSet
#endif // clang-format on
}

asm void fn_8021474C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerParamSet
#endif // clang-format on
}

asm void fn_80214750(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerValueGet
#endif // clang-format on
}

asm void fn_80214754(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerDoneCheck
#endif // clang-format on
}

asm void fn_80214758(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerPosSet
#endif // clang-format on
}

asm void fn_8021475C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerPosGet
#endif // clang-format on
}

asm void fn_80214760(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerBackColorSet
#endif // clang-format on
}

asm void fn_80214764(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerColorSet
#endif // clang-format on
}

asm void fn_80214768(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerRecordSet
#endif // clang-format on
}

asm void fn_8021476C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerRecordDispOn
#endif // clang-format on
}

asm void fn_80214770(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgTimerRecordDispOff
#endif // clang-format on
}

asm void fn_80214774(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreInit
#endif // clang-format on
}

asm void fn_80214778(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreKill
#endif // clang-format on
}

asm void fn_8021477C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreModeGet
#endif // clang-format on
}

asm void fn_80214780(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreValueSet
#endif // clang-format on
}

asm void fn_80214784(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreValueGet
#endif // clang-format on
}

asm void fn_80214788(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreMaxDigitSet
#endif // clang-format on
}

asm void fn_8021478C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreMaxDigitGet
#endif // clang-format on
}

asm void fn_80214790(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScorePosSet
#endif // clang-format on
}

asm void fn_80214794(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScorePosGet
#endif // clang-format on
}

asm void fn_80214798(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreScaleSet
#endif // clang-format on
}

asm void fn_8021479C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreScaleGet
#endif // clang-format on
}

asm void fn_802147A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreZRotSet
#endif // clang-format on
}

asm void fn_802147A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreZRotGet
#endif // clang-format on
}

asm void fn_802147A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreDigitWidthSet
#endif // clang-format on
}

asm void fn_802147AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreDigitWidthGet
#endif // clang-format on
}

asm void fn_802147B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreDispOn
#endif // clang-format on
}

asm void fn_802147B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreDispOff
#endif // clang-format on
}

asm void fn_802147B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreTPLvlSet
#endif // clang-format on
}

asm void fn_802147BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreTPLvlGet
#endif // clang-format on
}

asm void fn_802147C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreModeDefaultSet
#endif // clang-format on
}

asm void fn_802147C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreCreate
#endif // clang-format on
}

asm void fn_802147C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreColorSet
#endif // clang-format on
}

asm void fn_802147CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreDigitScaleSet
#endif // clang-format on
}

asm void fn_802147D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScorePriSet
#endif // clang-format on
}

asm void fn_802147D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgScoreUnitBankSet
#endif // clang-format on
}

asm void fn_802147D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqCreate
#endif // clang-format on
}

asm void fn_802147DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqCreatePrio
#endif // clang-format on
}

asm void fn_802147E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqKill
#endif // clang-format on
}

asm void fn_802147E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqModeGet
#endif // clang-format on
}

asm void fn_802147E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqModeNext
#endif // clang-format on
}

asm void fn_802147EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqModeSet
#endif // clang-format on
}

asm void fn_802147F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqRecordSet
#endif // clang-format on
}

asm void fn_802147F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqModeChangeOff
#endif // clang-format on
}

asm void fn_802147F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqModeChangeOn
#endif // clang-format on
}

asm void fn_802147FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqStatGet
#endif // clang-format on
}

asm void fn_80214800(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqWinnerSet
#endif // clang-format on
}

asm void fn_80214804(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqDrawSet
#endif // clang-format on
}

asm void fn_80214808(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_80071CCC
#endif // clang-format on
}

asm void fn_8021480C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqModeDelaySet
#endif // clang-format on
}

asm void fn_80214810(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqModeHookAdd
#endif // clang-format on
}

asm void fn_80214814(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqModeHookReset
#endif // clang-format on
}

asm void fn_80214818(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqTimerValueGet
#endif // clang-format on
}

asm void fn_8021481C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqFrameNoGet
#endif // clang-format on
}

asm void fn_80214820(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqFrameNoSet
#endif // clang-format on
}

asm void fn_80214824(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqGameMesIdGet
#endif // clang-format on
}

asm void fn_80214828(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqTimerKill
#endif // clang-format on
}

asm void fn_8021482C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqStatBitSet
#endif // clang-format on
}

asm void fn_80214830(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgSeqStatBitReset
#endif // clang-format on
}

asm void fn_80214834(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omCameraViewInit
#endif // clang-format on
}

asm void fn_80214838(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omCameraViewSetMulti
#endif // clang-format on
}

asm void fn_8021483C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omCameraViewSet
#endif // clang-format on
}

asm void fn_80214840(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omCameraViewMoveSimpleMulti
#endif // clang-format on
}

asm void fn_80214844(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omCameraViewMoveSimple
#endif // clang-format on
}

asm void fn_80214848(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omCameraViewMoveMulti
#endif // clang-format on
}

asm void fn_8021484C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omCameraViewMove
#endif // clang-format on
}

asm void fn_80214850(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b omCameraViewCheck
#endif // clang-format on
}

asm void fn_80214854(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorObjectSetup
#endif // clang-format on
}

asm void fn_80214858(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColMapInit
#endif // clang-format on
}

asm void fn_8021485C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerPadSet
#endif // clang-format on
}

asm void fn_80214860(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerModeFuncSet
#endif // clang-format on
}

asm void fn_80214864(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerCreate
#endif // clang-format on
}

asm void fn_80214868(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerCreateJumpAlt
#endif // clang-format on
}

asm void fn_8021486C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorCreate
#endif // clang-format on
}

asm void fn_80214870(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorKill
#endif // clang-format on
}

asm void fn_80214874(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorExec
#endif // clang-format on
}

asm void fn_80214878(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerModeLandSet
#endif // clang-format on
}

asm void fn_8021487C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerModeSet
#endif // clang-format on
}

asm void fn_80214880(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerComStkOn
#endif // clang-format on
}

asm void fn_80214884(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerComStkOff
#endif // clang-format on
}

asm void fn_80214888(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerVecChase
#endif // clang-format on
}

asm void fn_8021488C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorVecChase
#endif // clang-format on
}

asm void fn_80214890(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColAttrSet
#endif // clang-format on
}

asm void fn_80214894(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColAttrReset
#endif // clang-format on
}

asm void fn_80214898(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColAttrGet
#endif // clang-format on
}

asm void fn_8021489C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColMapPolyGet
#endif // clang-format on
}

asm void fn_802148A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorPosSet
#endif // clang-format on
}

asm void fn_802148A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorPosSetRaw
#endif // clang-format on
}

asm void fn_802148A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorPosGet
#endif // clang-format on
}

asm void fn_802148AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerDespawn
#endif // clang-format on
}

asm void fn_802148B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerSpawn
#endif // clang-format on
}

asm void fn_802148B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerColDisable
#endif // clang-format on
}

asm void fn_802148B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerColEnable
#endif // clang-format on
}

asm void fn_802148BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerVibrateCreate
#endif // clang-format on
}

asm void fn_802148C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerVibrateKill
#endif // clang-format on
}

asm void fn_802148C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColAttrParamSet
#endif // clang-format on
}

asm void fn_802148C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColAttrParamGet
#endif // clang-format on
}

asm void fn_802148CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColMapMaskSet
#endif // clang-format on
}

asm void fn_802148D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColMapMaskGet
#endif // clang-format on
}

asm void fn_802148D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColMaskSet
#endif // clang-format on
}

asm void fn_802148D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColMaskGet
#endif // clang-format on
}

asm void fn_802148DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerAttrCheck
#endif // clang-format on
}

asm void fn_802148E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerAttrSet
#endif // clang-format on
}

asm void fn_802148E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerAttrReset
#endif // clang-format on
}

asm void fn_802148E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerModeAttrCheck
#endif // clang-format on
}

asm void fn_802148EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerModeAttrReset
#endif // clang-format on
}

asm void fn_802148F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerModeAttrSet
#endif // clang-format on
}

asm void fn_802148F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerVibAttrCheck
#endif // clang-format on
}

asm void fn_802148F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerVibAttrSet
#endif // clang-format on
}

asm void fn_802148FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerStunSet
#endif // clang-format on
}

asm void fn_80214900(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColCylReset
#endif // clang-format on
}

asm void fn_80214904(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColCylSet
#endif // clang-format on
}

asm void fn_80214908(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColCodeGet
#endif // clang-format on
}

asm void fn_8021490C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColNormalGet
#endif // clang-format on
}

asm void fn_80214910(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorRotYSet
#endif // clang-format on
}

asm void fn_80214914(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorRotYGet
#endif // clang-format on
}

asm void fn_80214918(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorPushSet
#endif // clang-format on
}

asm void fn_8021491C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgPlayerPosSet
#endif // clang-format on
}

asm void fn_80214920(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorGravitySet
#endif // clang-format on
}

asm void fn_80214924(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorVelYSet
#endif // clang-format on
}

asm void fn_80214928(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColMeshGet
#endif // clang-format on
}

asm void fn_8021492C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorVelSet
#endif // clang-format on
}

asm void fn_80214930(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MgActorColBounceSet
#endif // clang-format on
}

asm void fn_80214934(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharModelFileNumGet
#endif // clang-format on
}

asm void fn_80214938(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b CharMotionFileNumGet
#endif // clang-format on
}

asm void fn_8021493C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b lbl_802BF860
#endif // clang-format on
}

asm void fn_80214940(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b lbl_802BF864
#endif // clang-format on
}

asm void fn_80214944(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b lbl_802BF868
#endif // clang-format on
}

asm void fn_80214948(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _savefpr_22
#endif // clang-format on
}

asm void fn_8021494C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _restfpr_22
#endif // clang-format on
}

asm void fn_80214950(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _savefpr_24
#endif // clang-format on
}

asm void fn_80214954(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _restfpr_24
#endif // clang-format on
}

asm void fn_80214958(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _savefpr_27
#endif // clang-format on
}

asm void fn_8021495C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _restfpr_27
#endif // clang-format on
}

asm void fn_80214960(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _savefpr_28
#endif // clang-format on
}

asm void fn_80214964(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b _restfpr_28
#endif // clang-format on
}

asm void fn_80214968(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b __construct_array
#endif // clang-format on
}

asm void fn_8021496C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b __destroy_arr
#endif // clang-format on
}

asm void fn_80214970(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b __msl_assertion_failed
#endif // clang-format on
}

asm void fn_80214974(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b __nw__FUl
#endif // clang-format on
}

asm void fn_80214978(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b __nwa__FUl
#endif // clang-format on
}

asm void fn_8021497C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b __ptmf_test
#endif // clang-format on
}

asm void fn_80214980(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b __ptmf_scall
#endif // clang-format on
}

asm void fn_80214984(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b __dla__FPv
#endif // clang-format on
}

asm void fn_80214988(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b lbl_802166D8
#endif // clang-format on
}

asm void fn_8021498C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b __ptmf_cmpr
#endif // clang-format on
}

asm void fn_80214990(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataDirCancelAsync
#endif // clang-format on
}

asm void fn_80214994(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuDataDirCloseAsync
#endif // clang-format on
}

asm void fn_80214998(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuTHPProcCheck
#endif // clang-format on
}

asm void fn_8021499C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSaveInit
#endif // clang-format on
}

asm void fn_802149A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSavePartyInit
#endif // clang-format on
}

asm void fn_802149A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSaveStoryInit
#endif // clang-format on
}

asm void fn_802149A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_80146BA0
#endif // clang-format on
}

asm void fn_802149AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_80146C88
#endif // clang-format on
}

asm void fn_802149B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuLoadProcStart
#endif // clang-format on
}

asm void fn_802149B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b GwMgTime
#endif // clang-format on
}

asm void fn_802149B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCSysInit
#endif // clang-format on
}

asm void fn_802149BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCInit
#endif // clang-format on
}

asm void fn_802149C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCClose
#endif // clang-format on
}

asm void fn_802149C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCDelayContextStart
#endif // clang-format on
}

asm void fn_802149C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCDelayResponseResultGet
#endif // clang-format on
}

asm void fn_802149CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCDelayResultGet
#endif // clang-format on
}

asm void fn_802149D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCContextStart
#endif // clang-format on
}

asm void fn_802149D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCResponseResultGet
#endif // clang-format on
}

asm void fn_802149D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCContextCreate
#endif // clang-format on
}

asm void fn_802149DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCContextKill
#endif // clang-format on
}

asm void fn_802149E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCThresholdSet
#endif // clang-format on
}

asm void fn_802149E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCCurResponseGet
#endif // clang-format on
}

asm void fn_802149E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCButtonGet
#endif // clang-format on
}

asm void fn_802149EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCButtonDownGet
#endif // clang-format on
}

asm void fn_802149F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCProbe
#endif // clang-format on
}

asm void fn_802149F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCMount
#endif // clang-format on
}

asm void fn_802149F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCMicSprCreate
#endif // clang-format on
}

asm void fn_802149FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCMicSprKill
#endif // clang-format on
}

asm void fn_80214A00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCMicGet
#endif // clang-format on
}

asm void fn_80214A04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCMicSet
#endif // clang-format on
}

asm void fn_80214A08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCSelWinCreate
#endif // clang-format on
}

asm void fn_80214A0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCSelWinKill
#endif // clang-format on
}

asm void fn_80214A10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCSelWinItemSet
#endif // clang-format on
}

asm void fn_80214A14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCSelWinItemRandSet
#endif // clang-format on
}

asm void fn_80214A18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCSelWinCheck
#endif // clang-format on
}

asm void fn_80214A1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCSelModeGet
#endif // clang-format on
}

asm void fn_80214A20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCListenerMQCreate
#endif // clang-format on
}

asm void fn_80214A24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCListenerMQWait
#endif // clang-format on
}

asm void fn_80214A28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCListenerMQKill
#endif // clang-format on
}

asm void fn_80214A2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCListenerMQMaxTimeSet
#endif // clang-format on
}

asm void fn_80214A30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCListenerMQMaxTimeGet
#endif // clang-format on
}

asm void fn_80214A34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCListenerCreate
#endif // clang-format on
}

asm void fn_80214A38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCListenerKill
#endif // clang-format on
}

asm void fn_80214A3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCSessionSet
#endif // clang-format on
}

asm void fn_80214A40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCSessionKill
#endif // clang-format on
}

asm void fn_80214A44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCSessionClose
#endif // clang-format on
}

asm void fn_80214A48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCRetypeResponseCheck
#endif // clang-format on
}

asm void fn_80214A4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCRetypeResponse
#endif // clang-format on
}

asm void fn_80214A50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCLevelInit
#endif // clang-format on
}

asm void fn_80214A54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCLevelGet
#endif // clang-format on
}

asm void fn_80214A58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b HuMCShiftsSet
#endif // clang-format on
}

asm void fn_80214A5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8014C818
#endif // clang-format on
}

asm void fn_80214A60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8014C81C
#endif // clang-format on
}

asm void fn_80214A64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBranchInit
#endif // clang-format on
}

asm void fn_80214A68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_Branch
#endif // clang-format on
}

asm void fn_80214A6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_BranchDebug
#endif // clang-format on
}

asm void fn_80214A70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBranchAttrSet
#endif // clang-format on
}

asm void fn_80214A74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBranchAttrReset
#endif // clang-format on
}

asm void fn_80214A78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBranchAttrGet
#endif // clang-format on
}

asm void fn_80214A7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBranchMAttrSet
#endif // clang-format on
}

asm void fn_80214A80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBranchMAttrReset
#endif // clang-format on
}

asm void fn_80214A84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBranchMAttrGet
#endif // clang-format on
}

asm void fn_80214A88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBranchAttrCheck
#endif // clang-format on
}

asm void fn_80214A8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBranchComStarHookSet
#endif // clang-format on
}

asm void fn_80214A90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraInit
#endif // clang-format on
}

asm void fn_80214A94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraZoomSet
#endif // clang-format on
}

asm void fn_80214A98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraZoomGet
#endif // clang-format on
}

asm void fn_80214A9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraRotSetV
#endif // clang-format on
}

asm void fn_80214AA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraRotSet
#endif // clang-format on
}

asm void fn_80214AA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraRotGet
#endif // clang-format on
}

asm void fn_80214AA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraCenterSetV
#endif // clang-format on
}

asm void fn_80214AAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraCenterSet
#endif // clang-format on
}

asm void fn_80214AB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraCenterGet
#endif // clang-format on
}

asm void fn_80214AB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraEyeSetV
#endif // clang-format on
}

asm void fn_80214AB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraEyeSet
#endif // clang-format on
}

asm void fn_80214ABC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraEyeGet
#endif // clang-format on
}

asm void fn_80214AC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraOffsetSetV
#endif // clang-format on
}

asm void fn_80214AC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraOffsetSet
#endif // clang-format on
}

asm void fn_80214AC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraOffsetGet
#endif // clang-format on
}

asm void fn_80214ACC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraDirGet
#endif // clang-format on
}

asm void fn_80214AD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraPosDirGet
#endif // clang-format on
}

asm void fn_80214AD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraLookAtFuncSet
#endif // clang-format on
}

asm void fn_80214AD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraBitSet
#endif // clang-format on
}

asm void fn_80214ADC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraFovSet
#endif // clang-format on
}

asm void fn_80214AE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraScissorSet
#endif // clang-format on
}

asm void fn_80214AE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraNearFarSet
#endif // clang-format on
}

asm void fn_80214AE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraNearFarGet
#endif // clang-format on
}

asm void fn_80214AEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraGet
#endif // clang-format on
}

asm void fn_80214AF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraStackPush
#endif // clang-format on
}

asm void fn_80214AF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraStackIdxSet
#endif // clang-format on
}

asm void fn_80214AF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraStackPop
#endif // clang-format on
}

asm void fn_80214AFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraLookAtGet
#endif // clang-format on
}

asm void fn_80214B00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraLookAtInvGet
#endif // clang-format on
}

asm void fn_80214B04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraCurveTypeSet
#endif // clang-format on
}

asm void fn_80214B08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraCurveTypeGet
#endif // clang-format on
}

asm void fn_80214B0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraCullCheck
#endif // clang-format on
}

asm void fn_80214B10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraPlayerViewSet
#endif // clang-format on
}

asm void fn_80214B14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraPlayerViewSetFast
#endif // clang-format on
}

asm void fn_80214B18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraPlayerViewGet
#endif // clang-format on
}

asm void fn_80214B1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraPlayerViewNoGet
#endif // clang-format on
}

asm void fn_80214B20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraPlayerViewZoomGet
#endif // clang-format on
}

asm void fn_80214B24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraPlayerViewRotXGet
#endif // clang-format on
}

asm void fn_80214B28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraFocusReset
#endif // clang-format on
}

asm void fn_80214B2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraFocusPosSet
#endif // clang-format on
}

asm void fn_80214B30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraFocusPosAdd
#endif // clang-format on
}

asm void fn_80214B34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraFocusPlayerSet
#endif // clang-format on
}

asm void fn_80214B38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraFocusPlayerAdd
#endif // clang-format on
}

asm void fn_80214B3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraFocusObjSet
#endif // clang-format on
}

asm void fn_80214B40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraFocusObjAdd
#endif // clang-format on
}

asm void fn_80214B44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraFocusMasuSet
#endif // clang-format on
}

asm void fn_80214B48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraFocusMasuAdd
#endif // clang-format on
}

asm void fn_80214B4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraMoveOnSet
#endif // clang-format on
}

asm void fn_80214B50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraSpeedSet
#endif // clang-format on
}

asm void fn_80214B54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraMovePos
#endif // clang-format on
}

asm void fn_80214B58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraMovePlayer
#endif // clang-format on
}

asm void fn_80214B5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraMoveObj
#endif // clang-format on
}

asm void fn_80214B60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraMoveMasu
#endif // clang-format on
}

asm void fn_80214B64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraShakeSet
#endif // clang-format on
}

asm void fn_80214B68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraShakeReset
#endif // clang-format on
}

asm void fn_80214B6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraMoveStop
#endif // clang-format on
}

asm void fn_80214B70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraMoveCheck
#endif // clang-format on
}

asm void fn_80214B74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCameraMoveWait
#endif // clang-format on
}

asm void fn_80214B78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelect
#endif // clang-format on
}

asm void fn_80214B7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapDelete
#endif // clang-format on
}

asm void fn_80214B80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8019A618
#endif // clang-format on
}

asm void fn_80214B84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8019A61C
#endif // clang-format on
}

asm void fn_80214B88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8019A620
#endif // clang-format on
}

asm void fn_80214B8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8019A624
#endif // clang-format on
}

asm void fn_80214B90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8019A628
#endif // clang-format on
}

asm void fn_80214B94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8019A62C
#endif // clang-format on
}

asm void fn_80214B98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectGrow
#endif // clang-format on
}

asm void fn_80214B9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectShrinkCheck
#endif // clang-format on
}

asm void fn_80214BA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectResultSet
#endif // clang-format on
}

asm void fn_80214BA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectResultGet
#endif // clang-format on
}

asm void fn_80214BA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectResultReset
#endif // clang-format on
}

asm void fn_80214BAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuExec
#endif // clang-format on
}

asm void fn_80214BB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuObjInit
#endif // clang-format on
}

asm void fn_80214BB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuObjClose
#endif // clang-format on
}

asm void fn_80214BB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuObjCreateAll
#endif // clang-format on
}

asm void fn_80214BBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuObjCreate
#endif // clang-format on
}

asm void fn_80214BC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuDispSet
#endif // clang-format on
}

asm void fn_80214BC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapCapsuleGet
#endif // clang-format on
}

asm void fn_80214BC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKinoko
#endif // clang-format on
}

asm void fn_80214BCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKinokoKill
#endif // clang-format on
}

asm void fn_80214BD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapSKinoko
#endif // clang-format on
}

asm void fn_80214BD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapSKinokoKill
#endif // clang-format on
}

asm void fn_80214BD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPKinoko
#endif // clang-format on
}

asm void fn_80214BDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPKinokoKill
#endif // clang-format on
}

asm void fn_80214BE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMKinoko
#endif // clang-format on
}

asm void fn_80214BE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMKinokoKill
#endif // clang-format on
}

asm void fn_80214BE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKiller
#endif // clang-format on
}

asm void fn_80214BEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKillerKill
#endif // clang-format on
}

asm void fn_80214BF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDokan
#endif // clang-format on
}

asm void fn_80214BF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDokanKill
#endif // clang-format on
}

asm void fn_80214BF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapHanachan
#endif // clang-format on
}

asm void fn_80214BFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapHanachanKill
#endif // clang-format on
}

asm void fn_80214C00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapNKinoko
#endif // clang-format on
}

asm void fn_80214C04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapNKinokoKill
#endif // clang-format on
}

asm void fn_80214C08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKillerMove
#endif // clang-format on
}

asm void fn_80214C0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKillerMoveKill
#endif // clang-format on
}

asm void fn_80214C10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapTogezo
#endif // clang-format on
}

asm void fn_80214C14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapTogezoKill
#endif // clang-format on
}

asm void fn_80214C18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKuribo
#endif // clang-format on
}

asm void fn_80214C1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKuriboKill
#endif // clang-format on
}

asm void fn_80214C20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPakkun
#endif // clang-format on
}

asm void fn_80214C24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPakkunKill
#endif // clang-format on
}

asm void fn_80214C28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapJango
#endif // clang-format on
}

asm void fn_80214C2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapJangoKill
#endif // clang-format on
}

asm void fn_80214C30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPatapata
#endif // clang-format on
}

asm void fn_80214C34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPatapataKill
#endif // clang-format on
}

asm void fn_80214C38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKokamekku
#endif // clang-format on
}

asm void fn_80214C3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKokamekkuKill
#endif // clang-format on
}

asm void fn_80214C40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKamekku
#endif // clang-format on
}

asm void fn_80214C44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKamekkuKill
#endif // clang-format on
}

asm void fn_80214C48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapThrowman
#endif // clang-format on
}

asm void fn_80214C4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapThrowmanKill
#endif // clang-format on
}

asm void fn_80214C50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBoble
#endif // clang-format on
}

asm void fn_80214C54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBobleKill
#endif // clang-format on
}

asm void fn_80214C58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBobleTrap
#endif // clang-format on
}

asm void fn_80214C5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBobleMove
#endif // clang-format on
}

asm void fn_80214C60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBiriQ
#endif // clang-format on
}

asm void fn_80214C64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBiriQKill
#endif // clang-format on
}

asm void fn_80214C68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBiriQTrap
#endif // clang-format on
}

asm void fn_80214C6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBiriQMetalShock
#endif // clang-format on
}

asm void fn_80214C70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapTumujikun
#endif // clang-format on
}

asm void fn_80214C74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapTumujikunKill
#endif // clang-format on
}

asm void fn_80214C78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapTumujikunTrap
#endif // clang-format on
}

asm void fn_80214C7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDossun
#endif // clang-format on
}

asm void fn_80214C80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDossunKill
#endif // clang-format on
}

asm void fn_80214C84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDossunTrap
#endif // clang-format on
}

asm void fn_80214C88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBomhei
#endif // clang-format on
}

asm void fn_80214C8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBomheiKill
#endif // clang-format on
}

asm void fn_80214C90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBomheiTrap
#endif // clang-format on
}

asm void fn_80214C94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBomheiMove
#endif // clang-format on
}

asm void fn_80214C98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapTeresa
#endif // clang-format on
}

asm void fn_80214C9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapTeresaKill
#endif // clang-format on
}

asm void fn_80214CA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapTeresaFadeCreate
#endif // clang-format on
}

asm void fn_80214CA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapTeresaFadeKill
#endif // clang-format on
}

asm void fn_80214CA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapTeresaFadeSet
#endif // clang-format on
}

asm void fn_80214CAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMiracle
#endif // clang-format on
}

asm void fn_80214CB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMiracleKill
#endif // clang-format on
}

asm void fn_80214CB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKettou
#endif // clang-format on
}

asm void fn_80214CB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKettouKill
#endif // clang-format on
}

asm void fn_80214CBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDonkey
#endif // clang-format on
}

asm void fn_80214CC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDonkeyKill
#endif // clang-format on
}

asm void fn_80214CC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKoopa
#endif // clang-format on
}

asm void fn_80214CC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKoopaKill
#endif // clang-format on
}

asm void fn_80214CCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCall
#endif // clang-format on
}

asm void fn_80214CD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCallTrap
#endif // clang-format on
}

asm void fn_80214CD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCapGet
#endif // clang-format on
}

asm void fn_80214CD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapWait
#endif // clang-format on
}

asm void fn_80214CDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCallKettou
#endif // clang-format on
}

asm void fn_80214CE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCircuitCallKettou
#endif // clang-format on
}

asm void fn_80214CE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCallDonkey
#endif // clang-format on
}

asm void fn_80214CE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCallKoopa
#endif // clang-format on
}

asm void fn_80214CEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MBCapsuleStub5
#endif // clang-format on
}

asm void fn_80214CF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MBCapsuleStub6
#endif // clang-format on
}

asm void fn_80214CF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MBCapsuleStub7
#endif // clang-format on
}

asm void fn_80214CF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCallTeresa
#endif // clang-format on
}

asm void fn_80214CFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCallMiracle
#endif // clang-format on
}

asm void fn_80214D00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBiriQShockCreate
#endif // clang-format on
}

asm void fn_80214D04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBiriQShockDelayGet
#endif // clang-format on
}

asm void fn_80214D08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBiriQMetalShockCreate
#endif // clang-format on
}

asm void fn_80214D0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapRandomBonusCoin
#endif // clang-format on
}

asm void fn_80214D10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBonusCoinCall
#endif // clang-format on
}

asm void fn_80214D14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBonusCoin
#endif // clang-format on
}

asm void fn_80214D18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBonusCoinCheck
#endif // clang-format on
}

asm void fn_80214D1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMoveMasuSet
#endif // clang-format on
}

asm void fn_80214D20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStopMasuSet
#endif // clang-format on
}

asm void fn_80214D24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKettouEndCall
#endif // clang-format on
}

asm void fn_80214D28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDonkeyEndCall
#endif // clang-format on
}

asm void fn_80214D2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKoopaEndCall
#endif // clang-format on
}

asm void fn_80214D30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapVsEndCall
#endif // clang-format on
}

asm void fn_80214D34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKillerMoveCall
#endif // clang-format on
}

asm void fn_80214D38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MBCapsuleStub11
#endif // clang-format on
}

asm void fn_80214D3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MBCapsuleStub12
#endif // clang-format on
}

asm void fn_80214D40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKillerMoveCheck
#endif // clang-format on
}

asm void fn_80214D44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKillerMoveCheckAll
#endif // clang-format on
}

asm void fn_80214D48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBankCoinInit
#endif // clang-format on
}

asm void fn_80214D4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapInit
#endif // clang-format on
}

asm void fn_80214D50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKillerCall
#endif // clang-format on
}

asm void fn_80214D54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKillerMultiCall
#endif // clang-format on
}

asm void fn_80214D58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapOpeningAdd
#endif // clang-format on
}

asm void fn_80214D5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKoopaAdd
#endif // clang-format on
}

asm void fn_80214D60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBankCoinGet
#endif // clang-format on
}

asm void fn_80214D64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBubbleHookSet
#endif // clang-format on
}

asm void fn_80214D68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBubbleHookCall
#endif // clang-format on
}

asm void fn_80214D6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBubbleHookCallStory
#endif // clang-format on
}

asm void fn_80214D70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKettouCoinLoseGet
#endif // clang-format on
}

asm void fn_80214D74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKettouOppCoinLoseGet
#endif // clang-format on
}

asm void fn_80214D78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKettouCoinLoseGet2
#endif // clang-format on
}

asm void fn_80214D7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapKettouOppCoinLoseGet2
#endif // clang-format on
}

asm void fn_80214D80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMotionCreate
#endif // clang-format on
}

asm void fn_80214D84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapObjCreate
#endif // clang-format on
}

asm void fn_80214D88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapObjPosSet
#endif // clang-format on
}

asm void fn_80214D8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerPosSet
#endif // clang-format on
}

asm void fn_80214D90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapSprCreate
#endif // clang-format on
}

asm void fn_80214D94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMalloc
#endif // clang-format on
}

asm void fn_80214D98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapObjClose
#endif // clang-format on
}

asm void fn_80214D9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapSprClose
#endif // clang-format on
}

asm void fn_80214DA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMallocClose
#endif // clang-format on
}

asm void fn_80214DA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffExplodeCreate
#endif // clang-format on
}

asm void fn_80214DA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffExhaustCreate
#endif // clang-format on
}

asm void fn_80214DAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffExplodeOMExec
#endif // clang-format on
}

asm void fn_80214DB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffExplodeKill
#endif // clang-format on
}

asm void fn_80214DB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffExplodeAnimGet
#endif // clang-format on
}

asm void fn_80214DB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffExplodeAnimSet
#endif // clang-format on
}

asm void fn_80214DBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffExplodeAdd
#endif // clang-format on
}

asm void fn_80214DC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffExplodeKillerAdd
#endif // clang-format on
}

asm void fn_80214DC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffExplodeCircleAdd
#endif // clang-format on
}

asm void fn_80214DC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffDustCloudAdd
#endif // clang-format on
}

asm void fn_80214DCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffDustExplodeAdd
#endif // clang-format on
}

asm void fn_80214DD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffDustHeavyAdd
#endif // clang-format on
}

asm void fn_80214DD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffDustMultiAdd
#endif // clang-format on
}

asm void fn_80214DD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffBoostCreate
#endif // clang-format on
}

asm void fn_80214DDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffBoostOMExec
#endif // clang-format on
}

asm void fn_80214DE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffBoostKill
#endif // clang-format on
}

asm void fn_80214DE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffBoostTimeGet
#endif // clang-format on
}

asm void fn_80214DE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffBoostBlendModeSet
#endif // clang-format on
}

asm void fn_80214DEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffBoostAdd
#endif // clang-format on
}

asm void fn_80214DF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffSnowCreate
#endif // clang-format on
}

asm void fn_80214DF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffSnowOMExec
#endif // clang-format on
}

asm void fn_80214DF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffSnowKill
#endif // clang-format on
}

asm void fn_80214DFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffSnowDispGet
#endif // clang-format on
}

asm void fn_80214E00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffSnowAdd
#endif // clang-format on
}

asm void fn_80214E04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowCreate
#endif // clang-format on
}

asm void fn_80214E08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowFireCreate
#endif // clang-format on
}

asm void fn_80214E0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowOMExec
#endif // clang-format on
}

asm void fn_80214E10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowKill
#endif // clang-format on
}

asm void fn_80214E14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowDispGet
#endif // clang-format on
}

asm void fn_80214E18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowPatSet
#endif // clang-format on
}

asm void fn_80214E1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowBlendModeSet
#endif // clang-format on
}

asm void fn_80214E20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowAnimSet
#endif // clang-format on
}

asm void fn_80214E24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowAdd
#endif // clang-format on
}

asm void fn_80214E28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowKinokoAdd
#endif // clang-format on
}

asm void fn_80214E2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowKinokoAddAlt
#endif // clang-format on
}

asm void fn_80214E30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowKinokoTimeSet
#endif // clang-format on
}

asm void fn_80214E34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffGlowCoinAdd
#endif // clang-format on
}

asm void fn_80214E38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRingCreate
#endif // clang-format on
}

asm void fn_80214E3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRingHitCreate
#endif // clang-format on
}

asm void fn_80214E40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRingOMExec
#endif // clang-format on
}

asm void fn_80214E44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRingKill
#endif // clang-format on
}

asm void fn_80214E48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRingDispGet
#endif // clang-format on
}

asm void fn_80214E4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRingAdd
#endif // clang-format on
}

asm void fn_80214E50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRingHitAdd
#endif // clang-format on
}

asm void fn_80214E54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRingAnimSet
#endif // clang-format on
}

asm void fn_80214E58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffElectricCreate
#endif // clang-format on
}

asm void fn_80214E5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffElectricOMExec
#endif // clang-format on
}

asm void fn_80214E60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffElectricKill
#endif // clang-format on
}

asm void fn_80214E64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffElectricDispGet
#endif // clang-format on
}

asm void fn_80214E68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffElectricAdd
#endif // clang-format on
}

asm void fn_80214E6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffElectricModelSet
#endif // clang-format on
}

asm void fn_80214E70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRayCreate
#endif // clang-format on
}

asm void fn_80214E74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRayOMExec
#endif // clang-format on
}

asm void fn_80214E78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRayKill
#endif // clang-format on
}

asm void fn_80214E7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRayDraw
#endif // clang-format on
}

asm void fn_80214E80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRayAdd
#endif // clang-format on
}

asm void fn_80214E84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRayAlphaSet
#endif // clang-format on
}

asm void fn_80214E88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffRayTransformSet
#endif // clang-format on
}

asm void fn_80214E8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffMasuHitCreate
#endif // clang-format on
}

asm void fn_80214E90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffMasuHitOMExec
#endif // clang-format on
}

asm void fn_80214E94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffMasuHitKill
#endif // clang-format on
}

asm void fn_80214E98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffMasuHitAdd
#endif // clang-format on
}

asm void fn_80214E9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffMasuHitTransformSet
#endif // clang-format on
}

asm void fn_80214EA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCoinCreate
#endif // clang-format on
}

asm void fn_80214EA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCoinOMExec
#endif // clang-format on
}

asm void fn_80214EA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCoinKill
#endif // clang-format on
}

asm void fn_80214EAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCoinNumGet
#endif // clang-format on
}

asm void fn_80214EB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCoinAdd
#endif // clang-format on
}

asm void fn_80214EB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCoinMaxYSet
#endif // clang-format on
}

asm void fn_80214EB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCoinMultiAdd
#endif // clang-format on
}

asm void fn_80214EBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCoinGlowSet
#endif // clang-format on
}

asm void fn_80214EC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCoinAdd
#endif // clang-format on
}

asm void fn_80214EC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCapLoseCreate
#endif // clang-format on
}

asm void fn_80214EC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCapLoseOMExec
#endif // clang-format on
}

asm void fn_80214ECC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCapLoseKill
#endif // clang-format on
}

asm void fn_80214ED0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCapLoseNumGet
#endif // clang-format on
}

asm void fn_80214ED4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCapLoseObjAdd
#endif // clang-format on
}

asm void fn_80214ED8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffCapLoseAdd
#endif // clang-format on
}

asm void fn_80214EDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCoinManCreate
#endif // clang-format on
}

asm void fn_80214EE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCoinManOMExec
#endif // clang-format on
}

asm void fn_80214EE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCoinManKill
#endif // clang-format on
}

asm void fn_80214EE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCoinManNumGet
#endif // clang-format on
}

asm void fn_80214EEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCoinManObjAdd
#endif // clang-format on
}

asm void fn_80214EF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCoinManAdd2
#endif // clang-format on
}

asm void fn_80214EF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCoinManAdd
#endif // clang-format on
}

asm void fn_80214EF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStarManCreate
#endif // clang-format on
}

asm void fn_80214EFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStarManOMExec
#endif // clang-format on
}

asm void fn_80214F00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStarManKill
#endif // clang-format on
}

asm void fn_80214F04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStarManNumGet
#endif // clang-format on
}

asm void fn_80214F08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStarManObjAdd
#endif // clang-format on
}

asm void fn_80214F0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStarManAdd2
#endif // clang-format on
}

asm void fn_80214F10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStarManAdd
#endif // clang-format on
}

asm void fn_80214F14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMoveObjInit
#endif // clang-format on
}

asm void fn_80214F18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMoveHitCreate
#endif // clang-format on
}

asm void fn_80214F1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMoveEjectCreate
#endif // clang-format on
}

asm void fn_80214F20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMoveIdleCreate
#endif // clang-format on
}

asm void fn_80214F24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMoveMinYSet
#endif // clang-format on
}

asm void fn_80214F28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMoveVelSet
#endif // clang-format on
}

asm void fn_80214F2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMoveObjExec
#endif // clang-format on
}

asm void fn_80214F30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMoveObjCheck
#endif // clang-format on
}

asm void fn_80214F34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMoveObjClose
#endif // clang-format on
}

asm void fn_80214F38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMoveObjKill
#endif // clang-format on
}

asm void fn_80214F3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerSquishSet
#endif // clang-format on
}

asm void fn_80214F40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerSquishVoiceSet
#endif // clang-format on
}

asm void fn_80214F44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerStunSet
#endif // clang-format on
}

asm void fn_80214F48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerShockSet
#endif // clang-format on
}

asm void fn_80214F4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerCheck
#endif // clang-format on
}

asm void fn_80214F50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCullPlayerCheck
#endif // clang-format on
}

asm void fn_80214F54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCullCheck
#endif // clang-format on
}

asm void fn_80214F58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPointCullCheck
#endif // clang-format on
}

asm void fn_80214F5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMasuNumGet
#endif // clang-format on
}

asm void fn_80214F60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerNoSearch
#endif // clang-format on
}

asm void fn_80214F64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerOrderGet
#endif // clang-format on
}

asm void fn_80214F68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMasuLinkNextGet
#endif // clang-format on
}

asm void fn_80214F6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMasuLinkNextRandomGet
#endif // clang-format on
}

asm void fn_80214F70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMasuValidPrevGet
#endif // clang-format on
}

asm void fn_80214F74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMasuPrevGet
#endif // clang-format on
}

asm void fn_80214F78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerComSelGet
#endif // clang-format on
}

asm void fn_80214F7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerComSelSameGet
#endif // clang-format on
}

asm void fn_80214F80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerComSelRandomGet
#endif // clang-format on
}

asm void fn_80214F84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerComSelKettouGet
#endif // clang-format on
}

asm void fn_80214F88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapEffColorSet
#endif // clang-format on
}

asm void fn_80214F8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMotShiftCheck
#endif // clang-format on
}

asm void fn_80214F90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMotShiftSet
#endif // clang-format on
}

asm void fn_80214F94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMotShiftWait
#endif // clang-format on
}

asm void fn_80214F98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapObjMotionSet
#endif // clang-format on
}

asm void fn_80214F9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapObjMotionOMExec
#endif // clang-format on
}

asm void fn_80214FA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMotionSet
#endif // clang-format on
}

asm void fn_80214FA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerMotionOMExec
#endif // clang-format on
}

asm void fn_80214FA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerRotate
#endif // clang-format on
}

asm void fn_80214FAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapPlayerIdleWait
#endif // clang-format on
}

asm void fn_80214FB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapCoinDisp
#endif // clang-format on
}

asm void fn_80214FB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapMasuMoveCheck
#endif // clang-format on
}

asm void fn_80214FB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapVibrate
#endif // clang-format on
}

asm void fn_80214FBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStatusDispSet
#endif // clang-format on
}

asm void fn_80214FC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDuelStatusOffSet
#endif // clang-format on
}

asm void fn_80214FC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDuelStatusOnSet
#endif // clang-format on
}

asm void fn_80214FC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapDuelStatusDispSet
#endif // clang-format on
}

asm void fn_80214FCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStatusDispSetAll
#endif // clang-format on
}

asm void fn_80214FD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapStatusDispCheck
#endif // clang-format on
}

asm void fn_80214FD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapChoiceSet
#endif // clang-format on
}

asm void fn_80214FD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapAngleWrap
#endif // clang-format on
}

asm void fn_80214FDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapAngleLerp
#endif // clang-format on
}

asm void fn_80214FE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapAngleSumLerp
#endif // clang-format on
}

asm void fn_80214FE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapVecChase
#endif // clang-format on
}

asm void fn_80214FE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapVecRotGet
#endif // clang-format on
}

asm void fn_80214FEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapColorLerp
#endif // clang-format on
}

asm void fn_80214FF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapHermiteConstGet
#endif // clang-format on
}

asm void fn_80214FF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapHermiteConstGet2
#endif // clang-format on
}

asm void fn_80214FF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapHermiteGetV
#endif // clang-format on
}

asm void fn_80214FFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBezierGet
#endif // clang-format on
}

asm void fn_80215000(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBezierGetV
#endif // clang-format on
}

asm void fn_80215004(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBezierSlopeGet
#endif // clang-format on
}

asm void fn_80215008(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_CapBezierNormGetV
#endif // clang-format on
}

asm void fn_8021500C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_ShopEnableSet
#endif // clang-format on
}

asm void fn_80215010(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_ShopInit
#endif // clang-format on
}

asm void fn_80215014(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_ShopCreate
#endif // clang-format on
}

asm void fn_80215018(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_ShopExInit
#endif // clang-format on
}

asm void fn_8021501C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_Shop
#endif // clang-format on
}

asm void fn_80215020(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_ShopExObjHookSet
#endif // clang-format on
}

asm void fn_80215024(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_ShopBackCreate
#endif // clang-format on
}

asm void fn_80215028(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_ShopBackMotCreate
#endif // clang-format on
}

asm void fn_8021502C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapUse
#endif // clang-format on
}

asm void fn_80215030(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MBCapsuleStub1
#endif // clang-format on
}

asm void fn_80215034(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b MBCapsuleStub2
#endif // clang-format on
}

asm void fn_80215038(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapEffUseCreate
#endif // clang-format on
}

asm void fn_8021503C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapEffUseModeGet
#endif // clang-format on
}

asm void fn_80215040(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapEffUsePosGet
#endif // clang-format on
}

asm void fn_80215044(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapEffUseWanWanCreate
#endif // clang-format on
}

asm void fn_80215048(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapPlayerThrow
#endif // clang-format on
}

asm void fn_8021504C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapPlayerThrowCheck
#endif // clang-format on
}

asm void fn_80215050(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapThrowColCreate
#endif // clang-format on
}

asm void fn_80215054(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapThrowColCheck
#endif // clang-format on
}

asm void fn_80215058(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapEffThrowCreate
#endif // clang-format on
}

asm void fn_8021505C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapEffThrowCheck
#endif // clang-format on
}

asm void fn_80215060(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapAutoThrow
#endif // clang-format on
}

asm void fn_80215064(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapThrowHookSet
#endif // clang-format on
}

asm void fn_80215068(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectMasu
#endif // clang-format on
}

asm void fn_8021506C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectMasuInit
#endif // clang-format on
}

asm void fn_80215070(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapListInit
#endif // clang-format on
}

asm void fn_80215074(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapListRead
#endif // clang-format on
}

asm void fn_80215078(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapListCopy
#endif // clang-format on
}

asm void fn_8021507C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapListDebug
#endif // clang-format on
}

asm void fn_80215080(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapNumInc
#endif // clang-format on
}

asm void fn_80215084(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapNumDebug
#endif // clang-format on
}

asm void fn_80215088(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapValueTypeGet
#endif // clang-format on
}

asm void fn_8021508C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuTypeGet
#endif // clang-format on
}

asm void fn_80215090(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapValuePlayerGet
#endif // clang-format on
}

asm void fn_80215094(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuPlayerGet2
#endif // clang-format on
}

asm void fn_80215098(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuPlayerSet
#endif // clang-format on
}

asm void fn_8021509C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuPlayerTypeSet
#endif // clang-format on
}

asm void fn_802150A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuPlayerGet
#endif // clang-format on
}

asm void fn_802150A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapUseModeGet
#endif // clang-format on
}

asm void fn_802150A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuDispTypeGet
#endif // clang-format on
}

asm void fn_802150AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapUseTrapCheck
#endif // clang-format on
}

asm void fn_802150B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapBuyCostGet
#endif // clang-format on
}

asm void fn_802150B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSellCostGet
#endif // clang-format on
}

asm void fn_802150B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapCostGet
#endif // clang-format on
}

asm void fn_802150BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapThrowMasuCheck
#endif // clang-format on
}

asm void fn_802150C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapComChanceGet
#endif // clang-format on
}

asm void fn_802150C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectComGet
#endif // clang-format on
}

asm void fn_802150C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectDeleteComGet
#endif // clang-format on
}

asm void fn_802150CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapRandomThrowAdd
#endif // clang-format on
}

asm void fn_802150D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapRandomTrapAdd
#endif // clang-format on
}

asm void fn_802150D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuCapsuleSet
#endif // clang-format on
}

asm void fn_802150D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapFileGet
#endif // clang-format on
}

asm void fn_802150DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapDescMesGet
#endif // clang-format on
}

asm void fn_802150E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapUseMesGet
#endif // clang-format on
}

asm void fn_802150E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapDebugNameGet
#endif // clang-format on
}

asm void fn_802150E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuPatGet
#endif // clang-format on
}

asm void fn_802150EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapColorGet
#endif // clang-format on
}

asm void fn_802150F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapUseCostGet
#endif // clang-format on
}

asm void fn_802150F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapUseCheck
#endif // clang-format on
}

asm void fn_802150F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapValidCheck
#endif // clang-format on
}

asm void fn_802150FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectMasuNum
#endif // clang-format on
}

asm void fn_80215100(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectMasuFrontNum
#endif // clang-format on
}

asm void fn_80215104(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapSelectMasuBackNum
#endif // clang-format on
}

asm void fn_80215108(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapValidListGet
#endif // clang-format on
}

asm void fn_8021510C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapNextGet
#endif // clang-format on
}

asm void fn_80215110(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapMasuNextGet
#endif // clang-format on
}

asm void fn_80215114(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapShopListGet
#endif // clang-format on
}

asm void fn_80215118(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapRandomListGet
#endif // clang-format on
}

asm void fn_8021511C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapBonusCoinNumGet
#endif // clang-format on
}

asm void fn_80215120(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapDescWinCreate
#endif // clang-format on
}

asm void fn_80215124(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapInit
#endif // clang-format on
}

asm void fn_80215128(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjCreate
#endif // clang-format on
}

asm void fn_8021512C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjBorderCreate
#endif // clang-format on
}

asm void fn_80215130(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjKill
#endif // clang-format on
}

asm void fn_80215134(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjBorderKill
#endif // clang-format on
}

asm void fn_80215138(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorCreate
#endif // clang-format on
}

asm void fn_8021513C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorKill
#endif // clang-format on
}

asm void fn_80215140(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorPosSet
#endif // clang-format on
}

asm void fn_80215144(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorRotSet
#endif // clang-format on
}

asm void fn_80215148(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorScaleSet
#endif // clang-format on
}

asm void fn_8021514C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorPosSetV
#endif // clang-format on
}

asm void fn_80215150(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorRotSetV
#endif // clang-format on
}

asm void fn_80215154(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorScaleSetV
#endif // clang-format on
}

asm void fn_80215158(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorPosGet
#endif // clang-format on
}

asm void fn_8021515C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorRotGet
#endif // clang-format on
}

asm void fn_80215160(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorScaleGet
#endif // clang-format on
}

asm void fn_80215164(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorLayerSet
#endif // clang-format on
}

asm void fn_80215168(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorLayerGet
#endif // clang-format on
}

asm void fn_8021516C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorAttrSet
#endif // clang-format on
}

asm void fn_80215170(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorAttrReset
#endif // clang-format on
}

asm void fn_80215174(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorDispSet
#endif // clang-format on
}

asm void fn_80215178(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorAlphaSet
#endif // clang-format on
}

asm void fn_8021517C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorMtxSet
#endif // clang-format on
}

asm void fn_80215180(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCapObjColorMtxGet
#endif // clang-format on
}

asm void fn_80215184(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinInit
#endif // clang-format on
}

asm void fn_80215188(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinClose
#endif // clang-format on
}

asm void fn_8021518C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinEffObjCreate
#endif // clang-format on
}

asm void fn_80215190(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinEffCreate
#endif // clang-format on
}

asm void fn_80215194(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinDispKill
#endif // clang-format on
}

asm void fn_80215198(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinDispKillCheck
#endif // clang-format on
}

asm void fn_8021519C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinDispCapsuleCreate
#endif // clang-format on
}

asm void fn_802151A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinDispMasuCreate
#endif // clang-format on
}

asm void fn_802151A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinDispCreate
#endif // clang-format on
}

asm void fn_802151A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinAddProcExec
#endif // clang-format on
}

asm void fn_802151AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinAddDispExec
#endif // clang-format on
}

asm void fn_802151B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinAddExec
#endif // clang-format on
}

asm void fn_802151B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinAddAllProcExecV
#endif // clang-format on
}

asm void fn_802151B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinAddAllProcExec
#endif // clang-format on
}

asm void fn_802151BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinAddAllExec
#endif // clang-format on
}

asm void fn_802151C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatTeamMinValGet
#endif // clang-format on
}

asm void fn_802151C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinCreate
#endif // clang-format on
}

asm void fn_802151C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinCreate2
#endif // clang-format on
}

asm void fn_802151CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjCreate
#endif // clang-format on
}

asm void fn_802151D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjCreate2
#endif // clang-format on
}

asm void fn_802151D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjNumDec
#endif // clang-format on
}

asm void fn_802151D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjKill
#endif // clang-format on
}

asm void fn_802151DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjGet
#endif // clang-format on
}

asm void fn_802151E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjPosSet
#endif // clang-format on
}

asm void fn_802151E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjPosSetV
#endif // clang-format on
}

asm void fn_802151E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjRotSet
#endif // clang-format on
}

asm void fn_802151EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjRotSetV
#endif // clang-format on
}

asm void fn_802151F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjScaleSet
#endif // clang-format on
}

asm void fn_802151F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjScaleSetV
#endif // clang-format on
}

asm void fn_802151F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjAlphaSet
#endif // clang-format on
}

asm void fn_802151FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjDispSet
#endif // clang-format on
}

asm void fn_80215200(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjLayerSet
#endif // clang-format on
}

asm void fn_80215204(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjMotSet
#endif // clang-format on
}

asm void fn_80215208(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjPosGet
#endif // clang-format on
}

asm void fn_8021520C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjRotGet
#endif // clang-format on
}

asm void fn_80215210(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjScaleGet
#endif // clang-format on
}

asm void fn_80215214(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjAlphaGet
#endif // clang-format on
}

asm void fn_80215218(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCoinObjDispGet
#endif // clang-format on
}

asm void fn_8021521C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbComChoiceLeftSet
#endif // clang-format on
}

asm void fn_80215220(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbComChoiceUpSet
#endif // clang-format on
}

asm void fn_80215224(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbComChoiceRightSet
#endif // clang-format on
}

asm void fn_80215228(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbComChoiceDownSet
#endif // clang-format on
}

asm void fn_8021522C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbComChoiceListDownSet
#endif // clang-format on
}

asm void fn_80215230(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbComChoiceListRightSet
#endif // clang-format on
}

asm void fn_80215234(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbComChoiceListSet
#endif // clang-format on
}

asm void fn_80215238(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbConfigExec
#endif // clang-format on
}

asm void fn_8021523C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseDispCopyKill
#endif // clang-format on
}

asm void fn_80215240(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseDispCopyCreate
#endif // clang-format on
}

asm void fn_80215244(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbConfigPadDisableSet
#endif // clang-format on
}

asm void fn_80215248(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseGuideCreate
#endif // clang-format on
}

asm void fn_8021524C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseGuideKill
#endif // clang-format on
}

asm void fn_80215250(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelCreate
#endif // clang-format on
}

asm void fn_80215254(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelKill
#endif // clang-format on
}

asm void fn_80215258(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelPosSet
#endif // clang-format on
}

asm void fn_8021525C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelRotSet
#endif // clang-format on
}

asm void fn_80215260(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelScaleSet
#endif // clang-format on
}

asm void fn_80215264(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelBankSet
#endif // clang-format on
}

asm void fn_80215268(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelBatsuSet
#endif // clang-format on
}

asm void fn_8021526C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelPosGet
#endif // clang-format on
}

asm void fn_80215270(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelScaleGet
#endif // clang-format on
}

asm void fn_80215274(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelFreezeGet
#endif // clang-format on
}

asm void fn_80215278(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelUnlockSet
#endif // clang-format on
}

asm void fn_8021527C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelSlideSet
#endif // clang-format on
}

asm void fn_80215280(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelSizeSet
#endif // clang-format on
}

asm void fn_80215284(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelAnmNoSet
#endif // clang-format on
}

asm void fn_80215288(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelGrowSet
#endif // clang-format on
}

asm void fn_8021528C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPausePanelShrinkSet
#endif // clang-format on
}

asm void fn_80215290(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseGuideMoveSet
#endif // clang-format on
}

asm void fn_80215294(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPadDStkRepGetAll
#endif // clang-format on
}

asm void fn_80215298(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPadBtnDownGetAll
#endif // clang-format on
}

asm void fn_8021529C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_Last5
#endif // clang-format on
}

asm void fn_802152A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceInit
#endif // clang-format on
}

asm void fn_802152A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDicePlayerExec
#endif // clang-format on
}

asm void fn_802152A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceExec
#endif // clang-format on
}

asm void fn_802152AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceProcExec
#endif // clang-format on
}

asm void fn_802152B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceChanceTradeExec
#endif // clang-format on
}

asm void fn_802152B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceChanceCharExec
#endif // clang-format on
}

asm void fn_802152B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceFadeSet
#endif // clang-format on
}

asm void fn_802152BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceKill
#endif // clang-format on
}

asm void fn_802152C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceClose
#endif // clang-format on
}

asm void fn_802152C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceKillCheck
#endif // clang-format on
}

asm void fn_802152C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceKillCheckAll
#endif // clang-format on
}

asm void fn_802152CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceResultGet
#endif // clang-format on
}

asm void fn_802152D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceHitHookSet
#endif // clang-format on
}

asm void fn_802152D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceTypeGet
#endif // clang-format on
}

asm void fn_802152D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceTutorialNumSet
#endif // clang-format on
}

asm void fn_802152DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceTutorialNumGet
#endif // clang-format on
}

asm void fn_802152E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceValueNoGet
#endif // clang-format on
}

asm void fn_802152E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceNumKill
#endif // clang-format on
}

asm void fn_802152E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceNumShrinkSet
#endif // clang-format on
}

asm void fn_802152EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceNumStopCheck
#endif // clang-format on
}

asm void fn_802152F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceStub
#endif // clang-format on
}

asm void fn_802152F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceSNpcNumCreate
#endif // clang-format on
}

asm void fn_802152F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceNumObjCreate
#endif // clang-format on
}

asm void fn_802152FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceMaxGet
#endif // clang-format on
}

asm void fn_80215300(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceValueMaxGet
#endif // clang-format on
}

asm void fn_80215304(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDicePadBtnHookSet
#endif // clang-format on
}

asm void fn_80215308(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceMotHookSet
#endif // clang-format on
}

asm void fn_8021530C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceObjHit
#endif // clang-format on
}

asm void fn_80215310(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceSNpcNumObjCreate
#endif // clang-format on
}

asm void fn_80215314(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceSNpcNumKill
#endif // clang-format on
}

asm void fn_80215318(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceSNpcNumDispSet
#endif // clang-format on
}

asm void fn_8021531C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceSNpcNumSet
#endif // clang-format on
}

asm void fn_80215320(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceSNpcNumPosSet
#endif // clang-format on
}

asm void fn_80215324(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceSNpcNumOfsSet
#endif // clang-format on
}

asm void fn_80215328(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDiceSNpcNumOfsGet
#endif // clang-format on
}

asm void fn_8021532C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbEffInit
#endif // clang-format on
}

asm void fn_80215330(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbEffClose
#endif // clang-format on
}

asm void fn_80215334(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbEffConfettiCreate
#endif // clang-format on
}

asm void fn_80215338(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbEffConfettiKill
#endif // clang-format on
}

asm void fn_8021533C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbEffConfettiReset
#endif // clang-format on
}

asm void fn_80215340(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbEffFadeOutSet
#endif // clang-format on
}

asm void fn_80215344(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbEffFadeCreate
#endif // clang-format on
}

asm void fn_80215348(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbEffFadeDoneCheck
#endif // clang-format on
}

asm void fn_8021534C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbEffFadeCheck
#endif // clang-format on
}

asm void fn_80215350(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbEffFadeCameraSet
#endif // clang-format on
}

asm void fn_80215354(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParticleCreate
#endif // clang-format on
}

asm void fn_80215358(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParticleKill
#endif // clang-format on
}

asm void fn_8021535C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParticleHookSet
#endif // clang-format on
}

asm void fn_80215360(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParticleAttrSet
#endif // clang-format on
}

asm void fn_80215364(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParticleAttrReset
#endif // clang-format on
}

asm void fn_80215368(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParticleUnkTotalGet
#endif // clang-format on
}

asm void fn_8021536C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParticleDataCreate
#endif // clang-format on
}

asm void fn_80215370(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParticleColorCreate
#endif // clang-format on
}

asm void fn_80215374(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParManCreate
#endif // clang-format on
}

asm void fn_80215378(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParManKill
#endif // clang-format on
}

asm void fn_8021537C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParManPosSet
#endif // clang-format on
}

asm void fn_80215380(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParManVecSet
#endif // clang-format on
}

asm void fn_80215384(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParManRotSet
#endif // clang-format on
}

asm void fn_80215388(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParManAttrSet
#endif // clang-format on
}

asm void fn_8021538C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParManAttrReset
#endif // clang-format on
}

asm void fn_80215390(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbParticleBlendModeSet
#endif // clang-format on
}

asm void fn_80215394(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGateInit
#endif // clang-format on
}

asm void fn_80215398(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGateClose
#endif // clang-format on
}

asm void fn_8021539C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGateCreate
#endif // clang-format on
}

asm void fn_802153A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_GateMasu
#endif // clang-format on
}

asm void fn_802153A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_GateBattanMasuFind
#endif // clang-format on
}

asm void fn_802153A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_GateBattan
#endif // clang-format on
}

asm void fn_802153AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGateBattanKill
#endif // clang-format on
}

asm void fn_802153B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideInit
#endif // clang-format on
}

asm void fn_802153B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideCreateIn
#endif // clang-format on
}

asm void fn_802153B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideCreateFlag
#endif // clang-format on
}

asm void fn_802153BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideCreate
#endif // clang-format on
}

asm void fn_802153C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideKill
#endif // clang-format on
}

asm void fn_802153C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideEnd
#endif // clang-format on
}

asm void fn_802153C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideFadeIn
#endif // clang-format on
}

asm void fn_802153CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideFadeOut
#endif // clang-format on
}

asm void fn_802153D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideModelGet
#endif // clang-format on
}

asm void fn_802153D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideIdleCheck
#endif // clang-format on
}

asm void fn_802153D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideScreenSet
#endif // clang-format on
}

asm void fn_802153DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideMotionNextSet
#endif // clang-format on
}

asm void fn_802153E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideMotionSet
#endif // clang-format on
}

asm void fn_802153E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideMotionShiftSet
#endif // clang-format on
}

asm void fn_802153E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideMotionStop
#endif // clang-format on
}

asm void fn_802153EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideMotionCheck
#endif // clang-format on
}

asm void fn_802153F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideNoGet
#endif // clang-format on
}

asm void fn_802153F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbGuideSpeakerNoGet
#endif // clang-format on
}

asm void fn_802153F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbExitWatchCreate
#endif // clang-format on
}

asm void fn_802153FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbExitReq
#endif // clang-format on
}

asm void fn_80215400(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbExitCheck
#endif // clang-format on
}

asm void fn_80215404(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbExitWatchProcGet
#endif // clang-format on
}

asm void fn_80215408(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbChangeTimeSet
#endif // clang-format on
}

asm void fn_8021540C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbChangeTime
#endif // clang-format on
}

asm void fn_80215410(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_NextTimeSet
#endif // clang-format on
}

asm void fn_80215414(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_LoadTimeSet
#endif // clang-format on
}

asm void fn_80215418(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjectSetup
#endif // clang-format on
}

asm void fn_8021541C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseEnableSet
#endif // clang-format on
}

asm void fn_80215420(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseEnableReset
#endif // clang-format on
}

asm void fn_80215424(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseEnableCheck
#endif // clang-format on
}

asm void fn_80215428(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbLightSet
#endif // clang-format on
}

asm void fn_8021542C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbLightReset
#endif // clang-format on
}

asm void fn_80215430(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbLightFuncSet
#endif // clang-format on
}

asm void fn_80215434(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBGRead
#endif // clang-format on
}

asm void fn_80215438(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBGReadWait
#endif // clang-format on
}

asm void fn_8021543C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbReturnMgCheck
#endif // clang-format on
}

asm void fn_80215440(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbOvlCall
#endif // clang-format on
}

asm void fn_80215444(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbDirClose
#endif // clang-format on
}

asm void fn_80215448(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_TurnStartSet
#endif // clang-format on
}

asm void fn_8021544C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_TurnEndSet
#endif // clang-format on
}

asm void fn_80215450(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8014C3B4
#endif // clang-format on
}

asm void fn_80215454(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8014C3BC
#endif // clang-format on
}

asm void fn_80215458(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8014C3C4
#endif // clang-format on
}

asm void fn_8021545C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b fn_8014C3CC
#endif // clang-format on
}

asm void fn_80215460(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbScrollInit
#endif // clang-format on
}

asm void fn_80215464(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbScrollClose
#endif // clang-format on
}

asm void fn_80215468(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_Scroll
#endif // clang-format on
}

asm void fn_8021546C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_ScrollCapsule
#endif // clang-format on
}

asm void fn_80215470(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbScrollStarFindFuncSet
#endif // clang-format on
}

asm void fn_80215474(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbScrollHookSet
#endif // clang-format on
}

asm void fn_80215478(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMapCameraSet
#endif // clang-format on
}

asm void fn_8021547C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMapHookSet
#endif // clang-format on
}

asm void fn_80215480(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMapSprAdd
#endif // clang-format on
}

asm void fn_80215484(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_StarScroll
#endif // clang-format on
}

asm void fn_80215488(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuInit
#endif // clang-format on
}

asm void fn_8021548C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuClose
#endif // clang-format on
}

asm void fn_80215490(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuStop
#endif // clang-format on
}

asm void fn_80215494(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuDataRead
#endif // clang-format on
}

asm void fn_80215498(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuMasuEnd
#endif // clang-format on
}

asm void fn_8021549C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuMasuStart
#endif // clang-format on
}

asm void fn_802154A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuCapStop
#endif // clang-format on
}

asm void fn_802154A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuMove
#endif // clang-format on
}

asm void fn_802154A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuNextSet
#endif // clang-format on
}

asm void fn_802154AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuNextDispSet
#endif // clang-format on
}

asm void fn_802154B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuNumGet
#endif // clang-format on
}

asm void fn_802154B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuRawNumGet
#endif // clang-format on
}

asm void fn_802154B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuGet
#endif // clang-format on
}

asm void fn_802154BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuLayerSet
#endif // clang-format on
}

asm void fn_802154C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuLayerGet
#endif // clang-format on
}

asm void fn_802154C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuAttrGet
#endif // clang-format on
}

asm void fn_802154C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuAttrSet
#endif // clang-format on
}

asm void fn_802154CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuMAttrGet
#endif // clang-format on
}

asm void fn_802154D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuMAttrSet
#endif // clang-format on
}

asm void fn_802154D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuBitGet
#endif // clang-format on
}

asm void fn_802154D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuAttrGet
#endif // clang-format on
}

asm void fn_802154DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuTypeGet
#endif // clang-format on
}

asm void fn_802154E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuTypeSet
#endif // clang-format on
}

asm void fn_802154E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuTypeChange
#endif // clang-format on
}

asm void fn_802154E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuDispCheck
#endif // clang-format on
}

asm void fn_802154EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuCapsuleReset
#endif // clang-format on
}

asm void fn_802154F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuCapsuleSet
#endif // clang-format on
}

asm void fn_802154F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuCapsuleGet
#endif // clang-format on
}

asm void fn_802154F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuPosGet
#endif // clang-format on
}

asm void fn_802154FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuPosSet
#endif // clang-format on
}

asm void fn_80215500(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuPosSetV
#endif // clang-format on
}

asm void fn_80215504(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuCornerRotPosGet
#endif // clang-format on
}

asm void fn_80215508(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuCornerPosGet
#endif // clang-format on
}

asm void fn_8021550C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuRotGet
#endif // clang-format on
}

asm void fn_80215510(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuRotSet
#endif // clang-format on
}

asm void fn_80215514(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuRotSetV
#endif // clang-format on
}

asm void fn_80215518(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuMtxGet
#endif // clang-format on
}

asm void fn_8021551C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuMtxSet
#endif // clang-format on
}

asm void fn_80215520(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuDispGet
#endif // clang-format on
}

asm void fn_80215524(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuDispSet
#endif // clang-format on
}

asm void fn_80215528(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuDispAttrReset
#endif // clang-format on
}

asm void fn_8021552C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuDispAttrSet
#endif // clang-format on
}

asm void fn_80215530(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuDispAttrGet
#endif // clang-format on
}

asm void fn_80215534(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuDispMAttrReset
#endif // clang-format on
}

asm void fn_80215538(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuDispMAttrSet
#endif // clang-format on
}

asm void fn_8021553C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuDispMAttrGet
#endif // clang-format on
}

asm void fn_80215540(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuModelDispSet
#endif // clang-format on
}

asm void fn_80215544(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuLinkGet
#endif // clang-format on
}

asm void fn_80215548(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuLinkNumGet
#endif // clang-format on
}

asm void fn_8021554C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuLinkTblGet
#endif // clang-format on
}

asm void fn_80215550(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuLinkTblGet2
#endif // clang-format on
}

asm void fn_80215554(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuAttrFindLink
#endif // clang-format on
}

asm void fn_80215558(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuAttrMatchFindLink
#endif // clang-format on
}

asm void fn_8021555C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuMAttrFindLink
#endif // clang-format on
}

asm void fn_80215560(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuMAttrMatchFindLink
#endif // clang-format on
}

asm void fn_80215564(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuTypeFindLink
#endif // clang-format on
}

asm void fn_80215568(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuLinkParentGet
#endif // clang-format on
}

asm void fn_8021556C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_TypeStepGet
#endif // clang-format on
}

asm void fn_80215570(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_TypeStepGet2
#endif // clang-format on
}

asm void fn_80215574(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_AttrStepGet
#endif // clang-format on
}

asm void fn_80215578(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_AttrMatchStepGet
#endif // clang-format on
}

asm void fn_8021557C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_AttrStepGet2
#endif // clang-format on
}

asm void fn_80215580(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_MAttrStepGet
#endif // clang-format on
}

asm void fn_80215584(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_MAttrMatchStepGet
#endif // clang-format on
}

asm void fn_80215588(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_MAttrStepGet2
#endif // clang-format on
}

asm void fn_8021558C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_IdStepGet
#endif // clang-format on
}

asm void fn_80215590(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_IdStepGet2
#endif // clang-format on
}

asm void fn_80215594(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_TypeSearch
#endif // clang-format on
}

asm void fn_80215598(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_TypeIdGet
#endif // clang-format on
}

asm void fn_8021559C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_AttrIdGet
#endif // clang-format on
}

asm void fn_802155A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_AttrMatchIdGet
#endif // clang-format on
}

asm void fn_802155A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_AttrMatchIdGet2
#endif // clang-format on
}

asm void fn_802155A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_MAttrIdGet
#endif // clang-format on
}

asm void fn_802155AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_MAttrMatchIdGet
#endif // clang-format on
}

asm void fn_802155B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_MAttrMatchIdGet2
#endif // clang-format on
}

asm void fn_802155B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuTypeListGet
#endif // clang-format on
}

asm void fn_802155B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuAttrListGet
#endif // clang-format on
}

asm void fn_802155BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuAttrMatchListGet
#endif // clang-format on
}

asm void fn_802155C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuMAttrListGet
#endif // clang-format on
}

asm void fn_802155C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuMAttrMatchTblGet
#endif // clang-format on
}

asm void fn_802155C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_TypeListGet
#endif // clang-format on
}

asm void fn_802155CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_TypeListGet2
#endif // clang-format on
}

asm void fn_802155D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_AttrNumGet
#endif // clang-format on
}

asm void fn_802155D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_AttrMatchNumGet
#endif // clang-format on
}

asm void fn_802155D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_AttrMatchNumGet2
#endif // clang-format on
}

asm void fn_802155DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_MAttrNumGet
#endif // clang-format on
}

asm void fn_802155E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_MAttrMatchListGet
#endif // clang-format on
}

asm void fn_802155E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_MAttrMatchListGet2
#endif // clang-format on
}

asm void fn_802155E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_IdListGet
#endif // clang-format on
}

asm void fn_802155EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuFind_IdListGet2
#endif // clang-format on
}

asm void fn_802155F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuMoveEndSet
#endif // clang-format on
}

asm void fn_802155F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuHatenaSet
#endif // clang-format on
}

asm void fn_802155F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuMoveStartSet
#endif // clang-format on
}

asm void fn_802155FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MasuLinkTblHookSet
#endif // clang-format on
}

asm void fn_80215600(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuPlayerDispSet
#endif // clang-format on
}

asm void fn_80215604(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuPlayerFadeSet
#endif // clang-format on
}

asm void fn_80215608(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuPlayerCapMoveCheck
#endif // clang-format on
}

asm void fn_8021560C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuPlayerCapStopCheck
#endif // clang-format on
}

asm void fn_80215610(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuStub
#endif // clang-format on
}

asm void fn_80215614(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuPlayerColorSet
#endif // clang-format on
}

asm void fn_80215618(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuPlayerPrizeReset
#endif // clang-format on
}

asm void fn_8021561C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuPKinokoValueGet
#endif // clang-format on
}

asm void fn_80215620(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMathInit
#endif // clang-format on
}

asm void fn_80215624(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMathClose
#endif // clang-format on
}

asm void fn_80215628(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbRandMod
#endif // clang-format on
}

asm void fn_8021562C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbVecMagCheck
#endif // clang-format on
}

asm void fn_80215630(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAngleAdd
#endif // clang-format on
}

asm void fn_80215634(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAngleMoveTo
#endif // clang-format on
}

asm void fn_80215638(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAngleWrap2
#endif // clang-format on
}

asm void fn_8021563C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbVecMagXZ
#endif // clang-format on
}

asm void fn_80215640(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxLookAtCalc
#endif // clang-format on
}

asm void fn_80215644(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAngleWrapV
#endif // clang-format on
}

asm void fn_80215648(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAngleWrap
#endif // clang-format on
}

asm void fn_8021564C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbVecMagXZCheck
#endif // clang-format on
}

asm void fn_80215650(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBezierCalc
#endif // clang-format on
}

asm void fn_80215654(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBezierCalcV
#endif // clang-format on
}

asm void fn_80215658(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBezierCalcVList
#endif // clang-format on
}

asm void fn_8021565C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBezierCalcSlope
#endif // clang-format on
}

asm void fn_80215660(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBezierCalcSlopeV
#endif // clang-format on
}

asm void fn_80215664(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbHermiteCalc
#endif // clang-format on
}

asm void fn_80215668(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbHermiteCalcV
#endif // clang-format on
}

asm void fn_8021566C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbHermiteCalcSlope
#endif // clang-format on
}

asm void fn_80215670(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPos3Dto2D
#endif // clang-format on
}

asm void fn_80215674(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPos2Dto3D
#endif // clang-format on
}

asm void fn_80215678(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPos3DtoNorm
#endif // clang-format on
}

asm void fn_8021567C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbNormPosto3D
#endif // clang-format on
}

asm void fn_80215680(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbNormPosto2D
#endif // clang-format on
}

asm void fn_80215684(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAngleLerp
#endif // clang-format on
}

asm void fn_80215688(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAngleEaseOut
#endif // clang-format on
}

asm void fn_8021568C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAngleEaseIn
#endif // clang-format on
}

asm void fn_80215690(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMathDistScale
#endif // clang-format on
}

asm void fn_80215694(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCosDeg
#endif // clang-format on
}

asm void fn_80215698(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSinDeg
#endif // clang-format on
}

asm void fn_8021569C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbCosRad
#endif // clang-format on
}

asm void fn_802156A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSinRad
#endif // clang-format on
}

asm void fn_802156A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRot
#endif // clang-format on
}

asm void fn_802156A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotAxisDeg
#endif // clang-format on
}

asm void fn_802156AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotAxisRad
#endif // clang-format on
}

asm void fn_802156B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotXDeg
#endif // clang-format on
}

asm void fn_802156B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotXRad
#endif // clang-format on
}

asm void fn_802156B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotYDeg
#endif // clang-format on
}

asm void fn_802156BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotYRad
#endif // clang-format on
}

asm void fn_802156C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotZDeg
#endif // clang-format on
}

asm void fn_802156C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotZRad
#endif // clang-format on
}

asm void fn_802156C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxScaleRotXDeg
#endif // clang-format on
}

asm void fn_802156CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxScaleRotYDeg
#endif // clang-format on
}

asm void fn_802156D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxScaleRotZDeg
#endif // clang-format on
}

asm void fn_802156D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotTrigX
#endif // clang-format on
}

asm void fn_802156D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotTrigY
#endif // clang-format on
}

asm void fn_802156DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotTrigZ
#endif // clang-format on
}

asm void fn_802156E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotTrigScaleX
#endif // clang-format on
}

asm void fn_802156E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotTrigScaleY
#endif // clang-format on
}

asm void fn_802156E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxRotTrigScaleZ
#endif // clang-format on
}

asm void fn_802156EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMtxTransCat
#endif // clang-format on
}

asm void fn_802156F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjCullInit
#endif // clang-format on
}

asm void fn_802156F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopPlayerCreate
#endif // clang-format on
}

asm void fn_802156F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopPlayerSkipCreate
#endif // clang-format on
}

asm void fn_802156FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopCreate
#endif // clang-format on
}

asm void fn_80215700(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopCheck
#endif // clang-format on
}

asm void fn_80215704(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopLastTurnCreate
#endif // clang-format on
}

asm void fn_80215708(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPadStkXGet
#endif // clang-format on
}

asm void fn_8021570C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPadStkYGet
#endif // clang-format on
}

asm void fn_80215710(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTauntInit
#endif // clang-format on
}

asm void fn_80215714(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTauntClose
#endif // clang-format on
}

asm void fn_80215718(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbLanguageGet
#endif // clang-format on
}

asm void fn_8021571C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbLanguageSet
#endif // clang-format on
}

asm void fn_80215720(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBoardDataNumGet
#endif // clang-format on
}

asm void fn_80215724(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbBoardDataDirRead
#endif // clang-format on
}

asm void fn_80215728(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopTimeChangeCreate
#endif // clang-format on
}

asm void fn_8021572C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopTimeChangeCheck
#endif // clang-format on
}

asm void fn_80215730(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopTimeChangeKill
#endif // clang-format on
}

asm void fn_80215734(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopTimeCreate
#endif // clang-format on
}

asm void fn_80215738(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopTimeSprCreate
#endif // clang-format on
}

asm void fn_8021573C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopTimeSprKill
#endif // clang-format on
}

asm void fn_80215740(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopTimeDispSet
#endif // clang-format on
}

asm void fn_80215744(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopTimeSprRotSet
#endif // clang-format on
}

asm void fn_80215748(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopTimeStarSet
#endif // clang-format on
}

asm void fn_8021574C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTelopTimeTPLvlSet
#endif // clang-format on
}

asm void fn_80215750(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjInit
#endif // clang-format on
}

asm void fn_80215754(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjClose
#endif // clang-format on
}

asm void fn_80215758(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjKill
#endif // clang-format on
}

asm void fn_8021575C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjDirSet
#endif // clang-format on
}

asm void fn_80215760(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjDataNumGet
#endif // clang-format on
}

asm void fn_80215764(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjCreate
#endif // clang-format on
}

asm void fn_80215768(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjCharCreate
#endif // clang-format on
}

asm void fn_8021576C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjGet
#endif // clang-format on
}

asm void fn_80215770(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjDispGet
#endif // clang-format on
}

asm void fn_80215774(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjDispSet
#endif // clang-format on
}

asm void fn_80215778(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjCameraSet
#endif // clang-format on
}

asm void fn_8021577C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjLayerSet
#endif // clang-format on
}

asm void fn_80215780(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjLayerGet
#endif // clang-format on
}

asm void fn_80215784(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjLayerSetAll
#endif // clang-format on
}

asm void fn_80215788(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjModelIDGet
#endif // clang-format on
}

asm void fn_8021578C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjPosGet
#endif // clang-format on
}

asm void fn_80215790(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjPosSetV
#endif // clang-format on
}

asm void fn_80215794(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjPosSet
#endif // clang-format on
}

asm void fn_80215798(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjOffsetGet
#endif // clang-format on
}

asm void fn_8021579C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjOffsetSetV
#endif // clang-format on
}

asm void fn_802157A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjOffsetSet
#endif // clang-format on
}

asm void fn_802157A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjRotGet
#endif // clang-format on
}

asm void fn_802157A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjRotSetV
#endif // clang-format on
}

asm void fn_802157AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjRotSet
#endif // clang-format on
}

asm void fn_802157B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjRotYGet
#endif // clang-format on
}

asm void fn_802157B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjRotYSet
#endif // clang-format on
}

asm void fn_802157B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjScaleGet
#endif // clang-format on
}

asm void fn_802157BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjScaleSetV
#endif // clang-format on
}

asm void fn_802157C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjScaleSet
#endif // clang-format on
}

asm void fn_802157C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMtxGet
#endif // clang-format on
}

asm void fn_802157C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMtxSet
#endif // clang-format on
}

asm void fn_802157CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjAttrSet
#endif // clang-format on
}

asm void fn_802157D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjAttrReset
#endif // clang-format on
}

asm void fn_802157D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjAmbSet
#endif // clang-format on
}

asm void fn_802157D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjAmbSetAll
#endif // clang-format on
}

asm void fn_802157DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjAlphaGet
#endif // clang-format on
}

asm void fn_802157E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjAlphaSet
#endif // clang-format on
}

asm void fn_802157E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjFadeSet
#endif // clang-format on
}

asm void fn_802157E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjZWriteOffSet
#endif // clang-format on
}

asm void fn_802157EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjSePlay
#endif // clang-format on
}

asm void fn_802157F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionVoiceOnSet
#endif // clang-format on
}

asm void fn_802157F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjCullRadiusSet
#endif // clang-format on
}

asm void fn_802157F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjHookSet
#endif // clang-format on
}

asm void fn_802157FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjHookReset
#endif // clang-format on
}

asm void fn_80215800(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjHookObjReset
#endif // clang-format on
}

asm void fn_80215804(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionGet
#endif // clang-format on
}

asm void fn_80215808(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionSet
#endif // clang-format on
}

asm void fn_8021580C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionIDCurGet
#endif // clang-format on
}

asm void fn_80215810(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionIDGet
#endif // clang-format on
}

asm void fn_80215814(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionShiftSet
#endif // clang-format on
}

asm void fn_80215818(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionShiftIDGet
#endif // clang-format on
}

asm void fn_8021581C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionNoCreate
#endif // clang-format on
}

asm void fn_80215820(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionCreate
#endif // clang-format on
}

asm void fn_80215824(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionKill
#endif // clang-format on
}

asm void fn_80215828(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionTimeGet
#endif // clang-format on
}

asm void fn_8021582C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionTimeSet
#endif // clang-format on
}

asm void fn_80215830(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionMaxTimeGet
#endif // clang-format on
}

asm void fn_80215834(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionSpeedGet
#endif // clang-format on
}

asm void fn_80215838(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionSpeedSet
#endif // clang-format on
}

asm void fn_8021583C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionEndCheck
#endif // clang-format on
}

asm void fn_80215840(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionStartEndSet
#endif // clang-format on
}

asm void fn_80215844(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionLoopSet
#endif // clang-format on
}

asm void fn_80215848(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionShapeSet
#endif // clang-format on
}

asm void fn_8021584C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionShapeTimeGet
#endif // clang-format on
}

asm void fn_80215850(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionShapeTimeSet
#endif // clang-format on
}

asm void fn_80215854(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionShapeMaxTimeGet
#endif // clang-format on
}

asm void fn_80215858(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionShapeSpeedGet
#endif // clang-format on
}

asm void fn_8021585C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionShapeSpeedSet
#endif // clang-format on
}

asm void fn_80215860(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionShapeEndCheck
#endif // clang-format on
}

asm void fn_80215864(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMotionShapeStartEndSet
#endif // clang-format on
}

asm void fn_80215868(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjShadowSet
#endif // clang-format on
}

asm void fn_8021586C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjShadowReset
#endif // clang-format on
}

asm void fn_80215870(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjShadowMapSet
#endif // clang-format on
}

asm void fn_80215874(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjShadowMapReset
#endif // clang-format on
}

asm void fn_80215878(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjStub0
#endif // clang-format on
}

asm void fn_8021587C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_OpeningParty
#endif // clang-format on
}

asm void fn_80215880(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_OpeningSingle
#endif // clang-format on
}

asm void fn_80215884(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_Opening
#endif // clang-format on
}

asm void fn_80215888(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbOpeningViewSet
#endif // clang-format on
}

asm void fn_8021588C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbOpeningZoomGet
#endif // clang-format on
}

asm void fn_80215890(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbOpeningRotGet
#endif // clang-format on
}

asm void fn_80215894(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbOpeningPosGet
#endif // clang-format on
}

asm void fn_80215898(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbOpeningInstHookSet
#endif // clang-format on
}

asm void fn_8021589C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbOpeningStarInstHookSet
#endif // clang-format on
}

asm void fn_802158A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbOpeningGuideObjIdGet
#endif // clang-format on
}

asm void fn_802158A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbOpeningGuidePosRestore
#endif // clang-format on
}

asm void fn_802158A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbOpeningCameraPosRestore
#endif // clang-format on
}

asm void fn_802158AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseInit
#endif // clang-format on
}

asm void fn_802158B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseCreate
#endif // clang-format on
}

asm void fn_802158B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseStartCheck
#endif // clang-format on
}

asm void fn_802158B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseProcCheck
#endif // clang-format on
}

asm void fn_802158BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseDisableSet
#endif // clang-format on
}

asm void fn_802158C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseDisableGet
#endif // clang-format on
}

asm void fn_802158C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseHookPush
#endif // clang-format on
}

asm void fn_802158C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseHookPop
#endif // clang-format on
}

asm void fn_802158CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseSet
#endif // clang-format on
}

asm void fn_802158D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseDataDirGet
#endif // clang-format on
}

asm void fn_802158D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPauseDataNumGet
#endif // clang-format on
}

asm void fn_802158D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMetalSet
#endif // clang-format on
}

asm void fn_802158DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerEffectSet
#endif // clang-format on
}

asm void fn_802158E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMetalColorSet
#endif // clang-format on
}

asm void fn_802158E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerBiriQSet
#endif // clang-format on
}

asm void fn_802158E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerInit
#endif // clang-format on
}

asm void fn_802158EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerClose
#endif // clang-format on
}

asm void fn_802158F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTurnExec
#endif // clang-format on
}

asm void fn_802158F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleTurnExec
#endif // clang-format on
}

asm void fn_802158F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerSwap
#endif // clang-format on
}

asm void fn_802158FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerNameMesGet
#endif // clang-format on
}

asm void fn_80215900(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerNameGet
#endif // clang-format on
}

asm void fn_80215904(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTagNameMesGet
#endif // clang-format on
}

asm void fn_80215908(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerColorGet
#endif // clang-format on
}

asm void fn_8021590C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerAmbSet
#endif // clang-format on
}

asm void fn_80215910(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerWorkGet
#endif // clang-format on
}

asm void fn_80215914(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerAllComCheck
#endif // clang-format on
}

asm void fn_80215918(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerObjIDGet
#endif // clang-format on
}

asm void fn_8021591C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerModelIDGet
#endif // clang-format on
}

asm void fn_80215920(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerDispSet
#endif // clang-format on
}

asm void fn_80215924(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerDispGet
#endif // clang-format on
}

asm void fn_80215928(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerLayerSet
#endif // clang-format on
}

asm void fn_8021592C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCameraSet
#endif // clang-format on
}

asm void fn_80215930(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCullRadiusSet
#endif // clang-format on
}

asm void fn_80215934(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerStubValSet
#endif // clang-format on
}

asm void fn_80215938(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMtxSet
#endif // clang-format on
}

asm void fn_8021593C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMtxGet
#endif // clang-format on
}

asm void fn_80215940(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerPosSet
#endif // clang-format on
}

asm void fn_80215944(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerPosSetV
#endif // clang-format on
}

asm void fn_80215948(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerPosGet
#endif // clang-format on
}

asm void fn_8021594C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerRotSet
#endif // clang-format on
}

asm void fn_80215950(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerRotSetV
#endif // clang-format on
}

asm void fn_80215954(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerRotGet
#endif // clang-format on
}

asm void fn_80215958(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerRotYSet
#endif // clang-format on
}

asm void fn_8021595C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerRotYGet
#endif // clang-format on
}

asm void fn_80215960(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerScaleSet
#endif // clang-format on
}

asm void fn_80215964(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerScaleSetV
#endif // clang-format on
}

asm void fn_80215968(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerScaleGet
#endif // clang-format on
}

asm void fn_8021596C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionGet
#endif // clang-format on
}

asm void fn_80215970(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionEndCheck
#endif // clang-format on
}

asm void fn_80215974(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionEndCheckAll
#endif // clang-format on
}

asm void fn_80215978(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionEndWait
#endif // clang-format on
}

asm void fn_8021597C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionMaxTimeGet
#endif // clang-format on
}

asm void fn_80215980(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionTimeGet
#endif // clang-format on
}

asm void fn_80215984(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionSet
#endif // clang-format on
}

asm void fn_80215988(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionVoiceOnSet
#endif // clang-format on
}

asm void fn_8021598C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionShiftSet
#endif // clang-format on
}

asm void fn_80215990(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionCreate
#endif // clang-format on
}

asm void fn_80215994(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionKill
#endif // clang-format on
}

asm void fn_80215998(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionSpeedSet
#endif // clang-format on
}

asm void fn_8021599C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionTimeSet
#endif // clang-format on
}

asm void fn_802159A0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotionStartEndSet
#endif // clang-format on
}

asm void fn_802159A4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerAttrSet
#endif // clang-format on
}

asm void fn_802159A8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerAttrReset
#endif // clang-format on
}

asm void fn_802159AC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMotIdleSet
#endif // clang-format on
}

asm void fn_802159B0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerRotateStart
#endif // clang-format on
}

asm void fn_802159B4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerRotateCheck
#endif // clang-format on
}

asm void fn_802159B8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerRotateCheckAll
#endif // clang-format on
}

asm void fn_802159BC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerEyeMatDarkSet
#endif // clang-format on
}

asm void fn_802159C0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerRankGet
#endif // clang-format on
}

asm void fn_802159C4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCapsuleUseGet
#endif // clang-format on
}

asm void fn_802159C8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCapsuleUseSet
#endif // clang-format on
}

asm void fn_802159CC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCapsuleRemove
#endif // clang-format on
}

asm void fn_802159D0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCapsuleFind
#endif // clang-format on
}

asm void fn_802159D4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCapsuleAdd
#endif // clang-format on
}

asm void fn_802159D8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCapsuleNumGet
#endif // clang-format on
}

asm void fn_802159DC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCapsuleGet
#endif // clang-format on
}

asm void fn_802159E0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCapsuleMaxGet
#endif // clang-format on
}

asm void fn_802159E4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCoinAdd
#endif // clang-format on
}

asm void fn_802159E8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCoinSet
#endif // clang-format on
}

asm void fn_802159EC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCoinGet
#endif // clang-format on
}

asm void fn_802159F0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMaxCoinGet
#endif // clang-format on
}

asm void fn_802159F4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerStarAdd
#endif // clang-format on
}

asm void fn_802159F8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerStarSet
#endif // clang-format on
}

asm void fn_802159FC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerStarGet
#endif // clang-format on
}

asm void fn_80215A00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamFind
#endif // clang-format on
}

asm void fn_80215A04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamFindOther
#endif // clang-format on
}

asm void fn_80215A08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamCoinSet
#endif // clang-format on
}

asm void fn_80215A0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamCoinGet
#endif // clang-format on
}

asm void fn_80215A10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerGrpStarSet
#endif // clang-format on
}

asm void fn_80215A14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerGrpStarGet
#endif // clang-format on
}

asm void fn_80215A18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamRankGet
#endif // clang-format on
}

asm void fn_80215A1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamFindPlayer
#endif // clang-format on
}

asm void fn_80215A20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamCapsuleGet
#endif // clang-format on
}

asm void fn_80215A24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamCapsuleNumGet
#endif // clang-format on
}

asm void fn_80215A28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamFindOpp
#endif // clang-format on
}

asm void fn_80215A2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamGet
#endif // clang-format on
}

asm void fn_80215A30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTeamCheckSame
#endif // clang-format on
}

asm void fn_80215A34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerWalkSpeedGet
#endif // clang-format on
}

asm void fn_80215A38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMoveExec
#endif // clang-format on
}

asm void fn_80215A3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMoveMain
#endif // clang-format on
}

asm void fn_80215A40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_PlayerColReserve
#endif // clang-format on
}

asm void fn_80215A44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_PlayerColSet
#endif // clang-format on
}

asm void fn_80215A48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMoveNumCreateColor
#endif // clang-format on
}

asm void fn_80215A4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMoveNumCreate
#endif // clang-format on
}

asm void fn_80215A50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMoveNumKill
#endif // clang-format on
}

asm void fn_80215A54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMoveNumDispSet
#endif // clang-format on
}

asm void fn_80215A58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMasuMoveTo
#endif // clang-format on
}

asm void fn_80215A5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMasuMove
#endif // clang-format on
}

asm void fn_80215A60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMasuMovePos
#endif // clang-format on
}

asm void fn_80215A64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMasuMoveSpeed
#endif // clang-format on
}

asm void fn_80215A68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerColSnapSet
#endif // clang-format on
}

asm void fn_80215A6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_PlayerColMasuSet
#endif // clang-format on
}

asm void fn_80215A70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_PlayerColMasuAllSet
#endif // clang-format on
}

asm void fn_80215A74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_PlayerColMasuAdd
#endif // clang-format on
}

asm void fn_80215A78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_PlayerColMasu
#endif // clang-format on
}

asm void fn_80215A7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_PlayerColCircleAdd
#endif // clang-format on
}

asm void fn_80215A80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_PlayerColBall
#endif // clang-format on
}

asm void fn_80215A84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerColSnapPlayerSet
#endif // clang-format on
}

asm void fn_80215A88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerColSnapGet
#endif // clang-format on
}

asm void fn_80215A8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerColRestSet
#endif // clang-format on
}

asm void fn_80215A90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerColCheck
#endif // clang-format on
}

asm void fn_80215A94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerBestPathGet
#endif // clang-format on
}

asm void fn_80215A98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerPosReset
#endif // clang-format on
}

asm void fn_80215A9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerPosResetAll
#endif // clang-format on
}

asm void fn_80215AA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMasuCornerSet
#endif // clang-format on
}

asm void fn_80215AA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMasuCornerGet
#endif // clang-format on
}

asm void fn_80215AA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerDiceMotExec
#endif // clang-format on
}

asm void fn_80215AAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerKettouCheck
#endif // clang-format on
}

asm void fn_80215AB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerBlackoutSet
#endif // clang-format on
}

asm void fn_80215AB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerBlackoutGet
#endif // clang-format on
}

asm void fn_80215AB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerWinLoseVoicePlay
#endif // clang-format on
}

asm void fn_80215ABC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerVoicePlay
#endif // clang-format on
}

asm void fn_80215AC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerVoicePanPlay
#endif // clang-format on
}

asm void fn_80215AC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerStartTurnHookSet
#endif // clang-format on
}

asm void fn_80215AC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerEndTurnHookSet
#endif // clang-format on
}

asm void fn_80215ACC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMoveHookSet
#endif // clang-format on
}

asm void fn_80215AD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerDiceTypeGet
#endif // clang-format on
}

asm void fn_80215AD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTurnInitHookSet
#endif // clang-format on
}

asm void fn_80215AD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerTurnCloseHookSet
#endif // clang-format on
}

asm void fn_80215ADC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerColFirstSet
#endif // clang-format on
}

asm void fn_80215AE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerColOrderReset
#endif // clang-format on
}

asm void fn_80215AE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerPlusMasuExec
#endif // clang-format on
}

asm void fn_80215AE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerMinusMasuExec
#endif // clang-format on
}

asm void fn_80215AEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbPlayerCapCoinMasuExec
#endif // clang-format on
}

asm void fn_80215AF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbRouletteCreate
#endif // clang-format on
}

asm void fn_80215AF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbRouletteKaneCreate
#endif // clang-format on
}

asm void fn_80215AF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbRouletteCheck
#endif // clang-format on
}

asm void fn_80215AFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbRouletteWait
#endif // clang-format on
}

asm void fn_80215B00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbRouletteResultGet
#endif // clang-format on
}

asm void fn_80215B04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbRouletteMaxSpeedSet
#endif // clang-format on
}

asm void fn_80215B08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbRouletteComValueSet
#endif // clang-format on
}

asm void fn_80215B0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleInit
#endif // clang-format on
}

asm void fn_80215B10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleClose
#endif // clang-format on
}

asm void fn_80215B14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleReturn
#endif // clang-format on
}

asm void fn_80215B18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleReturnWrite
#endif // clang-format on
}

asm void fn_80215B1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleGameEnd
#endif // clang-format on
}

asm void fn_80215B20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleSaveInit
#endif // clang-format on
}

asm void fn_80215B24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleMgUnlockWrite
#endif // clang-format on
}

asm void fn_80215B28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleMgUnlockInit
#endif // clang-format on
}

asm void fn_80215B2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleMgUnlockSet
#endif // clang-format on
}

asm void fn_80215B30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleMgUnlockReset
#endif // clang-format on
}

asm void fn_80215B34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleMgUnlockGet
#endif // clang-format on
}

asm void fn_80215B38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleMgUnlockCheckAny
#endif // clang-format on
}

asm void fn_80215B3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleMgUnlockNumGet
#endif // clang-format on
}

asm void fn_80215B40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_SingleMg
#endif // clang-format on
}

asm void fn_80215B44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_SingleMgEnd
#endif // clang-format on
}

asm void fn_80215B48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleStepGet
#endif // clang-format on
}

asm void fn_80215B4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleCall
#endif // clang-format on
}

asm void fn_80215B50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleOppCharGet
#endif // clang-format on
}

asm void fn_80215B54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleTeamCharSet
#endif // clang-format on
}

asm void fn_80215B58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleTeamCharGet
#endif // clang-format on
}

asm void fn_80215B5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleSaveFlush
#endif // clang-format on
}

asm void fn_80215B60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSingleMgUnlockCheck
#endif // clang-format on
}

asm void fn_80215B64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusPlay
#endif // clang-format on
}

asm void fn_80215B68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusFadeOut
#endif // clang-format on
}

asm void fn_80215B6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusFadeOutSpeed
#endif // clang-format on
}

asm void fn_80215B70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusStub
#endif // clang-format on
}

asm void fn_80215B74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusParamSet
#endif // clang-format on
}

asm void fn_80215B78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusPauseFadeOut
#endif // clang-format on
}

asm void fn_80215B7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusStatGet
#endif // clang-format on
}

asm void fn_80215B80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudClose
#endif // clang-format on
}

asm void fn_80215B84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudInit
#endif // clang-format on
}

asm void fn_80215B88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudStub1
#endif // clang-format on
}

asm void fn_80215B8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudStub2
#endif // clang-format on
}

asm void fn_80215B90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusBoardPlay
#endif // clang-format on
}

asm void fn_80215B94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusCheck
#endif // clang-format on
}

asm void fn_80215B98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusEndCheck
#endif // clang-format on
}

asm void fn_80215B9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusStopCheck
#endif // clang-format on
}

asm void fn_80215BA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusFadeCheck
#endif // clang-format on
}

asm void fn_80215BA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusPauseSet
#endif // clang-format on
}

asm void fn_80215BA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusJinglePlay
#endif // clang-format on
}

asm void fn_80215BAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusJingleWait
#endif // clang-format on
}

asm void fn_80215BB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMusJingleStatGet
#endif // clang-format on
}

asm void fn_80215BB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXObjSet
#endif // clang-format on
}

asm void fn_80215BB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXObjCreate
#endif // clang-format on
}

asm void fn_80215BBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXObjVolSet
#endif // clang-format on
}

asm void fn_80215BC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXObjKill
#endif // clang-format on
}

asm void fn_80215BC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXObjClose
#endif // clang-format on
}

asm void fn_80215BC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXObjEmitterSet
#endif // clang-format on
}

asm void fn_80215BCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXObjEmitterCreate
#endif // clang-format on
}

asm void fn_80215BD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXObjEmitterUpdate
#endif // clang-format on
}

asm void fn_80215BD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXObjEmiterPosSet
#endif // clang-format on
}

asm void fn_80215BD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXObjRefSet
#endif // clang-format on
}

asm void fn_80215BDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXPlay
#endif // clang-format on
}

asm void fn_80215BE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXStop
#endif // clang-format on
}

asm void fn_80215BE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXStopAll
#endif // clang-format on
}

asm void fn_80215BE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXPosPlay
#endif // clang-format on
}

asm void fn_80215BEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXPosPanGet
#endif // clang-format on
}

asm void fn_80215BF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXEmitterPlay
#endif // clang-format on
}

asm void fn_80215BF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXVolSet
#endif // clang-format on
}

asm void fn_80215BF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXPanning
#endif // clang-format on
}

asm void fn_80215BFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXPosPanning
#endif // clang-format on
}

asm void fn_80215C00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudFXDelaySet
#endif // clang-format on
}

asm void fn_80215C04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbAudGuidePlay
#endif // clang-format on
}

asm void fn_80215C08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_StarMasu
#endif // clang-format on
}

asm void fn_80215C0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_StarFreeMasu
#endif // clang-format on
}

asm void fn_80215C10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarGetMain
#endif // clang-format on
}

asm void fn_80215C14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarGetPosExec
#endif // clang-format on
}

asm void fn_80215C18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarMapViewExec
#endif // clang-format on
}

asm void fn_80215C1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarStub2
#endif // clang-format on
}

asm void fn_80215C20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarInit
#endif // clang-format on
}

asm void fn_80215C24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarClose
#endif // clang-format on
}

asm void fn_80215C28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarStub
#endif // clang-format on
}

asm void fn_80215C2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarNumSet
#endif // clang-format on
}

asm void fn_80215C30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarNumGet
#endif // clang-format on
}

asm void fn_80215C34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarFlagInit
#endif // clang-format on
}

asm void fn_80215C38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarFlagSet
#endif // clang-format on
}

asm void fn_80215C3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarFlagReset
#endif // clang-format on
}

asm void fn_80215C40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarFlagGet
#endif // clang-format on
}

asm void fn_80215C44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarNextNoSet
#endif // clang-format on
}

asm void fn_80215C48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarNextNoGet
#endif // clang-format on
}

asm void fn_80215C4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarNoSet
#endif // clang-format on
}

asm void fn_80215C50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarNoRandGet
#endif // clang-format on
}

asm void fn_80215C54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarReset
#endif // clang-format on
}

asm void fn_80215C58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarNoDispSet
#endif // clang-format on
}

asm void fn_80215C5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarMasuDispSet
#endif // clang-format on
}

asm void fn_80215C60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarDispSetAll
#endif // clang-format on
}

asm void fn_80215C64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjDispSetAll
#endif // clang-format on
}

asm void fn_80215C68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarMasuNextSet
#endif // clang-format on
}

asm void fn_80215C6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarGetExec
#endif // clang-format on
}

asm void fn_80215C70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarMoveHookSet
#endif // clang-format on
}

asm void fn_80215C74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarMasuFuncSet
#endif // clang-format on
}

asm void fn_80215C78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarAddNumSet
#endif // clang-format on
}

asm void fn_80215C7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarDispCreate
#endif // clang-format on
}

asm void fn_80215C80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarDispPlayerCreate
#endif // clang-format on
}

asm void fn_80215C84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarDispCheck
#endif // clang-format on
}

asm void fn_80215C88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjCreate
#endif // clang-format on
}

asm void fn_80215C8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjPosSet
#endif // clang-format on
}

asm void fn_80215C90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjRotSet
#endif // clang-format on
}

asm void fn_80215C94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjScaleSet
#endif // clang-format on
}

asm void fn_80215C98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjPosSetV
#endif // clang-format on
}

asm void fn_80215C9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjRotSetV
#endif // clang-format on
}

asm void fn_80215CA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjScaleSetV
#endif // clang-format on
}

asm void fn_80215CA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjPosGet
#endif // clang-format on
}

asm void fn_80215CA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjRotGet
#endif // clang-format on
}

asm void fn_80215CAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjScaleGet
#endif // clang-format on
}

asm void fn_80215CB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjDispSet
#endif // clang-format on
}

asm void fn_80215CB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjDispFlagSet
#endif // clang-format on
}

asm void fn_80215CB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarObjKill
#endif // clang-format on
}

asm void fn_80215CBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarChestCreate
#endif // clang-format on
}

asm void fn_80215CC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarAddProcExec
#endif // clang-format on
}

asm void fn_80215CC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarAddDispExec
#endif // clang-format on
}

asm void fn_80215CC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarAddExec
#endif // clang-format on
}

asm void fn_80215CCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarAddAllProcExecV
#endif // clang-format on
}

asm void fn_80215CD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarAddAllProcExec
#endif // clang-format on
}

asm void fn_80215CD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarAddAllExec
#endif // clang-format on
}

asm void fn_80215CD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbZtarObjInit
#endif // clang-format on
}

asm void fn_80215CDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbZtarObjClose
#endif // clang-format on
}

asm void fn_80215CE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbZtarMasuNextSet
#endif // clang-format on
}

asm void fn_80215CE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbZtarGetExec
#endif // clang-format on
}

asm void fn_80215CE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbZtarGetMain
#endif // clang-format on
}

asm void fn_80215CEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbZtarObjDispFlagSet
#endif // clang-format on
}

asm void fn_80215CF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStarMapViewProcExec
#endif // clang-format on
}

asm void fn_80215CF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusInit
#endif // clang-format on
}

asm void fn_80215CF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusClose
#endif // clang-format on
}

asm void fn_80215CFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusReset
#endif // clang-format on
}

asm void fn_80215D00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusDispGet
#endif // clang-format on
}

asm void fn_80215D04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusDispSet
#endif // clang-format on
}

asm void fn_80215D08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusDispFocusSet
#endif // clang-format on
}

asm void fn_80215D0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusDispSetAll
#endif // clang-format on
}

asm void fn_80215D10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusDispForceSet
#endif // clang-format on
}

asm void fn_80215D14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusDispForceSetAll
#endif // clang-format on
}

asm void fn_80215D18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusMoveCheck
#endif // clang-format on
}

asm void fn_80215D1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusOffCheckAll
#endif // clang-format on
}

asm void fn_80215D20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSprCreate
#endif // clang-format on
}

asm void fn_80215D24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusPosGet
#endif // clang-format on
}

asm void fn_80215D28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusNoPosGet
#endif // clang-format on
}

asm void fn_80215D2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusCapsuleDispSet
#endif // clang-format on
}

asm void fn_80215D30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusKaoNoSet
#endif // clang-format on
}

asm void fn_80215D34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSprNumSet
#endif // clang-format on
}

asm void fn_80215D38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusMoveTo
#endif // clang-format on
}

asm void fn_80215D3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusMoveSet
#endif // clang-format on
}

asm void fn_80215D40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusPosOnGet
#endif // clang-format on
}

asm void fn_80215D44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusPosOffGet
#endif // clang-format on
}

asm void fn_80215D48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusRainbowSet
#endif // clang-format on
}

asm void fn_80215D4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusColorSet
#endif // clang-format on
}

asm void fn_80215D50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusColorAllSet
#endif // clang-format on
}

asm void fn_80215D54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusColorGet
#endif // clang-format on
}

asm void fn_80215D58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusLayoutSet
#endif // clang-format on
}

asm void fn_80215D5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusCapsuleFocusSet
#endif // clang-format on
}

asm void fn_80215D60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusNoGet
#endif // clang-format on
}

asm void fn_80215D64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusNoMoveSet
#endif // clang-format on
}

asm void fn_80215D68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusDispBackup
#endif // clang-format on
}

asm void fn_80215D6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusDispRestoreForce
#endif // clang-format on
}

asm void fn_80215D70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusDispRestore
#endif // clang-format on
}

asm void fn_80215D74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbStatusMasuDispSet
#endif // clang-format on
}

asm void fn_80215D78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcInit
#endif // clang-format on
}

asm void fn_80215D7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcCreate
#endif // clang-format on
}

asm void fn_80215D80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcKill
#endif // clang-format on
}

asm void fn_80215D84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcMotReset
#endif // clang-format on
}

asm void fn_80215D88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcPlayerWalkSet
#endif // clang-format on
}

asm void fn_80215D8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcStarExec
#endif // clang-format on
}

asm void fn_80215D90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcMasuGet
#endif // clang-format on
}

asm void fn_80215D94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcMotWinSet
#endif // clang-format on
}

asm void fn_80215D98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcMotIdleSet
#endif // clang-format on
}

asm void fn_80215D9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcDispSet
#endif // clang-format on
}

asm void fn_80215DA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuChanceCreate
#endif // clang-format on
}

asm void fn_80215DA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuChanceTypeSet
#endif // clang-format on
}

asm void fn_80215DA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuChanceFlagSet
#endif // clang-format on
}

asm void fn_80215DAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuChancePlayerSet
#endif // clang-format on
}

asm void fn_80215DB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuChanceSet
#endif // clang-format on
}

asm void fn_80215DB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMasuChanceKill
#endif // clang-format on
}

asm void fn_80215DB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbSNpcMasuStarNextGet
#endif // clang-format on
}

asm void fn_80215DBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjStarTevStageSet
#endif // clang-format on
}

asm void fn_80215DC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjFadeCreate
#endif // clang-format on
}

asm void fn_80215DC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjFadeKill
#endif // clang-format on
}

asm void fn_80215DC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjFadeTexRotSet
#endif // clang-format on
}

asm void fn_80215DCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjFadeTexColorSet
#endif // clang-format on
}

asm void fn_80215DD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMetalCreate
#endif // clang-format on
}

asm void fn_80215DD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMetalKill
#endif // clang-format on
}

asm void fn_80215DD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMetalTPLvlSet
#endif // clang-format on
}

asm void fn_80215DDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjMetalColorSet
#endif // clang-format on
}

asm void fn_80215DE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjBiriQCreate
#endif // clang-format on
}

asm void fn_80215DE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjBiriQKill
#endif // clang-format on
}

asm void fn_80215DE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbObjBiriQColorSet
#endif // clang-format on
}

asm void fn_80215DEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialInit
#endif // clang-format on
}

asm void fn_80215DF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialCreate
#endif // clang-format on
}

asm void fn_80215DF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialExitReqGet
#endif // clang-format on
}

asm void fn_80215DF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialExitSet
#endif // clang-format on
}

asm void fn_80215DFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialExitOnSet
#endif // clang-format on
}

asm void fn_80215E00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialCall
#endif // clang-format on
}

asm void fn_80215E04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialMainFuncSet
#endif // clang-format on
}

asm void fn_80215E08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialSprCreate
#endif // clang-format on
}

asm void fn_80215E0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialSprDispOn
#endif // clang-format on
}

asm void fn_80215E10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialSprDispOff
#endif // clang-format on
}

asm void fn_80215E14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialSprKill
#endif // clang-format on
}

asm void fn_80215E18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialSprGrpSet
#endif // clang-format on
}

asm void fn_80215E1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialSprGrpKill
#endif // clang-format on
}

asm void fn_80215E20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialModelCreate
#endif // clang-format on
}

asm void fn_80215E24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialModelKill
#endif // clang-format on
}

asm void fn_80215E28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialGuideCreate
#endif // clang-format on
}

asm void fn_80215E2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialGuideGet
#endif // clang-format on
}

asm void fn_80215E30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialGuideClose
#endif // clang-format on
}

asm void fn_80215E34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialWinWait
#endif // clang-format on
}

asm void fn_80215E38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialWinClose
#endif // clang-format on
}

asm void fn_80215E3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialWinMesExec
#endif // clang-format on
}

asm void fn_80215E40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialWinCreate
#endif // clang-format on
}

asm void fn_80215E44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialWinMesMasuExec
#endif // clang-format on
}

asm void fn_80215E48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialWinKeyWait
#endif // clang-format on
}

asm void fn_80215E4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialViewSet
#endif // clang-format on
}

asm void fn_80215E50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialViewMasuSet
#endif // clang-format on
}

asm void fn_80215E54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialVSleep
#endif // clang-format on
}

asm void fn_80215E58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialCallEnd
#endif // clang-format on
}

asm void fn_80215E5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialCallWait
#endif // clang-format on
}

asm void fn_80215E60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialMultiCall
#endif // clang-format on
}

asm void fn_80215E64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialCallResult
#endif // clang-format on
}

asm void fn_80215E68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialMesCall
#endif // clang-format on
}

asm void fn_80215E6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialMesMasuCall
#endif // clang-format on
}

asm void fn_80215E70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialModeSet
#endif // clang-format on
}

asm void fn_80215E74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialModeGet
#endif // clang-format on
}

asm void fn_80215E78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialResultSet
#endif // clang-format on
}

asm void fn_80215E7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialSceneGet
#endif // clang-format on
}

asm void fn_80215E80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialTurnCall
#endif // clang-format on
}

asm void fn_80215E84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialGuideCall
#endif // clang-format on
}

asm void fn_80215E88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialCapsuleUseCall
#endif // clang-format on
}

asm void fn_80215E8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialMgCallExec
#endif // clang-format on
}

asm void fn_80215E90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbTutorialMgCallClose
#endif // clang-format on
}

asm void fn_80215E94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinInit
#endif // clang-format on
}

asm void fn_80215E98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinClose
#endif // clang-format on
}

asm void fn_80215E9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinCreate
#endif // clang-format on
}

asm void fn_80215EA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinCreateChoice
#endif // clang-format on
}

asm void fn_80215EA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinCreateHelp
#endif // clang-format on
}

asm void fn_80215EA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinCreateFrame
#endif // clang-format on
}

asm void fn_80215EAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinCreateTime
#endif // clang-format on
}

asm void fn_80215EB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinCreateBlank
#endif // clang-format on
}

asm void fn_80215EB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinCreateBlankFrame
#endif // clang-format on
}

asm void fn_80215EB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinKillAll
#endif // clang-format on
}

asm void fn_80215EBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinKill
#endif // clang-format on
}

asm void fn_80215EC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopKill
#endif // clang-format on
}

asm void fn_80215EC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinPosSet
#endif // clang-format on
}

asm void fn_80215EC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopPosSet
#endif // clang-format on
}

asm void fn_80215ECC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinPosGet
#endif // clang-format on
}

asm void fn_80215ED0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopPosGet
#endif // clang-format on
}

asm void fn_80215ED4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinSizeSet
#endif // clang-format on
}

asm void fn_80215ED8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopSizeSet
#endif // clang-format on
}

asm void fn_80215EDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinMesMaxSizeGet
#endif // clang-format on
}

asm void fn_80215EE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopMesMaxSizeGet
#endif // clang-format on
}

asm void fn_80215EE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinScaleSet
#endif // clang-format on
}

asm void fn_80215EE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopScaleSet
#endif // clang-format on
}

asm void fn_80215EEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinScaleGet
#endif // clang-format on
}

asm void fn_80215EF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopScaleGet
#endif // clang-format on
}

asm void fn_80215EF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinChoiceGet
#endif // clang-format on
}

asm void fn_80215EF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopChoiceGet
#endif // clang-format on
}

asm void fn_80215EFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinPause
#endif // clang-format on
}

asm void fn_80215F00(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopPause
#endif // clang-format on
}

asm void fn_80215F04(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinInsertMesSet
#endif // clang-format on
}

asm void fn_80215F08(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopInsertMesSet
#endif // clang-format on
}

asm void fn_80215F0C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinDoneCheck
#endif // clang-format on
}

asm void fn_80215F10(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopDoneCheck
#endif // clang-format on
}

asm void fn_80215F14(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinWait
#endif // clang-format on
}

asm void fn_80215F18(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopWait
#endif // clang-format on
}

asm void fn_80215F1C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinAttrSet
#endif // clang-format on
}

asm void fn_80215F20(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopAttrSet
#endif // clang-format on
}

asm void fn_80215F24(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinAttrReset
#endif // clang-format on
}

asm void fn_80215F28(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopAttrReset
#endif // clang-format on
}

asm void fn_80215F2C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinChoiceDisable
#endif // clang-format on
}

asm void fn_80215F30(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopChoiceDisable
#endif // clang-format on
}

asm void fn_80215F34(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinMesSpeedSet
#endif // clang-format on
}

asm void fn_80215F38(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopMesSpeedSet
#endif // clang-format on
}

asm void fn_80215F3C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinMesColSet
#endif // clang-format on
}

asm void fn_80215F40(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopMesColSet
#endif // clang-format on
}

asm void fn_80215F44(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinChoiceNowGet
#endif // clang-format on
}

asm void fn_80215F48(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopChoiceNowGet
#endif // clang-format on
}

asm void fn_80215F4C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinPriSet
#endif // clang-format on
}

asm void fn_80215F50(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopPriSet
#endif // clang-format on
}

asm void fn_80215F54(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinChoiceNowGet2
#endif // clang-format on
}

asm void fn_80215F58(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopChoiceNowGet2
#endif // clang-format on
}

asm void fn_80215F5C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinPlayerDisable
#endif // clang-format on
}

asm void fn_80215F60(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopPlayerDisable
#endif // clang-format on
}

asm void fn_80215F64(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinIDGet
#endif // clang-format on
}

asm void fn_80215F68(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopIDGet
#endif // clang-format on
}

asm void fn_80215F6C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinDispSet
#endif // clang-format on
}

asm void fn_80215F70(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopDispSet
#endif // clang-format on
}

asm void fn_80215F74(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinPauseHook
#endif // clang-format on
}

asm void fn_80215F78(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinGet
#endif // clang-format on
}

asm void fn_80215F7C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinCenterGet
#endif // clang-format on
}

asm void fn_80215F80(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinCenterInsertGet
#endif // clang-format on
}

asm void fn_80215F84(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWinTopCenterInsertGet
#endif // clang-format on
}

asm void fn_80215F88(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeSpecialInit
#endif // clang-format on
}

asm void fn_80215F8C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeSpecialClose
#endif // clang-format on
}

asm void fn_80215F90(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeSpecialKill
#endif // clang-format on
}

asm void fn_80215F94(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeWait
#endif // clang-format on
}

asm void fn_80215F98(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeCreate
#endif // clang-format on
}

asm void fn_80215F9C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeFadeOut
#endif // clang-format on
}

asm void fn_80215FA0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeFadeIn
#endif // clang-format on
}

asm void fn_80215FA4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeWhiteFadeOut
#endif // clang-format on
}

asm void fn_80215FA8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeWhiteFadeIn
#endif // clang-format on
}

asm void fn_80215FAC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeDissolveFadeOut
#endif // clang-format on
}

asm void fn_80215FB0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeDissolveFadeIn
#endif // clang-format on
}

asm void fn_80215FB4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeFadeOutTime
#endif // clang-format on
}

asm void fn_80215FB8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeFadeInTime
#endif // clang-format on
}

asm void fn_80215FBC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeWhiteFadeOutTime
#endif // clang-format on
}

asm void fn_80215FC0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeWhiteFadeInTime
#endif // clang-format on
}

asm void fn_80215FC4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeDissolveFadeOutTime
#endif // clang-format on
}

asm void fn_80215FC8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeDissolveFadeInTime
#endif // clang-format on
}

asm void fn_80215FCC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeSpecialCreate
#endif // clang-format on
}

asm void fn_80215FD0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeSpecialWait
#endif // clang-format on
}

asm void fn_80215FD4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeSpecialFadeOutCreate
#endif // clang-format on
}

asm void fn_80215FD8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeSpecialFadeInCreate
#endif // clang-format on
}

asm void fn_80215FDC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbWipeSpecialStatGet
#endif // clang-format on
}

asm void fn_80215FE0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMgCallInit
#endif // clang-format on
}

asm void fn_80215FE4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MgCall
#endif // clang-format on
}

asm void fn_80215FE8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MgCallKettou
#endif // clang-format on
}

asm void fn_80215FEC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MgCallDonkey
#endif // clang-format on
}

asm void fn_80215FF0(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MgCallKoopa
#endif // clang-format on
}

asm void fn_80215FF4(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MgCallSingle
#endif // clang-format on
}

asm void fn_80215FF8(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MgCallSingleKoopa
#endif // clang-format on
}

asm void fn_80215FFC(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMgCallDataClose
#endif // clang-format on
}

asm void fn_80216000(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbev_MgCallTutorial
#endif // clang-format on
}

asm void fn_80216004(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMgRouletteFocusKill
#endif // clang-format on
}

asm void fn_80216008(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMgRouletteNumGet
#endif // clang-format on
}

asm void fn_8021600C(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMgCallSingleOnCheck
#endif // clang-format on
}

asm void fn_80216010(void) {
#ifdef __MWERKS__ // clang-format off
    nofralloc
    b mbMgCallVsEffCreate
#endif // clang-format on
}
