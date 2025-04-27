#ifndef BENTO_BOY_BUTTONWIDGET_H
#define BENTO_BOY_BUTTONWIDGET_H

#include "cocos2d.h"
#include "generic/coreModule/nodes/nodeProperties.h"

namespace bt::interfaceModule {

    class buttonWidget
      : public ax::Node
      , public generic::coreModule::nodeProperties {
    public:
        buttonWidget();

        void setText(const std::string&);
        void setDisabled();
        void setSelect(bool status);
        void setSmallText();

    private:
        void initWidget();
        void updateSize();
        float selectHorizontalOffset = 0.f;
    };
}// namespace bt::interfaceModule

#endif// BENTO_BOY_BUTTONWIDGET_H
