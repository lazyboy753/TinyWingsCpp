#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Terrain.h"
#include "Box2D\Box2D.h"
#include "Constant.h"
#include "Hero.h"
USING_NS_CC;
 
class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

	Sprite* _backGround;

	Sprite* initSpriteWithColor(Color4F c1, float textureWidth, float textureHeight);

	Sprite* stripedSpriteWithColor1(Color4F c1, Color4F c2, float textureWidth, float textureHeight, int nStripes);

	Color4F randomBrightColor();

	void genBackGround();

    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event);
	virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
    virtual void onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event);

	void onEnter();
	void render(Color4F c2, float textureWidth, float textureHeight, int nStripes);
	void myRender();	

	void update(float dt);
  //  virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
	void setupWorld();
	void createTestBodyAtPosition(Point position);
	float distanceOfSegments(float distance, float segmentLength);

private:
	Size _screenSize;
	CustomCommand _customCommand;
	Terrain* _terrain; 
	b2World* _world;
	Hero* _hero;
	bool _tapDown;
	Sprite* _helloSprite;
};
#endif // __HELLOWORLD_SCENE_H__
