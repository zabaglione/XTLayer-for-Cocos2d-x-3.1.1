//
//  XTLayer.cpp
//  quadro
//
//  Created by EAA van Poeijer on 12-02-13.
//
//

#include "XTLayer.h"

USING_NS_CC;

XTLayer::XTLayer()
: Layer()
{
    this->_xtSwipeThreshold  =   30;
    this->_xtSwipeTime       =  500;
    this->_xtTapThreshold    =   10;
    this->_xtTapTime         =  250;
    this->_xtLongTapTreshold =   10;
    this->_xtLongTapTime     = 2000;
    this->_xtDoubleTapTime   =  250;
    this->_xtNumberOfTaps    =    0;
}

XTLayer::~XTLayer()
{
}

long long XTLayer::millisecondNow()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    return (now.tv_sec * (long long)1000 + now.tv_usec / (long long)1000);
}

// Override of touches
bool XTLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    std::vector<Touch*> touches;
    touches.push_back(touch);
    this->onTouchesBegan(touches, unused_event);
    
    return true;
}

void XTLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    std::vector<Touch*> touches;
    touches.push_back(touch);
    this->onTouchesMoved(touches, unused_event);
}

void XTLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    std::vector<Touch*> touches;
    touches.push_back(touch);
    this->onTouchesEnded(touches, unused_event);
}

void XTLayer::onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* unused_event)
{
    std::vector<Touch*> touches;
    touches.push_back(touch);
    this->onTouchesCancelled(touches, unused_event);
}

void XTLayer::onTouchesBegan(const std::vector<Touch*>& _touches, cocos2d::Event* event)
{
    this->_touchHasMoved = false;
    this->_xtTouchStart  = this->millisecondNow();
    
    auto fingerOne = _touches.at(0);
    Point pointOne = CCDirector::getInstance()->convertToUI(fingerOne->getLocationInView());
    
    Point location = this->convertToNodeSpace(pointOne);
    this->xtGestureStartPoint= location;//touch locationInView:touch->view);
    this->xtPreviousPoint = location;
   
    // Passthrough
    this->xtTouchesBegan(location);
    this->xtTouchesBegan(_touches, event);

}

void XTLayer::onTouchesMoved(const std::vector<Touch *>& _touches, cocos2d::Event* event)
{
    this->_touchHasMoved = true;
    
    auto fingerOne = _touches.at(0);
    Point pointOne = CCDirector::getInstance()->convertToUI(fingerOne->getLocationInView());
    
    Point location = this->convertToNodeSpace(pointOne);
    this->xtActualPoint = location;
    
    // log("Position:%f,%f",location.x,location.y);
    // Passthrough
    this->xtTouchesMoved(location);
    this->xtTouchesMoved(_touches, event);

}

void XTLayer::onTouchesEnded(const std::vector<Touch *>& _touches, cocos2d::Event* event)
{
    long endTime   = this->millisecondNow();
    long deltaTime = endTime - this->_xtTouchStart;
    // log("Deltatime %ld",deltaTime);
    
    auto fingerOne = _touches.at(0);
    Point pointOne = CCDirector::getInstance()->convertToUI(fingerOne->getLocationInView());
    
    Point location = this->convertToNodeSpace(pointOne);
    this->xtGestureEndPoint= location;//touch locationInView:touch->view);

    // Calculate the distance
    float deltaX = this->xtGestureStartPoint.x - this->xtGestureEndPoint.x;
    float deltaY = this->xtGestureStartPoint.y - this->xtGestureEndPoint.y;
    
    Size screenSize = CCDirector::getInstance()->getWinSize();
    
    float horSwipeDistancePercentage = fabs((deltaX / screenSize.width ) * 100);
    float verSwipeDistancePercentage = fabs((deltaY / screenSize.height) * 100);
    
    // Calculate the direction
    // First horizontal or vertical
    if (fabs(deltaX) > fabs(deltaY) && horSwipeDistancePercentage > this->_xtSwipeThreshold && deltaTime < this->_xtSwipeTime) // horizontal
    {
        if (deltaX < 0)
            this->_xtTouchDirection = LEFT;
        if (deltaX > 0)
            this->_xtTouchDirection = RIGHT;
        float speed = fabs(deltaX) / deltaTime;
        this->xtSwipeGesture(this->_xtTouchDirection, fabs(deltaX), speed);
    }
    else if (fabs(deltaX) < fabs(deltaY) && verSwipeDistancePercentage > this->_xtSwipeThreshold && deltaTime <this->_xtSwipeTime)// Vertical
    {
        if (deltaY < 0)
            this->_xtTouchDirection = DOWN;
        if (deltaY > 0)
            this->_xtTouchDirection = UP;
        float speed = fabs(deltaY) / deltaTime;
        this->xtSwipeGesture(this->_xtTouchDirection, fabs(deltaY), speed);
    }
    else if (deltaTime >= this->_xtLongTapTime)// No movement, tap detected
    {
        this->xtLongTapGesture(location);
    }
    else
        {
            if (this->_xtNumberOfTaps == 0)
            {
                this->_xtTouchStart = this->millisecondNow();
                this->schedule(schedule_selector(XTLayer::tapHandler), (this->_xtDoubleTapTime / 1000), 1, 0);
            }
            this->_xtNumberOfTaps++;
    }

    this->xtTouchesEnded(location);
    this->xtTouchesEnded(_touches, event);
}

