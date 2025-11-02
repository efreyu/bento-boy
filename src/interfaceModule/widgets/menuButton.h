#ifndef BENTO_BOY_MENUBUTTON_H
#define BENTO_BOY_MENUBUTTON_H

#include "axmol/axmol.h"
#include "generic/coreModule/nodes/nodeProperties.h"
#include "generic/coreModule/nodes/types/soundButton.h"

namespace bt::interfaceModule {

    class menuButton
      : public generic::coreModule::soundButton {
    public:
        menuButton();
        ~menuButton() override;
        CREATE_FUNC(menuButton);

    };

}// namespace bt::interfaceModule


#endif// BENTO_BOY_MENUBUTTON_H
