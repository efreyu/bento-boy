#ifndef BENTO_BOY_GAMEBOARD_H
#define BENTO_BOY_GAMEBOARD_H

#include "axmol/axmol.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/nodes/types/eventNode.h"
#include "generic/coreModule/signals/signalHolder.h"
#include "moveEnum.h"
#include <functional>
#include <string>
#include <tuple>
#include <vector>


namespace bt::interfaceModule {
    struct sControllerStickEvents;
}

namespace bt::gameplayModule {

    class mapDispatcher;

    class gameBoard
      : public generic::coreModule::nodeProperties
      , public ax::Node {
    public:
        gameBoard();
        void loadLevel(int id);
        ~gameBoard() override;
        ax::TMXTiledMap* getTiled() const {
            return tiledMap;
        }
        void attachController(interfaceModule::sControllerStickEvents* emitter, generic::coreModule::eventNode* replayBtn);

    private:
        void loadSettings();
        void updateMovesScore();
        void runShowAnimation(const std::function<void()>& clb);
        void runHideAnimation(const std::function<void()>& clb);

        ax::Node* gameFieldNode = nullptr;
        mapDispatcher* dispatcher = nullptr;
        ax::TMXTiledMap* tiledMap = nullptr;
        ax::Label* movesLabel = nullptr;
        ax::Label* levelsLabel = nullptr;
        int currentLevel;
        int movesCnt;
        bool boardBlocked = true;

        struct gameBoardSettings {
            std::string movesPattern;
            std::string levelsPattern;
            float fadeDuration;
            float delayDuration;
        };
        gameBoardSettings settings;
    };
}// namespace bt::gameplayModule


#endif// BENTO_BOY_GAMEBOARD_H
