#pragma once
#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include "GLES-Render.h"
#include "Constant.h"

#define kMaxHillKeyPoints 1000
#define kHillSegmentWidth 10
#define kMaxHillVertices 4000
#define kMaxBorderVertices 5000
 
USING_NS_CC;

class Terrain : public Node
{
public:
 	CREATE_FUNC(Terrain);

	void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	void render(const Mat4& transform, uint32_t flags);
	CustomCommand _customCommand;
	CustomCommand _debugCommand;
	void debugRender(const Mat4& transform, uint32_t flags);
	void resetHillVertices();

	Terrain(void);
	~Terrain(void);

	int _fromKeyPointI;
	int _toKeyPointI;
	int _nHillVertices;

	int _nBorderVertices;
 	bool initWithWorld(b2World* world);
	static Terrain* createTerrainWithWorld(b2World* world);
	void resetBox2DBody();
	void setupDebugDraw();
	//SpriteBatchNode* _batchNode;
	void createHillBody();
	void generateBorderVertices();

	float decimalPreservedDiv(float dividend, float divisor);


protected: float offsetX;
public: virtual float getOffsetX(void) const {return offsetX;}
public: virtual void setOffsetX(float var);
private:
	b2World* _world;
 	int _OffsetX;
	Point _hillKeyPoints[kMaxHillKeyPoints];
	Point _hillVertices[kMaxHillVertices];
	Point _hillTexCoords[kMaxHillVertices];
	Color4F _hillTexColor[kMaxHillVertices];
	Point _borderVertices[kMaxBorderVertices];

	CC_SYNTHESIZE_RETAIN(Sprite*, _stripes, _stripes);
	void generateHills();
	Size _screenSize;
	b2Body* _body;
	b2Body* _bodyTest;
	GLESDebugDraw* _debugDraw;


};

