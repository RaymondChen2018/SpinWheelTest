#ifndef __BONUSWHEEL_SCENE_H
#define __BONUSWHEEL_SCENE_H

#include "cocos2d.h"
enum class PRIZE : int
{
    Life_30_min = 7,// 20%
    Brush_3X = 6,// 10%
    Gems_35 = 5,// 10%
    Hammer_3X = 4,// 10%
    Coins_750 = 3,// 5%
    Brush_1x = 2,// 20%
    Gems_75 = 1,// 5%
    Hammer_1X = 0,// 20%
};
USING_NS_CC;
class BonusWheelScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    static void UnitTest();
    virtual bool init();
    void pushPrizeIcon(const char* imagePath, const char* textPath, float angleDegree, Node* spinWheel);

    // a selector callback
    void resetWheel(cocos2d::Ref* pSender);
    void menuSpinCallback(cocos2d::Ref* pSender);
    static PRIZE rollPrize();
    void rollAnimation(PRIZE);
    void prizeAnimation(PRIZE);

    // implement the "static create()" method manually
    CREATE_FUNC(BonusWheelScene);

private:
    Vec2 center;
    Vec2 buttonPos;
};

#endif //__BONUSWHEEL_SCENE_H
