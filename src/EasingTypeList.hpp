#pragma once
#include <Geode/loader/SettingNode.hpp>

using namespace geode::prelude;

class EasingListPopup : public geode::Popup<> {
protected:
    bool setup() override {
        auto win = CCDirector::sharedDirector()->getWinSize();

        // convenience function provided by Popup 
        // for adding/setting a title to the popup
        this->setTitle("Easing Types");

        CCMenu* layer = CCMenu::create();
        layer->setPosition(CCPoint{ 0, 0 });

        std::vector<std::string> easingList = {
            "Ease In Out",
            "Ease In",
            "Ease Out",
            "Elastic In Out",
            "Elastic In",
            "Elastic Out",
            "Bounce In Out",
            "Bounce In",
            "Bounce Out",
            "Exponential In Out",
            "Exponential In",
            "Exponential Out",
            "Sine In Out",
            "Sine In",
            "Sine Out",
            "Back In Out",
            "Back In",
            "Back Out",
        };

        // loop needs to be reversed
        for (int i = 0; i < easingList.size(); i++) {
            // the following magic numbers have all been picked at random
            CCLabelBMFont* indexLabel = CCLabelBMFont::create(std::to_string(i).c_str(), "bigFont.fnt", 350.f, CCTextAlignment::kCCTextAlignmentRight);
            indexLabel->setPosition(CCPoint{ win.width / 2, 250.f - (float)i * 12 });
            indexLabel->setScale(0.4f);
            indexLabel->setContentSize(CCSize{ 500.f, 32.5f });
            layer->addChild(indexLabel);

            CCLabelBMFont* easingLabel = CCLabelBMFont::create(easingList[i].c_str(), "bigFont.fnt", 350.f, CCTextAlignment::kCCTextAlignmentLeft);
            easingLabel->setPosition(CCPoint{ win.width / 2 + 80.f, 250.f - (float)i * 12 });
            easingLabel->setScale(0.4f);
            easingLabel->setContentSize(CCSize{ 480.f, 32.5f });
            layer->addChild(easingLabel);
        }

        this->addChild(layer);

        return true;
    }

public:
    static EasingListPopup* create() {
        auto ret = new EasingListPopup();
        if (ret && ret->init(240.f, 265.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class EasingListSetting : public SettingValue {
protected:
    std::string m_placeholder;
public:
    // lines 5, 8, 11, and 12 are copied from GDUtils
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

        log::info("init easinglistsetting");

        this->setContentSize({ width, 40.f });

        CCMenu* menu = CCMenu::create();
        menu->setPosition(CCPoint{ 0, 0 });

        ButtonSprite* spr = ButtonSprite::create("Easing types");
        CCMenuItemSpriteExtra* button = CCMenuItemSpriteExtra::create(spr, this, menu_selector(EasingListNode::showList));
        button->setPosition(CCPoint{ width / 2, 20.f });
        menu->addChild(button);

        this->addChild(menu);

        return true;
    }

    void showList(CCObject* sender) { EasingListPopup::create()->show(); }

public:
    void commit() override { this->dispatchCommitted(); }

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
