//
//// Created by merza on 4/13/2020.
////

#include "BonusWheelScene.h"
#include "SimpleAudioEngine.h"
//USING_NS_CC;

Scene* BonusWheelScene::createScene()
{
    return BonusWheelScene::create();
}
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// Place icon in the spin wheel slot
void parentPrizeItem(float angleDegree, Node* prizeIcon, Node* spinWheel)
{
    float centerX = spinWheel->getBoundingBox().size.width / 2.0f;
    float centerY = spinWheel->getBoundingBox().size.height / 2.0f;
    float offsetCenter = 0.6f * spinWheel->getBoundingBox().size.width / 2.0f;
    prizeIcon->setRotation(90.0f - angleDegree);

    float angleRadian = 3.14f * angleDegree / 180.0f;
    prizeIcon->setPosition(centerX + offsetCenter * cosf(angleRadian), centerY + offsetCenter * sinf(angleRadian));
}

// Create
void BonusWheelScene::pushPrizeIcon(const char* imagePath, const char* textPath, float angle, Node* spinWheel)
{
    auto prizeIcon = Sprite::create(imagePath);
    spinWheel->addChild(prizeIcon, 1);

    auto prizeHint = Sprite::create(textPath);
    prizeIcon->addChild(prizeHint, 1);
    prizeHint->setPosition(prizeIcon->getBoundingBox().size.width * 0.7f, prizeIcon->getBoundingBox().size.height * 0.1f);

    parentPrizeItem(angle, prizeIcon, spinWheel);
}

static Node* pSpinWheel;
Node* pLayerSpinWheel;
Node* pPlayOnButton;
Node* pResetButton;
// on "init" you need to initialize your instance
bool BonusWheelScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    this->center.x = visibleSize.width/2 + origin.x;
    this->center.y = visibleSize.height/2 + origin.y + 30.0f;

    // create spinning wheel
    auto layerSpinWheel = Layer::create();
    layerSpinWheel->setPosition(0.0f, 0.0f);
    this->addChild(layerSpinWheel, 0);

    auto spinWheel = Sprite::create("tapblaze/wheel_sections_8.png");
    layerSpinWheel->addChild(spinWheel, 0);
    // place prize icons
    float angleIncrement = 45.0f;
    float angleAggregate = 22.5f;
    pushPrizeIcon("tapblaze/brush.png", "tapblaze/x3.png", angleAggregate, spinWheel);
    angleAggregate += angleIncrement;
    pushPrizeIcon("tapblaze/heart.png", "tapblaze/30min.png", angleAggregate, spinWheel);
    angleAggregate += angleIncrement;
    pushPrizeIcon("tapblaze/hammer.png", "tapblaze/x1.png", angleAggregate, spinWheel);
    angleAggregate += angleIncrement;
    pushPrizeIcon("tapblaze/gem.png", "tapblaze/x75.png", angleAggregate, spinWheel);
    angleAggregate += angleIncrement;
    pushPrizeIcon("tapblaze/brush.png", "tapblaze/x1.png", angleAggregate, spinWheel);
    angleAggregate += angleIncrement;
    pushPrizeIcon("tapblaze/coin.png", "tapblaze/x750.png", angleAggregate, spinWheel);
    angleAggregate += angleIncrement;
    pushPrizeIcon("tapblaze/hammer.png", "tapblaze/x3.png", angleAggregate, spinWheel);
    angleAggregate += angleIncrement;
    pushPrizeIcon("tapblaze/gem.png", "tapblaze/x35.png", angleAggregate, spinWheel);
    // position spin wheel
    spinWheel->setPosition(center);
    spinWheel->setScale(0.8f,0.8f);
    // border
    auto spinWheelBorder = Sprite::create("tapblaze/wheel_border.png");
    spinWheelBorder->setScale(0.8f, 0.8f);
    spinWheelBorder->setPosition(center);
    layerSpinWheel->addChild(spinWheelBorder, 2);
    // arrow
    auto arrow = Sprite::create("tapblaze/wheel_arrow.png");
    arrow->setPosition(center.x, center.y + 0.9f * spinWheelBorder->getBoundingBox().size.height / 2.0f);
    layerSpinWheel->addChild(arrow, 3);


    // Buttons
    this->buttonPos.x = origin.x + visibleSize.width/2;
    this->buttonPos.y = origin.y + 50;

    // play button
    auto playOnButton = MenuItemImage::create("tapblaze/spin_button.png","tapblaze/spin_button.png",CC_CALLBACK_1(BonusWheelScene::menuSpinCallback, this));
    playOnButton->setPosition(buttonPos);
    auto playOnMenu = Menu::create(playOnButton, NULL);
    playOnMenu->setPosition(Vec2::ZERO);
    this->addChild(playOnMenu, 1);
    auto playOnLabel = Sprite::create("tapblaze/playOn.png");
    playOnButton->addChild(playOnLabel, 1);
    playOnLabel->setPosition(playOnButton->getBoundingBox().size.width / 2.0f, playOnButton->getBoundingBox().size.height / 2.0f);

    // reset button
    auto resetButton = MenuItemImage::create("tapblaze/spin_button.png","tapblaze/spin_button.png",CC_CALLBACK_1(BonusWheelScene::resetWheel, this));
    resetButton->setPosition(0.0f, -500.0f);
    auto resetMenu = Menu::create(resetButton, NULL);
    resetMenu->setPosition(Vec2::ZERO);
    this->addChild(resetMenu, 1);
    auto resetLabel = Sprite::create("tapblaze/reset.png");
    resetButton->addChild(resetLabel, 1);
    resetLabel->setPosition(resetButton->getBoundingBox().size.width / 2.0f, resetButton->getBoundingBox().size.height / 2.0f);


    pSpinWheel = spinWheel;
    pLayerSpinWheel = layerSpinWheel;
    pPlayOnButton = playOnButton;
    pResetButton = resetButton;
    return true;
}

