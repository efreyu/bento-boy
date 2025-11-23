#include "menuButton.h"

using namespace bt::interfaceModule;

menuButton::menuButton() {
    this->setName("menuButton");
    initWithProperties("widgets/" + std::string(this->getName()));
}

menuButton::~menuButton() {}
