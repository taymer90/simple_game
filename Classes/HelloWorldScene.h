#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::CCLayerColor
{
protected:
    cocos2d::CCArray *_targets;
    cocos2d::CCArray *_projectiles;
public:
    ~HelloWorld();
    HelloWorld();

    void update(float dt);

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    void onTouchesEnded(const std::vector<cocos2d::Touch*>& pTouches,cocos2d::Event *pEvent);

    void addTarget();

    void spriteMoveFinished(cocos2d::CCNode* sender);

    void gameLogic(float dt);






};

#endif // __HELLOWORLD_SCENE_H__