#include <android/log.h>
Node* pPrizeSprite = nullptr;
void BonusWheelScene::menuSpinCallback(Ref* pSender)
{
    // Hide play button
    pPlayOnButton->setPosition(0.0f, -500.0f);

    // Roll chance
    PRIZE prize = rollPrize();
    printf("item: %i", prize);
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "Degree %i", (int)prize);

    // Roll Animation
    rollAnimation(prize);

    // Prize animation
    prizeAnimation(prize);
}

PRIZE BonusWheelScene::rollPrize()
{
    float seed = random(0.0f, 100.0f);

    if(seed <= 20.0f)
    {
        return PRIZE::Hammer_1X;
    }
    else if(seed <= 25.0f)
    {
        return PRIZE::Gems_75;
    }
    else if(seed <= 45.0f)
    {
        return PRIZE::Brush_1x;
    }
    else if(seed <= 50.0f)
    {
        return PRIZE::Coins_750;
    }
    else if(seed <= 60.0f)
    {
        return PRIZE::Hammer_3X;
    }
    else if(seed <= 70.0f)
    {
        return PRIZE::Gems_35;
    }
    else if(seed <= 80.0f)
    {
        return PRIZE::Brush_3X;
    }

    return PRIZE::Life_30_min;
}


void BonusWheelScene::rollAnimation(PRIZE prize)
{
    int iterations = 10;
    float totalDegree = (int)prize * 45.0f + 360.0f * iterations + 22.5f;

    float easeInDegree = totalDegree * 0.105f;
    float midDegree = totalDegree * 0.89f;
    float easeOutDegree = totalDegree * 0.005f;

    float totalDuration = 4.0f;
    float easeInDuration = totalDuration * 0.1f;
    float midDuration = totalDuration * 0.8f;
    float easeOutDuration = totalDuration * 0.1f;

    auto startRotate = RotateBy::create(easeInDuration, Vec3(0,0,easeInDegree));
    EaseBackIn * startRotateEaseBackIn = EaseBackIn::create(startRotate);

    auto rotate = RotateBy::create(midDuration, Vec3(0,0,midDegree));
    EaseOut* slowDownRotate = EaseOut::create(rotate, 5.0f);

    auto endRotate = RotateBy::create(easeOutDuration, Vec3(0,0,easeOutDegree));
    EaseBackOut* backRotate = EaseBackOut::create(endRotate);

    auto sequence = Sequence::create(startRotateEaseBackIn, slowDownRotate, backRotate, NULL);

    pSpinWheel->runAction(sequence);
}

void BonusWheelScene::resetWheel(cocos2d::Ref* pSender)
{
    pSpinWheel->setRotation(0.0f);
    pLayerSpinWheel->setPosition(0.0f, 0.0f);
    pPlayOnButton->setPosition(this->buttonPos);
    pResetButton->setPosition(0.0f, -500.0f);

    if(pPrizeSprite != nullptr)
    {
        this->removeChild(pPrizeSprite, true);
        pPrizeSprite = nullptr;
    }
}


