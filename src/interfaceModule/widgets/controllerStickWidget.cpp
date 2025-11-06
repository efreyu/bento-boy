#include "controllerStickWidget.h"
#include "generic/coreModule/scenes/scenesFactoryInstance.h"
#include "generic/utilityModule/findUtility.h"

using namespace bt::interfaceModule;
using namespace bt::gameplayModule;
using namespace generic::utilityModule;

const std::map<std::string, eMoveDirection> moveTypesMap = {
    {"up", eMoveDirection::UP},
    {"down", eMoveDirection::DOWN},
    {"left", eMoveDirection::LEFT},
    {"right", eMoveDirection::RIGHT}
};

const std::map<eMoveDirection, std::string> animationTypesMap = {
    {eMoveDirection::UNDEFINED, "default"},
    {eMoveDirection::UP, "up_pressed"},
    {eMoveDirection::DOWN, "down_pressed"},
    {eMoveDirection::LEFT, "left_pressed"},
    {eMoveDirection::RIGHT, "right_pressed"}
};

const int buttonActionTag = 1;
const float animDelay = 0.2f;

controllerStickWidget::controllerStickWidget() {
    this->setName("controllerStickWidget");
    initWithProperties("widgets/" + std::string(this->getName()));
    removeJsonData();
    arrowsNode = dynamic_cast<generic::coreModule::asepriteNode*>(findNode(this, "arrows"));
    if (!arrowsNode)
        LOG_ERROR("Can't find 'arrows' node.");
}

void controllerStickWidget::initHandler() {
    listener = ax::EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](auto touch, auto event) {
        return touchProceed(touch, event);
    };
    listener->onTouchMoved = [this](auto touch, auto event) {
        return touchProceed(touch, event);
    };
    listener->onTouchCancelled = listener->onTouchEnded = [this](auto, auto){
        currentPressed = eMoveDirection::UNDEFINED;
        getEmitter()->onReleased.emit();
        return true;
    };
    GET_CURRENT_SCENE()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32) || (AX_TARGET_PLATFORM == AX_PLATFORM_MAC) || (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX)
    keyboardListener = ax::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [this](ax::EventKeyboard::KeyCode keyCode, ax::Event* event) {
        auto keyboardDir = eMoveDirection::UNDEFINED;
        switch (keyCode) {
        case ax::EventKeyboard::KeyCode::KEY_UP_ARROW:
        case ax::EventKeyboard::KeyCode::KEY_W:
            keyboardDir = eMoveDirection::UP;
            break;
        case ax::EventKeyboard::KeyCode::KEY_DOWN_ARROW:
        case ax::EventKeyboard::KeyCode::KEY_S:
            keyboardDir = eMoveDirection::DOWN;
            break;
        case ax::EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        case ax::EventKeyboard::KeyCode::KEY_A:
            keyboardDir = eMoveDirection::LEFT;
            break;
        case ax::EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
        case ax::EventKeyboard::KeyCode::KEY_D:
            keyboardDir = eMoveDirection::RIGHT;
            break;
        }
        if (keyboardDir != eMoveDirection::UNDEFINED) {
            currentPressed = keyboardDir;
            onButtonHold();
        }
    };
    keyboardListener->onKeyReleased = [this](auto, auto) {
        currentPressed = eMoveDirection::UNDEFINED;
        getEmitter()->onReleased.emit();
    };
    GET_CURRENT_SCENE()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

#endif
}

bool controllerStickWidget::init() {
    for (const auto& [nodeName, direction] : moveTypesMap) {
        if (auto node = findNode(this, nodeName)) {
            nodesWithDirections[direction] = node;
        }
    }
    initHandler();
    return Node::init();
}

bool controllerStickWidget::touchProceed(ax::Touch* touch, ax::Event* event) {
    auto touchLocation = event->getCurrentTarget()->convertToNodeSpace(touch->getLocation());
    for (const auto& [direction, buttonNode] : nodesWithDirections) {
        auto boundingBox = buttonNode->getBoundingBox();
        boundingBox.origin.x += boundingBox.size.width * buttonNode->getAnchorPoint().x + boundingBox.size.width * buttonNode->getPivotPoint().x;
        boundingBox.origin.y += boundingBox.size.height * buttonNode->getAnchorPoint().y + boundingBox.size.height * buttonNode->getPivotPoint().y;
        bool correctNode = boundingBox.containsPoint(touchLocation);
        if (!correctNode)
            continue;

        currentPressed = direction;
        onButtonHold();

        return true;
    }
    currentPressed = eMoveDirection::UNDEFINED;
    return false;
}

void controllerStickWidget::onButtonHold() {
    if (!arrowsNode || currentPressed == eMoveDirection::UNDEFINED || getActionByTag(buttonActionTag))
        return;
    auto delayAction = ax::DelayTime::create(animDelay);
    auto clb = ax::CallFunc::create([this]() {
        if (currentPressed == eMoveDirection::UNDEFINED) {
            if (auto currentAction = getActionByTag(buttonActionTag)) {
                stopAction(currentAction);
            }
        } else {
            getEmitter()->onPressed.emit(currentPressed);
        }
        if (animationTypesMap.count(currentPressed)) {
            arrowsNode->setAnimation(animationTypesMap.at(currentPressed));
        }
    });
    auto seq = ax::Sequence::create(clb, delayAction, nullptr);
    auto repeatAction = ax::RepeatForever::create(seq);
    repeatAction->setTag(buttonActionTag);
    runAction(repeatAction);
}
