#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( CCLayer::init() )
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        Size visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();


        _targets = new CCArray;
        _projectiles = new CCArray;


        /////////////////////////////
        // 2. add a menu item with "X" image, which is clicked to quit the program
        //    you may modify it.

        // add a "close" icon to exit the progress. it's an autorelease object
        auto closeItem = MenuItemImage::create(
                                               "CloseNormal.png",
                                               "CloseSelected.png",
                                               CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                    origin.y + closeItem->getContentSize().height/2));

        // create menu, it's an autorelease object
        auto menu = Menu::create(closeItem, NULL);
        menu->setPosition(Vec2::ZERO);
        this->addChild(menu, 1);

        // creating player
        CCSprite *player = CCSprite::create("Player.png",CCRectMake(0, 0, 27, 40) );
        player->setPosition( ccp(player->getContentSize().width/2, winSize.height/2) );
        if( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)) ) //RGBA
            {
                return false;
            }

        this->setTouchEnabled(true);
        this->addChild(player);
        this->schedule( schedule_selector(HelloWorld::gameLogic), 1.1 );
        this->schedule( schedule_selector(HelloWorld::update) );
    }
    return true;
}




void HelloWorld::update(float dt)
{
    CCArray *projectilesToDelete = new CCArray;
    CCArray* targetsToDelete =new CCArray;
    CCObject* it = NULL;
    CCObject* jt = NULL;
/*
    CCARRAY_FOREACH(_projectiles, it)
    {
        CCSprite *projectile = dynamic_cast<CCSprite*>(it);
        CCRect projectileRect = CCRectMake(
                                           projectile->getPosition().x - (projectile->getContentSize().width/2),
                                           projectile->getPosition().y - (projectile->getContentSize().height/2),
                                           projectile->getContentSize().width,
                                           projectile->getContentSize().height);


        CCARRAY_FOREACH(_targets, jt)
        {
            CCSprite *target = dynamic_cast<CCSprite*>(jt);
            CCRect targetRect = CCRectMake(
                                           target->getPosition().x - (target->getContentSize().width/2),
                                           target->getPosition().y - (target->getContentSize().height/2),
                                           target->getContentSize().width,
                                           target->getContentSize().height);

            if (projectileRect.intersectsRect(targetRect))
            {
                targetsToDelete->addObject(target);
                projectilesToDelete->addObject(projectile);
            }
        }
    }

    CCARRAY_FOREACH(targetsToDelete, jt)
    {
        CCSprite *target = dynamic_cast<CCSprite*>(jt);
        //_target->removeObject(target);
        this->removeChild(target, true);
    }

    CCARRAY_FOREACH(projectilesToDelete, it)
    {
        CCSprite* projectile = dynamic_cast<CCSprite*>(it);
        //_bullet->removeObject(projectile);
        this->removeChild(projectile, true);
    }


    projectilesToDelete->release();
    targetsToDelete->release();
*/
}



//*******************************************************************
HelloWorld::~HelloWorld()
{
  if (_targets)
  {
    _targets->release();
    _targets = NULL;
  }

  if (_projectiles)
  {
    _projectiles->release();
    _projectiles = NULL;
  }

  // cpp don't need to call super dealloc
  // virtual destructor will do this
}

HelloWorld::HelloWorld()
{
	_targets = NULL;
	_projectiles = NULL;

}


//***************************************************	gameLogic
void HelloWorld::gameLogic(float dt)
{
    this->addTarget();
}

void HelloWorld::spriteMoveFinished(CCNode* sender)
{
  CCSprite *sprite = (CCSprite *)sender;
  this->removeChild(sprite, true);
/*
  if (sprite->getTag() == 1)  // target
    {
      _targets->removeObject(sprite);
    }
    else if (sprite->getTag() == 2) // projectile
    {
      _projectiles->removeObject(sprite);
    }
*/
}

//***************************************************	ccTouchesEnded
void HelloWorld::onTouchesEnded(const std::vector<Touch*>& pTouches,Event *pEvent)
{
    // Choose one of the touches to work with
    Touch* touch = (Touch*)pTouches[0];
    CCPoint location = touch->getLocationInView();
    location = CCDirector::sharedDirector()->convertToGL(location);

    // Set up initial location of projectile
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSprite *projectile = CCSprite::create("Projectile.png",
        CCRectMake(0, 0, 20, 20));
    projectile->setPosition( ccp(20, winSize.height/2) );

        //projectile->get;


    // Determinie offset of location to projectile
    int offX = location.x - projectile->getPosition().x;
    int offY = location.y - projectile->getPosition().y;

    // Bail out if we are shooting down or backwards
    if (offX <= 0) return;

    // Ok to add now - we've double checked position
    this->addChild(projectile);

    // Determine where we wish to shoot the projectile to
    int realX = winSize.width
                         + (projectile->getContentSize().width/2);
    float ratio = (float)offY / (float)offX;
    int realY = (realX * ratio) + projectile->getPosition().y;
    CCPoint realDest = ccp(realX, realY);

    // Determine the length of how far we're shooting
    int offRealX = realX - projectile->getPosition().x;
    int offRealY = realY - projectile->getPosition().y;
    float length = sqrtf((offRealX * offRealX)
                                        + (offRealY*offRealY));
    float velocity = 480/1; // 480pixels/1sec
    float realMoveDuration = length/velocity;

    _projectiles->addObject(projectile);

    // Move projectile to actual endpoint
    projectile->runAction( CCSequence::create(
        CCMoveTo::create(realMoveDuration, realDest),
        CCCallFuncN::create(this,

        callfuncN_selector(HelloWorld::spriteMoveFinished)),
        NULL) );
}

//***************************************************	addTarget
void HelloWorld::addTarget()
{
    CCSprite *target = CCSprite::create("Target.png",
        CCRectMake(0,0,27,40) );
    	//target->setTag(1);
    	//_targets->addObject(target);

    // Determine where to spawn the target along the Y axis
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    int minY = target->getContentSize().height/2;
    int maxY = winSize.height
                          -  target->getContentSize().height/2;
    int rangeY = maxY - minY;
    // srand( TimGetTicks() );
    int actualY = ( rand() % rangeY ) + minY;

    // Create the target slightly off-screen along the right edge,
    // and along a random position along the Y axis as calculated
    target->setPosition( ccp(winSize.width + (target->getContentSize().width/2), actualY) );
    this->addChild(target);

    // Determine speed of the target
    int minDuration = (int)2.0;
    int maxDuration = (int)4.0;
    int rangeDuration = maxDuration - minDuration;
    // srand( TimGetTicks() );
    int actualDuration = ( rand() % rangeDuration )
                                        + minDuration;


    // Create the actions
    CCFiniteTimeAction* actionMove =
        CCMoveTo::create( (float)actualDuration, ccp(0 - target->getContentSize().width/2, actualY) );
    CCFiniteTimeAction* actionMoveDone = CCCallFuncN::create( this, callfuncN_selector(HelloWorld::spriteMoveFinished));
    target->runAction( CCSequence::create(actionMove, actionMoveDone, NULL) );
}



void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
