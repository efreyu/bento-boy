#ifndef BENTO_BOY_BENTONODE_H
#define BENTO_BOY_BENTONODE_H

#include "axmol/axmol.h"
#include "databaseModule/mapObjectsDatabase.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include <string>

namespace bt::gameplayModule {
    enum class eBentoAnimation {
        IDLE = 0, SLEEP, WIN, MOVE
    };
    class bentoNode
      : public generic::coreModule::nodeProperties
      , public ax::Node {
    public:
        bentoNode();
        ~bentoNode() override;
        CREATE_FUNC(bentoNode);
        void initWithData(const bt::databaseModule::sMapObjectsData&);
        void setAnimation(eBentoAnimation);
        databaseModule::eMapObjectType type;
        int objectId = 0;// local id from database
    private:
        eBentoAnimation lastAnimation = eBentoAnimation::SLEEP;
    };
}// namespace bt::gameplayModule


#endif// BENTO_BOY_BENTONODE_H
