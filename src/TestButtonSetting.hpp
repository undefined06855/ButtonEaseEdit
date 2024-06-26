#pragma once
#include <Geode/loader/SettingNode.hpp>
#include <random>
std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> randomFireChance(1, 5);


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
    int selectedButtonIndex = 0;
    std::vector<CCMenuItemSpriteExtra*> buttons = {};

    bool init(TestButtonSetting* value, float width) {
        if (!SettingNode::init(value)) return false;

        this->setContentSize({ width, 150.f });

        CCMenu* menu = CCMenu::create();
        menu->setPosition(CCPoint{ 0, 0 });

        std::vector<CCSprite*> sprites = {
            CCSprite::createWithSpriteFrameName("GJ_playBtn_001.png"),
            ButtonSprite::create("Test button"),
            CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png"),
            CCSprite::createWithSpriteFrameName("GJ_createBtn_001.png"),
            CCSprite::createWithSpriteFrameName("GJ_logo_001.png"),
            CircleButtonSprite::createWithSpriteFrameName("diffIcon_01_btn_001.png", 0.9f, geode::CircleBaseColor::Green, geode::CircleBaseSize::MediumAlt)
        };

        sprites[4]->setScale(0.55); // scale down gd logo

        bool isFirstButton = true;

        for (CCSprite* sprite : sprites) {
            CCMenuItemSpriteExtra* button = CCMenuItemSpriteExtra::create(
                sprite, this, menu_selector(TestButtonNode::onTestButtonPressed)
            );
            button->setPosition(CCPoint{ width / 2, 65.f });
            button->setVisible(isFirstButton);
            menu->addChild(button);
            this->buttons.push_back(button);
            isFirstButton = false;
        }

        CCLabelBMFont* label = CCLabelBMFont::create("Test Buttons:", "bigFont.fnt");
        label->setPosition(CCPoint{ width / 2, 130.f });
        label->setZOrder(-1); // below the test button
        label->setScale(0.625f);
        menu->addChild(label);

        auto flippedArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
        flippedArrow->setFlipX(true);
        CCMenuItemSpriteExtra* nextButtonButton = CCMenuItemSpriteExtra::create(
            flippedArrow, this, menu_selector(TestButtonNode::onNextButtonPressed)
        );
        nextButtonButton->setPosition(CCPoint{ width - 25.f, 150.f / 2 });
        menu->addChild(nextButtonButton);

        CCMenuItemSpriteExtra* prevButtonButton = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"), this, menu_selector(TestButtonNode::onPrevButtonPressed)
        );
        prevButtonButton->setPosition(CCPoint{ 25.f, 150.f / 2 });
        menu->addChild(prevButtonButton);

        this->addChild(menu);

        return true;
    }

    void onTestButtonPressed(CCObject* sender) {
        if (randomFireChance(rng) != 1) return;

        FMODAudioEngine::sharedEngine()->playEffect("fire.ogg"_spr);
    }

    void updateButtonVisibilities() {
        int i = 0;
        for (CCMenuItemSpriteExtra* button : this->buttons) {
            button->setVisible(false);
            if (i == this->selectedButtonIndex) button->setVisible(true);
            i++;
        }
    }

    void onNextButtonPressed(CCObject* sender) {
        this->selectedButtonIndex++;
        if (this->selectedButtonIndex == this->buttons.size()) this->selectedButtonIndex = 0;
        this->updateButtonVisibilities();
    }

    void onPrevButtonPressed(CCObject* sender) {
        int i = 0;
        this->selectedButtonIndex--;
        if (this->selectedButtonIndex == -1) this->selectedButtonIndex = this->buttons.size() - 1;
        this->updateButtonVisibilities();
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
