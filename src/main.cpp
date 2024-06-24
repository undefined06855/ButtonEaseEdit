#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
#include "TestButton.hpp"
#include "EasingTypeList.hpp"
using namespace geode::prelude;

// needs to be defined somewhere after TestButtonSetting
SettingNode* TestButtonSetting::createNode(float width) { return TestButtonNode::create(this, width); }
// same as above
SettingNode* EasingListSetting::createNode(float width) { return EasingListNode::create(this, width); }

// set if a button is being selected or unselected
bool isSelectingOrUnselecting = false;

/*
Easings (taken from geode enums, subtract one):
	EaseInOut = 0,
	EaseIn = 1,
	EaseOut = 2,
	ElasticInOut = 3,
	ElasticIn = 4,
	ElasticOut = 5,
	BounceInOut = 6,
	BounceIn = 7,
	BounceOut = 8,
	ExponentialInOut = 9,
	ExponentialIn = 10,
	ExponentialOut = 11,
	SineInOut = 12,
	SineIn = 13,
	SineOut = 14,
	BackInOut = 15,
	BackIn = 16,
	BackOut = 17,
*/

// TRUST ME I TRIED $MODIFY IT JUST DID NOT WANT TO WORK :sob:
CCActionEase* CCEaseBounceOut_create(CCActionInterval* pAction) {
	// default behaviour (a button isn't being pressed)
	log::info("Trying to get a CCEaseBounceOut");
	if (!isSelectingOrUnselecting) return CCEaseBounceOut::create(pAction);
	
	int64_t chosenEasing = Mod::get()->getSettingValue<int64_t>("chosen-easing");
	double easingRate = Mod::get()->getSettingValue<double>("easing-rate");
	double easingLength = Mod::get()->getSettingValue<double>("easing-length");
	double easingAmplitude = Mod::get()->getSettingValue<double>("easing-amplitude");
#ifndef GEODE_IS_ANDROID
	// this just doesnt exist on android?? like what? why???
	double easingSpeedMod = Mod::get()->getSettingValue<double>("easing-speedMod");
	pAction->setSpeedMod(easingSpeedMod);
#endif
	pAction->setDuration(easingLength);
	pAction->setAmplitudeRate(easingAmplitude);

	log::info("switching to easing {}", chosenEasing);

	switch (chosenEasing) {
		case 0 : return CCEaseInOut::create(pAction, easingRate);
		case 1 : return CCEaseIn::create(pAction, easingRate);
		case 2 : return CCEaseOut::create(pAction, easingRate);

		case 3 : return CCEaseElasticInOut::create(pAction, easingRate);
		case 4 : return CCEaseElasticIn::create(pAction, easingRate);
		case 5 : return CCEaseElasticOut::create(pAction, easingRate);

		case 6 : return CCEaseBounceInOut::create(pAction);
		case 7 : return CCEaseBounceIn::create(pAction);
		case 8 : return CCEaseBounceOut::create(pAction);

		case 9 : return CCEaseExponentialInOut::create(pAction);
		case 10: return CCEaseExponentialIn::create(pAction);
		case 11: return CCEaseExponentialOut::create(pAction);

		case 12: return CCEaseSineInOut::create(pAction);
		case 13: return CCEaseSineIn::create(pAction);
		case 14: return CCEaseSineOut::create(pAction);

		case 15: return CCEaseBackInOut::create(pAction);
		case 16: return CCEaseBackIn::create(pAction);
		case 17: return CCEaseBackOut::create(pAction);

		default:
			// this should never actually get to here so if it does then
			// there's something wrong
			log::info("What the fuck? {}", chosenEasing);
			Mod::get()->setSettingValue<int64_t>("chosen-easing", 8);
			return CCEaseBounceOut::create(pAction);
	}
}

class $modify(CCMenuItemSpriteExtra) {
	void selected() {
		isSelectingOrUnselecting = true;
		CCMenuItemSpriteExtra::selected();
		isSelectingOrUnselecting = false;
	}

	void unselected() {
		isSelectingOrUnselecting = true;
		CCMenuItemSpriteExtra::unselected();
		isSelectingOrUnselecting = false;
	}
};

$execute {
	Mod::get()->hook(
		reinterpret_cast<void*>(geode::addresser::getNonVirtual(&CCEaseBounceOut::create)),
		&CCEaseBounceOut_create,
		"CCEaseBounceOut::create",
		tulip::hook::TulipConvention::Cdecl
	);
}

$on_mod(Loaded) {
	Mod::get()->addCustomSetting<TestButtonSetting>("test-button", "");
	Mod::get()->addCustomSetting<EasingListSetting>("easing-list", "");
}