void XTLayer::tapHandler(float dt)
{
    this->unschedule(schedule_selector(XTLayer::tapHandler));

    if (this->_xtNumberOfTaps == 1 && !this->_touchHasMoved) // singletap
    {
        this->_xtNumberOfTaps = 0;
        this->xtTapGesture(this->xtGestureEndPoint);
    }
    else if (this->_xtNumberOfTaps == 2) // double tap
    {
        this->_xtNumberOfTaps = 0;
        this->xtDoubleTapGesture(this->xtGestureEndPoint);
    }
    else
    {
        this->_xtNumberOfTaps = 0;
    }
}

/*
 *  for v3.0
 */

/*!
 *  @brief  xtTouchEnabled
 *  - see Layer::setTouchEnabled
 */
void XTLayer::xtTouchEnabled(bool enabled)
{
    if (_touchEnabled != enabled)
    {
        _touchEnabled = enabled;
        if (enabled)
        {
            if (_touchListener != nullptr)
                return;
            
            if (_touchMode == Touch::DispatchMode::ALL_AT_ONCE)
            {
                // Register Touch Event
                auto listener = EventListenerTouchAllAtOnce::create();
                
                listener->onTouchesBegan = CC_CALLBACK_2(XTLayer::onTouchesBegan, this);
                listener->onTouchesMoved = CC_CALLBACK_2(XTLayer::onTouchesMoved, this);
                listener->onTouchesEnded = CC_CALLBACK_2(XTLayer::onTouchesEnded, this);
                listener->onTouchesCancelled = CC_CALLBACK_2(XTLayer::onTouchesCancelled, this);
                
                _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
                _touchListener = listener;
            }
            else
            {
                // Register Touch Event
                auto listener = EventListenerTouchOneByOne::create();
                listener->setSwallowTouches(_swallowsTouches);
                
                listener->onTouchBegan = CC_CALLBACK_2(XTLayer::onTouchBegan, this);
                listener->onTouchMoved = CC_CALLBACK_2(XTLayer::onTouchMoved, this);
                listener->onTouchEnded = CC_CALLBACK_2(XTLayer::onTouchEnded, this);
                listener->onTouchCancelled = CC_CALLBACK_2(XTLayer::onTouchCancelled, this);
                
                _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
                _touchListener = listener;
            }
        }
        else
        {
            _eventDispatcher->removeEventListener(_touchListener);
            _touchListener = nullptr;
        }
    }
}

/*!
 *  @brief  xtTouchMode
 *  - see Layer::setTouchMode
 */
void XTLayer::xtTouchMode(Touch::DispatchMode mode)
{
    if(_touchMode != mode)
    {
        _touchMode = mode;
        
        if (_touchEnabled)
        {
            xtTouchEnabled(false);
            xtTouchEnabled(true);
        }
    }
}

/*!
 *  @brief  onExit
 *
 *  remove _touchListener from _eventDispatcher
 */
void XTLayer::onExit()
{
    xtTouchEnabled(false);
    Layer::onExit();
}
