#include "Hero.h"

Hero* Hero::createWithWorld(b2World* world)
{
	Hero* hero = new Hero();
	hero->initWithWorld(world);
	hero->autorelease();

	return hero; 
}

bool Hero::initWithWorld(b2World* world)
{
	_screenSize = Director::getInstance()->getWinSize();

	if(this->initWithSpriteFrameName("seal1.png"))
	{
		_world = world;
		this->createBody();
	}
		
	return true;
}

void Hero::createBody()
{
	float radius = 16.0f;
	int screenH = _screenSize.height;

	Point startPosition = Point(0 , screenH/2+radius);//torecover
	//Point startPosition = Point(0 , screenH/2 + 200);//torecover
	//Point startPosition = Point(0 , 0+radius);//torecover

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	//bd.linearDamping = 0.05f; // to comment out
	bd.fixedRotation = true;
	bd.position.Set(startPosition.x/PTM_RATIO, startPosition.y/PTM_RATIO);
	_body = _world->CreateBody(&bd);

	b2CircleShape shape;
	shape.m_radius = radius/PTM_RATIO;

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 1.0f; // CC_CONTENT_SCALE_FACTOR();
	fd.friction = 0;
	fd.restitution	= 0;

	_body->CreateFixture(&fd);

	//_body->SetLinearVelocity(b2Vec2(15, 0)); //to delete

}

void Hero::wake()
{
	_awake = true;
	_body->SetActive(true);
	_body->ApplyLinearImpulse(b2Vec2(1, 2), _body->GetPosition(), true);
}

void Hero::dive()
{
	//log("dive");
	_body->ApplyForce(b2Vec2(5, -50), _body->GetPosition(), true);
}

void Hero::limitVelocity()
{
	if(!_awake) return;
	//log("limitvelocity after !awake return");
	const float minVelocityX = 5;
	const float minVelocityY = -40;
	b2Vec2 vel = _body->GetLinearVelocity();
	if(vel.x < minVelocityX)
	{
		vel.x = minVelocityX;
	}
	if(vel.y < minVelocityY)
	{
		vel.y = minVelocityY;
	}
	_body->SetLinearVelocity(vel);
}

//test
/*
void Hero::stopAndRun()
{
	if(_stopAndRun == false)
	{
		_stopAndRun = true;
	}else
	{
		_stopAndRun = false;
	}

	if(_stopAndRun)
	{
		_body->SetLinearVelocity(b2Vec2(15, 0));
	}else
	{
		_body->SetLinearVelocity(b2Vec2(0, 0));
	}
}
*/

void Hero::update()
{
	this->setPosition(Point(_body->GetPosition().x*PTM_RATIO, _body->GetPosition().y*PTM_RATIO));
	b2Vec2 vel = _body->GetLinearVelocity();
	b2Vec2 weightedVe1 = vel;

	for(int i = 0; i < NUM_PREV_VELS; i++)
	{
		weightedVe1 += _preVels[i];
	}
	weightedVe1 = b2Vec2(weightedVe1.x/NUM_PREV_VELS, weightedVe1.y/NUM_PREV_VELS);
	_preVels[_nextVel++] = vel;
	if(_nextVel >= NUM_PREV_VELS) _nextVel = 0;

	float angle = atan2f(weightedVe1.y, weightedVe1.x);
	if(_awake)
	{
		this->setRotation(-1 * CC_RADIANS_TO_DEGREES(angle));
	}
}


Hero::Hero(void):
_awake(true),
_nextVel(0)
//,_stopAndRun(false)
{
	log("Hero()");
 
	for(int i = 0; i < 5; i++)
	{
		_preVels[i] = b2Vec2(0, 0);
	}
 
}


Hero::~Hero(void)
{
}

