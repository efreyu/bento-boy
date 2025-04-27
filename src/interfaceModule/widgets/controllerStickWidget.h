#ifndef BENTO_BOY_CONTROLLERSTICKWIDGET_H
#define BENTO_BOY_CONTROLLERSTICKWIDGET_H

#include "cocos2d.h"
#include "gameplayModule/moveEnum.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/nodes/types/asepriteNode.h"
#include "generic/coreModule/nodes/types/buttonType.h"
#include "generic/coreModule/nodes/types/eventNode.h"
#include "generic/coreModule/signals/signalHolder.h"

namespace bt::interfaceModule {

    struct sControllerStickEvents {
        generic::signal::signalHolder<gameplayModule::eMoveDirection> onPressed;
        generic::signal::signalHolder<> onReleased;
    };

    class controllerStickWidget
      : public generic::coreModule::buttonType<ax::Node>
      , public generic::coreModule::nodeProperties {
    public:
        controllerStickWidget();
        bool init() override;

        sControllerStickEvents* getEmitter() {
            return &eventHolder;
        }

    private:
        void initHandler();
        bool touchProceed(ax::Touch* touch, ax::Event* event);
        void onButtonHold();

        ax::EventListenerTouchOneByOne* listener = nullptr;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        ax::EventListenerKeyboard* keyboardListener = nullptr;
#endif
        std::map<gameplayModule::eMoveDirection, ax::Node*> nodesWithDirections;
        gameplayModule::eMoveDirection currentPressed = gameplayModule::eMoveDirection::UNDEFINED;
        generic::coreModule::asepriteNode* arrowsNode = nullptr;
        sControllerStickEvents eventHolder;
    };
}// namespace bt::interfaceModule


#endif// BENTO_BOY_CONTROLLERSTICKWIDGET_H
