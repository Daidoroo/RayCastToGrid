#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <list>

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

protected:
	cocos2d::CustomCommand cmd;

	cocos2d::Vec2 m_vTouchBegin;
	cocos2d::Vec2 m_vTouchEnd;

	std::list< cocos2d::Vec2 > m_Path;
	std::list< cocos2d::Vec2 > m_Pathf;
	enum eAxisIndex
	{
		X = 0,
		Z = 1
	};

	cocos2d::Vec2 m_vBackPos;

	void makePath();

public:
	virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags);
	virtual void onDraw(const cocos2d::Mat4 &transform, uint32_t flags);

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
};

#endif // __HELLOWORLD_SCENE_H__
