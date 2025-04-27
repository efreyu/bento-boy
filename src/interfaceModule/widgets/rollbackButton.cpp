#include "rollbackButton.h"

#include "generic/utilityModule/findUtility.h"

using namespace bt::interfaceModule;
using namespace generic::utilityModule;

rollbackButton::rollbackButton() {
    this->setName("rollbackButton");
    initWithProperties("widgets/" + std::string(this->getName()));
    setLabelText();
}

rollbackButton::~rollbackButton() {}

ax::Label* rollbackButton::getLabel() {
    if (auto label = dynamic_cast<ax::Label*>(findNode(this, "notificationCounter")))
        return label;
    return nullptr;
}

void rollbackButton::setLabelText(const std::string& str) {
    if (auto sprite = findNode(this, "notificationBg")) {
        sprite->setVisible(!str.empty());
    }
    if (auto label = getLabel()) {
        label->setString(str);
    }
}
