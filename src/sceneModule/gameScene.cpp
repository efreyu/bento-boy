#include "gameScene.h"
#include "gameplayModule/gameBoard.h"
#include "generic/utilityModule/findUtility.h"
#include "interfaceModule/widgets/controllerButtonWidget.h"
#include "interfaceModule/widgets/controllerStickWidget.h"

using namespace bt::sceneModule;
using namespace generic::utilityModule;

gameScene::gameScene() {
    this->setName("gameScene");
    initWithProperties("scenes/" + std::string(this->getName()));
    if (!hasPropertyObject("settings"))
        return;
    const auto& json = getPropertyObject("settings");
    auto color = ax::Color32::BLACK;
    if (json.HasMember("bgColor") && json["bgColor"].IsArray()) {
        color.r = static_cast<uint8_t>(json["bgColor"][0u].GetInt());
        color.g = static_cast<uint8_t>(json["bgColor"][1u].GetInt());
        color.b = static_cast<uint8_t>(json["bgColor"][2u].GetInt());
    }
    initLayerColor(color);
    if (json.HasMember("fadeTransitionTime") && json["fadeTransitionTime"].IsNumber()) {
        _fadeTransitionTime = json["fadeTransitionTime"].GetFloat();
    }
}

void gameScene::onSceneLoading() {
    sceneInterface::onSceneLoading();
    auto displayHolder = findNode(this, "displayHolder");
    auto controllerNode = dynamic_cast<interfaceModule::controllerStickWidget*>(findNode(this, "controller"));
    auto buttonA = dynamic_cast<interfaceModule::controllerButtonWidget*>(findNode(this, "buttonA"));
    auto buttonB = dynamic_cast<interfaceModule::controllerButtonWidget*>(findNode(this, "buttonB"));
    if (!displayHolder || !controllerNode
        || !buttonA || !buttonB) {
        LOG_ERROR("Problems with loading nodes.");
        return;
    }
    auto board = new gameplayModule::gameBoard();
    displayHolder->addChild(board, -1);
    auto levelId = 1;
    if (_sceneData.count("levelId")) {
        levelId = _sceneData["levelId"].asInt();
    }
    board->loadLevel(levelId);
    controllerNode->init();
    buttonA->initListener();
    buttonB->initListener();
    buttonB->setOnTouchEnded([](){
        GET_SCENES_FACTORY().runScene("menuScene");
    });
    board->attachController(controllerNode->getEmitter(), buttonA);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
    keyboardListener = ax::EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [this, board](ax::EventKeyboard::KeyCode keyCode, ax::Event* event) {
        switch (keyCode) {
        case ax::EventKeyboard::KeyCode::KEY_SPACE:
        case ax::EventKeyboard::KeyCode::KEY_ESCAPE:
        // case ax::EventKeyboard::KeyCode::KEY_BACKSPACE:
            GET_SCENES_FACTORY().runScene("menuScene");
            break;
        case ax::EventKeyboard::KeyCode::KEY_ENTER:
            board->reloadLevel();
            break;
        default:
            break;
        }
    };
    GET_CURRENT_SCENE()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);

#endif
}
