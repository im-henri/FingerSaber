#include "FingerSaber.hpp"
#include "main.hpp"

#include "GlobalNamespace/OVRSkeleton.hpp"
#include "GlobalNamespace/OVRSkeletonRenderer.hpp"
#include "GlobalNamespace/OVRSkeletonRenderer_BoneVisualization.hpp"

MAKE_HOOK_MATCH(
    BoneVisualization_Update,
    &GlobalNamespace::OVRSkeletonRenderer::BoneVisualization::Update,
    void,
    GlobalNamespace::OVRSkeletonRenderer::BoneVisualization* self,

    float scale,
    bool shouldRender,
    bool shouldUseSystemGestureMaterial,
    GlobalNamespace::OVRSkeletonRenderer::ConfidenceBehavior confidenceBehavior,
    GlobalNamespace::OVRSkeletonRenderer::SystemGestureBehavior systemGestureBehavior
) {

    float scale_in;
    if( modManager.is_scene_GameCore == true && modManager.multiplayerGameFailed == false){
        scale_in = 7.5f;
    }
    else{
        scale_in = scale;
    }

    BoneVisualization_Update(
        self,
        scale_in,
        shouldRender,
        shouldUseSystemGestureMaterial,
        confidenceBehavior,
        systemGestureBehavior
    );

}

void FingerSaber::_Hook_BoneVisualization_Update(){
    INSTALL_HOOK(getLogger(), BoneVisualization_Update);
}