#pragma once
#include <Geode/loader/SettingNode.hpp>
#include "EasingListPopup.hpp"

using namespace geode::prelude;

// fake setting!!!
class EasingListSetting : public SettingValue {
protected:
    std::string m_placeholder;
public:
    EasingListSetting(std::string const& key, std::string const& modID, std::string const& placeholder)
        : SettingValue(key, modID), m_placeholder(placeholder) {}
    bool load(matjson::Value const& json) override { return true; }
    bool save(matjson::Value& json) const override { return true; }
    SettingNode* createNode(float width) override;
};

class EasingListNode : public SettingNode {
protected:
    bool init(EasingListSetting* value, float width) {
        if (!SettingNode::init(value)) return false;

        setContentSize({ width, 35.f });

        CCMenu* menu = CCMenu::create();
        menu->setPosition(CCPoint{ 0, 0 });

        ButtonSprite* spr = ButtonSprite::create("Easing types");
        spr->setScale(0.7f);
        CCMenuItemSpriteExtra* button = CCMenuItemSpriteExtra::create(spr, this, menu_selector(EasingListNode::showList));
        button->setPosition(CCPoint{ width / 2, 20.f });
        menu->addChild(button);

        addChild(menu);

        return true;
    }

    void showList(CCObject* sender) { EasingListPopup::create()->show(); }

public:
    void commit() override { dispatchCommitted(); }

    bool hasUncommittedChanges() override { return false; }
    bool hasNonDefaultValue() override { return false; }
    void resetToDefault() override {}

    static EasingListNode* create(EasingListSetting* value, float width) {
        auto ret = new EasingListNode();
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