void BonusWheelScene::prizeAnimation(PRIZE prize)
{
    float spinDuration = 4.0f;
    float pauseDuration = 0.3f;
    float prizeShowDuration = 2.0f;
    const char* prizeIconPath = nullptr;
    const char* prizeTextPath = nullptr;
    switch(prize)
    {
        case PRIZE::Life_30_min:
            prizeIconPath = "tapblaze/heart.png";
            prizeTextPath = "tapblaze/30min.png";
            break;
        case PRIZE::Brush_3X:
            prizeIconPath = "tapblaze/brush.png";
            prizeTextPath = "tapblaze/x3.png";
            break;
        case PRIZE::Gems_35:
            prizeIconPath = "tapblaze/gem.png";
            prizeTextPath = "tapblaze/x35.png";
            break;
        case PRIZE::Hammer_3X:
            prizeIconPath = "tapblaze/hammer.png";
            prizeTextPath = "tapblaze/x3.png";
            break;
        case PRIZE::Coins_750:
            prizeIconPath = "tapblaze/coin.png";
            prizeTextPath = "tapblaze/x750.png";
            break;
        case PRIZE::Brush_1x:
            prizeIconPath = "tapblaze/brush.png";
            prizeTextPath = "tapblaze/x1.png";
            break;
        case PRIZE::Gems_75:
            prizeIconPath = "tapblaze/gem.png";
            prizeTextPath = "tapblaze/x75.png";
            break;
        case PRIZE::Hammer_1X:
            prizeIconPath = "tapblaze/hammer.png";
            prizeTextPath = "tapblaze/x1.png";
            break;
    }

    // create prize icon
    auto prizeGlow = Sprite::create("tapblaze/glow.png");
    auto prizeIcon = Sprite::create(prizeIconPath);
    this->addChild(prizeIcon, 1);
    auto prizeHint = Sprite::create(prizeTextPath);
    prizeIcon->addChild(prizeHint, 1);
    prizeHint->setPosition(prizeIcon->getBoundingBox().size.width * 0.7f, prizeIcon->getBoundingBox().size.height * 0.1f);
    prizeIcon->addChild(prizeGlow, -1);
    prizeGlow->setPosition(prizeIcon->getBoundingBox().size.width / 2.0f, prizeIcon->getBoundingBox().size.height / 2.0f);
    prizeGlow->runAction(RotateBy::create(spinDuration + pauseDuration + prizeShowDuration, Vec3(0,0,1300.0f)));
    prizeGlow->runAction(EaseBackIn::create(ScaleTo::create(spinDuration + pauseDuration + prizeShowDuration, 0.01f)));
    prizeIcon->setPosition(0.0f, -500.0f);
    // move prize icon
    auto snapToPrizePosition = MoveTo::create(0.0f, Vec2(this->center.x, this->center.y + pSpinWheel->getBoundingBox().size.height * 0.6f/2.0f));
    auto movement = MoveTo::create(2.0f, this->center);
    auto scale = ScaleTo::create(2.0f, 2.0f, 2.0f);
    auto move_scale = Spawn::create(movement, scale, NULL);
    auto sequence = Sequence::create(DelayTime::create(spinDuration + pauseDuration), snapToPrizePosition, move_scale, NULL);
    prizeIcon->runAction(sequence);

    // Hide Wheel
    auto hideWheel = MoveTo::create(0.0f, Vec2(0.0f, -500.0f));
    auto sequence2 = Sequence::create(DelayTime::create(spinDuration + pauseDuration), hideWheel, NULL);
    pLayerSpinWheel->runAction(sequence2);

    // show reset
    auto showReset = MoveTo::create(0.0f, buttonPos);
    auto sequence3 = Sequence::create(DelayTime::create(spinDuration + pauseDuration + prizeShowDuration), showReset, NULL);
    pResetButton->runAction(sequence3);

    // Save pointer
    pPrizeSprite = prizeIcon;
}

void BonusWheelScene::UnitTest()
{
    int sample = 1000;

    int occuranceTable[8] = {0,0,0,0,0,0,0,0};
    for(int i=0;i<sample;i++)
    {
        PRIZE prize = rollPrize();
        int index = (int)prize;
        occuranceTable[index]++;
    }

    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "Spin Wheel Test, 1000 Spins: \n");

    int occurance = occuranceTable[(int)PRIZE::Brush_1x];
    float chance = 100.0f * occurance / (float)sample;
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "Brush_1x: %i, percentage: %f %c\n", occurance, chance, '%');

    occurance = occuranceTable[(int)PRIZE::Brush_3X];
    chance = 100.0f * occurance / (float)sample;
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "Brush_3X: %i, percentage: %f %c\n", occurance, chance, '%');

    occurance = occuranceTable[(int)PRIZE::Coins_750];
    chance = 100.0f * occurance / (float)sample;
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "Coins_750: %i, percentage: %f %c\n", occurance, chance, '%');

    occurance = occuranceTable[(int)PRIZE::Gems_35];
    chance = 100.0f * occurance / (float)sample;
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "Gems_35: %i, percentage: %f %c\n", occurance, chance, '%');

    occurance = occuranceTable[(int)PRIZE::Gems_75];
    chance = 100.0f * occurance / (float)sample;
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "Gems_75: %i, percentage: %f %c\n", occurance, chance, '%');

    occurance =  occuranceTable[(int)PRIZE::Hammer_1X];
    chance = 100.0f * occurance / (float)sample;
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "Hammer_1X: %i, percentage: %f %c\n", occurance, chance, '%');

    occurance = occuranceTable[(int)PRIZE::Hammer_3X];
    chance = 100.0f * occurance / (float)sample;
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "Hammer_3X: %i, percentage: %f %c\n", occurance, chance, '%');

    occurance = occuranceTable[(int)PRIZE::Life_30_min];
    chance = 100.0f * occurance / (float)sample;
    __android_log_print(ANDROID_LOG_DEBUG, "TRACKERS", "Life_30_min: %i, percentage: %f %c\n", occurance, chance, '%');
}


