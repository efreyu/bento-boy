#include "gameBoard.h"
#include "databaseModule/databaseManager.h"
#include "databaseModule/levelsDatabase.h"
#include "gameplayModule/mapDispatcher.h"
#include "generic/audioModule/audioEngineInstance.h"
#include "generic/coreModule/nodes/types/node3d.h"
// #include "generic/debugModule/imGuiLayer.h"
// #include "generic/debugModule/logManager.h"
#include "generic/profileModule/profileManager.h"
#include "generic/utilityModule/findUtility.h"
#include "interfaceModule/widgets/controllerStickWidget.h"
#include "profileModule/progressProfileBlock.h"

using namespace bt::gameplayModule;
using namespace bt::databaseModule;
using namespace generic::utilityModule;

gameBoard::gameBoard() {
    this->setName("gameBoard");
    initWithProperties(this->getName());
    gameFieldNode = findNode(this, "gameField");
    if (!gameFieldNode) {
        LOG_ERROR("gameField was not loaded correctly!");
    }
    movesLabel = dynamic_cast<ax::Label*>(findNode(this, "movesLabel"));
    levelsLabel = dynamic_cast<ax::Label*>(findNode(this, "infoLabel"));
    loadSettings();
    removeJsonData();
}

void gameBoard::loadSettings() {
    if (!hasPropertyObject("settings"))
        return;
    const auto& json = getPropertyObject("settings");
    if (json.HasMember("movesPattern") && json["movesPattern"].IsString()) {
        settings.movesPattern = json["movesPattern"].GetString();
    }
    if (json.HasMember("levelsPattern") && json["levelsPattern"].IsString()) {
        settings.levelsPattern = json["levelsPattern"].GetString();
    }
    if (json.HasMember("fadeDuration") && json["fadeDuration"].IsNumber()) {
        settings.fadeDuration = json["fadeDuration"].GetFloat();
    }
    if (json.HasMember("delayDuration") && json["delayDuration"].IsNumber()) {
        settings.delayDuration = json["delayDuration"].GetFloat();
    }
}

gameBoard::~gameBoard() {
    delete dispatcher;
    dispatcher = nullptr;
}

void gameBoard::loadLevel(int id) {
    auto levelsDb = GET_DATABASE_MANAGER().getDatabase<levelsDatabase>(databaseManager::eDatabaseType::LEVELS_DB);
    if (!levelsDb->hasLevelById(id)) {
        LOG_ERROR(fmt::format("level {} not found!", id));
        GET_SCENES_FACTORY().runScene("menuScene");
        return;
    }
    if (tiledMap) {
        tiledMap->removeFromParentAndCleanup(true);
        delete tiledMap;
        tiledMap = nullptr;
    }
    currentLevel = id;
    auto levelData = levelsDb->getLevelById(id);
    tiledMap = new ax::TMXTiledMap();
    tiledMap->initWithTMXFile(levelData.tmxPath);
    gameFieldNode->addChild(tiledMap);
    // update battleField size
    {
        auto size = tiledMap->getTileSize();
        auto mapSize = tiledMap->getMapSize();
        gameFieldNode->setContentSize({ mapSize.width * size.width, mapSize.height * size.height });
        setContentSize(gameFieldNode->getContentSize());
    }
    // update timeMap scale and position
    {
        auto width = tiledMap->getTileSize().width;
        auto mapSize = tiledMap->getMapSize();
        auto scale = ax::Director::getInstance()->getVisibleSize().width / (mapSize.width * width - width);
        setScale(scale);
    }

    if (auto objectsLayer = findNode(gameFieldNode, "objectsLayer")) {
        objectsLayer->removeFromParentAndCleanup(true);
        delete objectsLayer;
        objectsLayer = nullptr;
    }
    auto objectsLayer = new generic::coreModule::node3d();
    objectsLayer->setName("objectsLayer");
    gameFieldNode->addChild(objectsLayer);
    boardBlocked = true;
    runShowAnimation([this](){
        boardBlocked = false;
    });
    dispatcher = mapDispatcher::createWithObjectsNode(objectsLayer, tiledMap, id);
    dispatcher->getEmitter()->onWin.connect([this](){
        GET_AUDIO_ENGINE().play("gameScene.win");
        auto progressBlock = GET_PROFILE().getBlock<profileModule::progressProfileBlock>("progress");
        if (auto block = progressBlock->getProgressForLevel(currentLevel)) {
            block->addProgress(movesCnt);
        }
        SAVE_PROFILE();
        boardBlocked = true;
        runHideAnimation([this](){
            loadLevel(currentLevel + 1);
        });
    });
    movesCnt = 0;
    dispatcher->getEmitter()->onPlayerMove.connect([this](){
        GET_AUDIO_ENGINE().play("gameScene.move");
        movesCnt++;
        updateMovesScore();
    });
    updateMovesScore();
    if (levelsLabel) {
        if (!settings.levelsPattern.empty()) {
            levelsLabel->setString(fmt::format(fmt::runtime(settings.levelsPattern), currentLevel));
        } else {
            levelsLabel->setString(std::to_string(currentLevel));
        }
    }
}

void gameBoard::attachController(interfaceModule::sControllerStickEvents* emitter, generic::coreModule::eventNode* replayBtn) {
    emitter->onPressed.connect([this](auto direction){
        if (!boardBlocked && dispatcher)
            dispatcher->move(direction);
    });
    replayBtn->setOnTouchBegan([this](){
        if (!boardBlocked) {
            boardBlocked = true;
            runHideAnimation([this](){
                loadLevel(currentLevel);
            });
        }
    });
}

void gameBoard::updateMovesScore() {
    if (movesLabel) {
        if (!settings.movesPattern.empty()) {
            movesLabel->setString(fmt::format(fmt::runtime(settings.movesPattern), movesCnt));
        } else {
            movesLabel->setString(std::to_string(movesCnt));
        }
    }
}

void gameBoard::runShowAnimation(const std::function<void()>& clb) {
//    auto fadein = ax::Show::create(settings.fadeDuration);
    auto clbDone = ax::CallFunc::create([clb](){
        if (clb)
            clb();
    });
    runAction(ax::Sequence::create(clbDone, nullptr));
}

void gameBoard::runHideAnimation(const std::function<void()>& clb) {
//    auto fadeout = ax::Hide::create(settings.fadeDuration);
    auto delay = ax::DelayTime::create(settings.delayDuration);
    auto clbDone = ax::CallFunc::create([clb](){
        if (clb)
            clb();
    });
    runAction(ax::Sequence::create(delay, clbDone, nullptr));
}
