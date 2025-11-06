#ifndef BENTO_BOY_GAMESCENE_H
#define BENTO_BOY_GAMESCENE_H

#include "axmol/axmol.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/scenes/sceneInterface.h"
// #ifdef AX_ENABLE_EXT_INSPECTOR
#include "Inspector/Inspector.h"
// #endif

namespace bt::sceneModule {

    class gameScene
      : public generic::coreModule::nodeProperties
        , public generic::coreModule::sceneInterface {
    public:
        gameScene();
        ~gameScene() override = default;
        void onSceneLoading() override;
// #ifdef AX_ENABLE_EXT_INSPECTOR
        void onEnter() override {
            generic::coreModule::sceneInterface::onEnter();
            ax::extension::Inspector::getInstance()->openForScene(this);
        }
        void onExit() override {
            ax::extension::Inspector::getInstance()->close();
            generic::coreModule::sceneInterface::onExit();
        }
// #endif

    private:
#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32) || (AX_TARGET_PLATFORM == AX_PLATFORM_MAC) || (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX) || (AX_TARGET_PLATFORM == AX_PLATFORM_EMSCRIPTEN)
        ax::EventListenerKeyboard* keyboardListener = nullptr;
#endif
    };
}// namespace bt::sceneModule


#endif// BENTO_BOY_GAMESCENE_H
