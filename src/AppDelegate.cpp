#include "AppDelegate.h"
#include "generic/audioModule/audioEngineInstance.h"
#include "generic/coreModule/resources/resourceManager.h"
#include "generic/coreModule/scenes/scenesFactoryInstance.h"
// #include "generic/debugModule/logManager.h"

// all profile block header
#include "generic/profileModule/profileManager.h"
#include "profileModule/progressProfileBlock.h"
// all databases header
#include "databaseModule/databaseManager.h"
#include "databaseModule/levelsDatabase.h"
#include "databaseModule/mapObjectsDatabase.h"
// all scenes
#include "sceneModule/gameScene.h"
#include "sceneModule/menuScene.h"
// widgets
#include "interfaceModule/customNodeTypes.h"

using namespace ax;
using namespace bt;

#define USE_VR_RENDERER  0

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {
    generic::audioModule::audioEngineInstance::cleanup();
    generic::profileModule::profileManager::cleanup();
    databaseModule::databaseManager::cleanup();
    generic::coreModule::nodeFactory::cleanup();
    generic::coreModule::scenesFactoryInstance::cleanup();
    // generic::debugModule::logManager::cleanup();
}

// if you want a different context, modify the value of contextAttrs
// it will affect all platforms
void AppDelegate::initContextAttrs()
{
    // set app context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    // powerPreference only affect when RHI backend is D3D
    ContextAttrs contextAttrs = {.powerPreference = PowerPreference::HighPerformance};

    // V-Sync is enabled by default since axmol 2.2.
    // Uncomment to disable V-Sync and unlock FPS.
    // contextAttrs.vsync = false;

    // Enable high-DPI scaling support (non-Windows platforms only)
    // Note: cpp-tests keep the default render mode to ensure consistent performance benchmarks
#if AX_TARGET_PLATFORM != AX_PLATFORM_WIN32
    contextAttrs.renderScaleMode = RenderScaleMode::Physical;
#endif
    setContextAttrs(contextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto setting = GET_RESOLUTION_SETTING();
    setting->load();
#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32) || (AX_TARGET_PLATFORM == AX_PLATFORM_MAC) || (AX_TARGET_PLATFORM == AX_PLATFORM_LINUX) || (AX_TARGET_PLATFORM == AX_PLATFORM_EMSCRIPTEN)
    setting->init(false, "640x1136");// default development resolution
#else
    setting->init(true);
#endif
    auto currentResolution = setting->getCurrentSize();
    auto director = Director::getInstance();
    auto renderView = director->getRenderView();
    if (!renderView)
    {
#if (AX_TARGET_PLATFORM == AX_PLATFORM_WIN32) || (AX_TARGET_PLATFORM == AX_PLATFORM_MAC) || \
(AX_TARGET_PLATFORM == AX_PLATFORM_LINUX) || (AX_TARGET_PLATFORM == AX_PLATFORM_EMSCRIPTEN)
        renderView = RenderViewImpl::createWithRect(
            "Bento boy", ax::Rect(0, 0, currentResolution->size.x, currentResolution->size.y));
#else
        renderView = RenderViewImpl::create("Bento boy");
#endif
        director->setRenderView(renderView);
    }
#if USE_VR_RENDERER && defined(AX_ENABLE_VR)
    auto vrRenderer = std::make_unique<VRGenericRenderer>();
    // On Android/iOS emulator devices, uncomment to visualize the left/right eye VR rendering output.
    // Useful for debugging stereo rendering without a physical headset.
    // vrRenderer->setDebugIgnoreHeadTracker(true);
    renderView->setVR(std::move(vrRenderer));
#endif

    // turn on display FPS
    director->setStatsDisplay(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);
    // director->setProjection(Director::Projection::_3D);

    // Set the design resolution
#if (AX_TARGET_PLATFORM == AX_PLATFORM_EMSCRIPTEN)
    renderView->setDesignResolutionSize(640, 1136, ResolutionPolicy::SHOW_ALL);
#else
    renderView->setDesignResolutionSize(currentResolution->size.x, currentResolution->size.y, ResolutionPolicy::SHOW_ALL);
#endif

    GET_AUDIO_ENGINE().stopAll();
    // register all profile
    GET_PROFILE().registerBlock("progress", []() {
        return new profileModule::progressProfileBlock();
    });
    GET_PROFILE().executeLoad();
    // register all databases
    GET_DATABASE_MANAGER().addDatabase(
        databaseModule::databaseManager::eDatabaseType::LEVELS_DB,
        "properties/database/levels/db.json",
        new databaseModule::levelsDatabase
    );
    GET_DATABASE_MANAGER().addDatabase(
        databaseModule::databaseManager::eDatabaseType::MAP_OBJECTS_DB,
        "properties/database/mapObjects/db.json",
        new databaseModule::mapObjectsDatabase
    );
    GET_DATABASE_MANAGER().executeLoadData();
    // register external node types
    interfaceModule::registerAllCustomNodes();
    // register all states
    GET_SCENES_FACTORY().registerScene("menuScene", []() {
        return new sceneModule::menuScene();
        // return utils::createInstance<sceneModule::menuScene>();
    });
    GET_SCENES_FACTORY().registerScene("gameScene", []() {
        return new sceneModule::gameScene();
        // return utils::createInstance<sceneModule::gameScene>();
    });

    // run first scene
    GET_SCENES_FACTORY().runScene("menuScene");

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    GET_AUDIO_ENGINE().pauseAll();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    GET_AUDIO_ENGINE().resumeAll();
}
