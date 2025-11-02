#ifndef BENTO_BOY_GAMESCENE_H
#define BENTO_BOY_GAMESCENE_H

#include "axmol/axmol.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/scenes/sceneInterface.h"

namespace bt::sceneModule {

    class gameScene
      : public generic::coreModule::nodeProperties
        , public generic::coreModule::sceneInterface {
    public:
        gameScene();
        ~gameScene() override = default;
        void onSceneLoading() override;
    };
}// namespace bt::sceneModule


#endif// BENTO_BOY_GAMESCENE_H
