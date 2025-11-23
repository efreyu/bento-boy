#pragma once
#ifndef BENTO_BOY_ROLLBACKBUTTON_H
#define BENTO_BOY_ROLLBACKBUTTON_H

#include "axmol/axmol.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/nodes/types/soundButton.h"

namespace bt::interfaceModule {

    class rollbackButton
      : public generic::coreModule::soundButton {
    public:
        rollbackButton();
        ~rollbackButton() override;
        CREATE_FUNC(rollbackButton);

        ax::Label* getLabel();
        void setLabelText(const std::string& str = std::string());

    };

}// namespace bt::interfaceModule


#endif// BENTO_BOY_ROLLBACKBUTTON_H
