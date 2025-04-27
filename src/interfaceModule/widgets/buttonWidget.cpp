#include "buttonWidget.h"
#include "generic/utilityModule/convertUtility.h"
#include "generic/utilityModule/findUtility.h"
#include "ui/CocosGUI.h"

using namespace bt::interfaceModule;
using namespace generic::utilityModule;

buttonWidget::buttonWidget() {
    this->setName("buttonWidget");
    initWithProperties("widgets/" + std::string(this->getName()));
    setSelect(false);
    initWidget();
}

void buttonWidget::setText(const std::string& value) {
    if (auto label = dynamic_cast<ax::Label*>(findNode(this, "label"))) {
        label->setString(value);
        label->init();
        updateSize();
    }
}

void buttonWidget::setDisabled() {
    if (auto label = dynamic_cast<ax::Label*>(findNode(this, "label"))) {
        loadProperty(label, "gray");
    }
}

void buttonWidget::setSelect(bool status) {
    if (auto select = dynamic_cast<ax::ui::Scale9Sprite*>(findNode(this, "select"))) {
        select->setVisible(status);
    }
}

void buttonWidget::initWidget() {
    if (!hasPropertyObject("settings"))
        return;
    const auto& json = getPropertyObject("settings");
    if (json.HasMember("selectHorizontalOffset") && json["selectHorizontalOffset"].IsNumber()) {
        selectHorizontalOffset = json["selectHorizontalOffset"].GetFloat();
    }
    float lightening = 1.f;
    float selectDuration = .1f;
    if (json.HasMember("lightening") && json["lightening"].IsNumber()) {
        lightening = json["lightening"].GetFloat();
    }
    if (json.HasMember("selectDuration") && json["selectDuration"].IsNumber()) {
        selectDuration = json["selectDuration"].GetFloat();
    }
    if (auto select = findNode(this, "select")) {
        auto color = select->getColor();
        auto nextColor = generic::utilityModule::convertUtility::changeColorByPercent(color, lightening);
        auto tint = ax::TintTo::create(selectDuration, nextColor);
        auto reverse = ax::TintTo::create(selectDuration, color);
        auto seq = ax::Sequence::create(tint, reverse, nullptr);
        select->runAction(ax::RepeatForever::create(seq));
    }
}

void buttonWidget::setSmallText() {
    if (auto label = findNode(this, "label")) {
        loadProperty(label, "small");
        updateSize();
    }
}

void buttonWidget::updateSize() {
    if (auto label = findNode(this, "label")) {
        auto size = label->getContentSize();
        setContentSize(size);
        if (auto select = dynamic_cast<ax::ui::Scale9Sprite*>(findNode(this, "select"))) {
            select->setContentSize({size.width + selectHorizontalOffset, size.height});
        }
    }
}
