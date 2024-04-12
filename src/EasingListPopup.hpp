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
            CCLabelBMFont* indexLabel = CCLabelBMFont::create(std::to_string(i).c_str(), "bigFont.fnt");
            indexLabel->setPosition(CCPoint{ win.width / 2 - 5.f - 90.f, 250.f - (float)i * 12 });
            indexLabel->setScale(0.4f);
            indexLabel->setAnchorPoint(CCPoint{ 1.f, 0.5f });
            layer->addChild(indexLabel);

            CCLabelBMFont* easingLabel = CCLabelBMFont::create(easingList[i].c_str(), "bigFont.fnt", 350.f);
            easingLabel->setPosition(CCPoint{ win.width / 2 + 5.f - 90.f, 250.f - (float)i * 12 });
            easingLabel->setScale(0.4f);
            easingLabel->setAnchorPoint(CCPoint{ 0.f, 0.5f });
            layer->addChild(easingLabel);
        }

        CCSprite* spacer = CCSprite::createWithSpriteFrameName("floorLine_001.png");
        spacer->setScaleX(0.480f);
        spacer->setScaleY(0.75f);
        spacer->setPosition(CCPoint{ 195.f, 147.f });
        spacer->setRotation(90.f);
        layer->addChild(spacer);

        this->addChild(layer);

        return true;
    }

public:
    static EasingListPopup* create() {
        auto ret = new EasingListPopup();
        if (ret && ret->initAnchored(240.f, 265.f)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};
