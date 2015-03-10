#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "Constant.h"

USING_NS_CC;

class Hero : public Sprite
{
public: 
	static Hero* createWithWorld(b2World* world);
	bool initWithWorld(b2World* world);
	void update();
	void createBody();
	void wake();
	void dive();
	void limitVelocity();
	void stopAndRun();
	Hero(void);
	~Hero(void);
private:
	b2World* _world;
	b2Body* _body;
	CC_SYNTHESIZE_READONLY(bool, _awake, _awake);
	Size _screenSize;
	b2Vec2 _preVels[NUM_PREV_VELS];
	int _nextVel;
	//bool _stopAndRun;
};

