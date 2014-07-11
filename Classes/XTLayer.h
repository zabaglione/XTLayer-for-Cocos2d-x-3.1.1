/* Copyright (c) 2013      Edwin van Poeijer
 
 http://www.vanpoeijer.com
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __XTLayer__
#define __XTLayer__

#include "cocos2d.h"

class XTLayer : public cocos2d::Layer
{
public:
    enum XTTouchDirection { LEFT,RIGHT,UP,DOWN };
    enum XTGesture { TAP,LONGTAP,SWIPE };
    
    XTLayer();
    virtual ~XTLayer();
    
private:
    XTTouchDirection _xtTouchDirection;

    // Default settings
    float _xtSwipeThreshold;
    float _xtSwipeTime;
    float _xtTapThreshold;
    float _xtTapTime;
    float _xtLongTapTreshold;
    float _xtLongTapTime;
    float _xtDoubleTapTime;
    
    // Touch positions
    cocos2d::Point xtGestureStartPoint;
    cocos2d::Point xtActualPoint;
    cocos2d::Point xtPreviousPoint;
    cocos2d::Point xtGestureEndPoint;
    
    bool  _touchHasMoved;
    long  _xtTouchStart;
    int   _xtNumberOfTaps;
    long long  millisecondNow();
    void  tapHandler(float dt);
    
public:
    // Gesture settings
    float xtSwipeThreshold()             { return _xtSwipeThreshold;   }
    void  xtSwipeThreshold(float value)  { _xtSwipeThreshold -= value; }
    float xtSwipeTime()                  { return _xtSwipeTime;        }
    void  xtSwipeTime(float value)       { _xtSwipeTime = value;       }
    float xtTapTreshold()                { return _xtTapThreshold;     }
    void  xtTapTreshold(float value)     { _xtTapThreshold = value;    }
    float xtTapTime()                    { return _xtTapTime;          }
    void  xtTapTime(float value)         { _xtTapTime = value;         }
    float xtLongTapTreshold()            { return _xtLongTapTreshold;  }
    void  xtLongTapTreshold(float value) { _xtLongTapTreshold = value; }
    float xtLongTapTime()                { return _xtLongTapTime;      }
    void  xtLongTapTime(float value)     { _xtLongTapTime = value;     }
    float xtDoubleTapTime()              { return _xtDoubleTapTime;    }
    void  xtDoubleTapTime(float value)   { _xtDoubleTapTime = value;   }
    
    
    // Override of touches
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* unused_event) override;

    virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* unused_event) override;
    virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* unused_event) override;
    virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* unused_event) override;
    virtual void onTouchesCancelled(const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* unused_event) override {}
    
    // Callbacks
    virtual void xtTouchesBegan(const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* event){}
    virtual void xtTouchesMoved(const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* event){}
    virtual void xtTouchesEnded(const std::vector<cocos2d::Touch*>& _touches, cocos2d::Event* event){}

    // Easy touch callbacks
    virtual void xtTouchesBegan(cocos2d::Point position){}
    virtual void xtTouchesMoved(cocos2d::Point position){}
    virtual void xtTouchesEnded(cocos2d::Point position){}
    
    // Gesture Callbacks
    virtual void xtTapGesture(cocos2d::Point position) {}
    virtual void xtDoubleTapGesture(cocos2d::Point position) {}
    virtual void xtLongTapGesture(cocos2d::Point position) {}
    virtual void xtSwipeGesture(XTTouchDirection direction, float distance, float speed) {}
    
    
    // v3.0 helper
    virtual void xtTouchEnabled(bool enabled);
    virtual void xtTouchMode(cocos2d::Touch::DispatchMode mode);
    
    // v3.0 event
    virtual void onExit() override;
};

#endif /* defined(__XTLayer__) */
