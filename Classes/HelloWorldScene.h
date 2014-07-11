#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "XTLayer.h"

class HelloWorld : public XTLayer
{
public:
    HelloWorld();
    virtual ~HelloWorld();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    virtual void xtTapGesture(cocos2d::Point position) override;
    virtual void xtDoubleTapGesture(cocos2d::Point position)  override;
    virtual void xtLongTapGesture(cocos2d::Point position)  override;
    virtual void xtSwipeGesture(XTTouchDirection direction, float distance, float speed)  override;

};

#endif // __HELLOWORLD_SCENE_H__
