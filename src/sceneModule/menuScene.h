#ifndef BENTO_BOY_MENUSCENE_H
#define BENTO_BOY_MENUSCENE_H

#include "axmol/axmol.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/scenes/sceneInterface.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace bt::sceneModule {

    enum class eMenuPageType {
        MAIN_MENU = 0,
        PLAY
    };

    struct menuItem {
        std::string type;
        std::string text;
        bool enabled = true;
        bool backBtn = false;
        std::function<void()> clb = nullptr;
    };

    struct menuPage {
        std::string pageId;
        std::vector<std::shared_ptr<menuItem>> buttons;
        std::string hintText;
        bool smallText = false;
    };

    struct sActiveMenu {
        ax::Node* node = nullptr;
        bool selected = false;
        std::function<void()> clb = nullptr;
        ~sActiveMenu() {
            AX_SAFE_RELEASE(node);
        }
    };

    class menuScene
      : public generic::coreModule::nodeProperties
      , public generic::coreModule::sceneInterface {
    public:
        menuScene();
        void onSceneLoading() override;

    private:
        void loadSettings();
        void initMenu(const std::string& path);
        void loadPage(const std::string& page);
        std::map<std::string, std::shared_ptr<menuPage>> menuPagesMap;
        std::list<std::shared_ptr<sActiveMenu>> menuList;

        struct menuSceneSettings {
            int allowedItemCount;
            std::string moreButtonText;
            std::string levelProgressPattern;
            float lightening = 1.f;
            float selectDuration = .1f;
        };
        menuSceneSettings settings;

#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32) || (AX_TARGET_PLATFORM == AX_PLATFORM_MAC) || (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX) || (AX_TARGET_PLATFORM == AX_PLATFORM_EMSCRIPTEN)
        ax::EventListenerKeyboard* keyboardListener = nullptr;
#endif
    };
}// namespace bt::sceneModule


#endif// BENTO_BOY_MENUSCENE_H
