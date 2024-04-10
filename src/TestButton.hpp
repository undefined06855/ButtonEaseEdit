#pragma once
#include <Geode/loader/SettingNode.hpp>

using namespace geode::prelude;

class TestButtonSetting : public SettingValue {
protected:
    std::string m_placeholder;
public:
    // lines 5, 8, 11, and 12 are copied from GDUtils
    TestButtonSetting(std::string const& key, std::string const& modID, std::string const& placeholder)
        : SettingValue(key, modID), m_placeholder(placeholder) {}
    bool load(matjson::Value const& json) override { return true; }
    bool save(matjson::Value& json) const override { return true; }
    SettingNode* createNode(float width) override;
};

class TestButtonNode : public SettingNode {
protected:
    bool init(TestButtonSetting* value, float width) {
        if (!SettingNode::init(value)) return false;

        this->setContentSize({ width, 130.f });

        CCMenu* menu = CCMenu::create();
        menu->setPosition(CCPoint{ 0, 0 });

        CCSprite* sprite = CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png");
        CCMenuItemSpriteExtra* button = CCMenuItemSpriteExtra::create(
            sprite, this, menu_selector(TestButtonNode::onTestButtonPressed)
        );
        button->setPosition(CCPoint{ width / 2, 130.f / 2 });
        menu->addChild(button);
        this->addChild(menu);

        return true;
    }

    void onTestButtonPressed(CCObject* sender) {

    }

public:
    void commit() override { this->dispatchCommitted(); }

    bool hasUncommittedChanges() override { return false; }
    bool hasNonDefaultValue() override { return false; }
    void resetToDefault() override {}

    static TestButtonNode* create(TestButtonSetting* value, float width) {
        auto ret = new TestButtonNode();
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
