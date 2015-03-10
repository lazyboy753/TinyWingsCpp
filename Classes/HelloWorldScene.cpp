#include "HelloWorldScene.h"

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

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    _screenSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(HelloWorld::onTouchesCancelled, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->setupWorld();


	//this->createTestBodyAtPosition(Point(_screenSize.width/2, _screenSize.height/2));

	_terrain = Terrain::createTerrainWithWorld(_world);
	
	this->genBackGround();

	this->addChild(_terrain, 1);

	_terrain->setOffsetX(0);

	_hero = Hero::createWithWorld(_world);
	_terrain->addChild(_hero);
	//_helloSprite = Sprite::create("Icon-Small.png");
	//_helloSprite = Sprite::create("noise333.png");
	//_helloSprite->setScaleX(_screenSize.width / _helloSprite->getContentSize().width);
	// _helloSprite->setScaleY(_screenSize.height / _helloSprite->getContentSize().height);	
	//_helloSprite->setPosition(Point(_screenSize.width + _helloSprite->getContentSize().width/2, _screenSize.height / 2));

	//_helloSprite->setPosition(Point(-1 * _helloSprite->getContentSize().width/2, _screenSize.height/2));
	//this->addChild(_helloSprite, 1);

	this->scheduleUpdate();
	
	return true;
}

Sprite* HelloWorld::initSpriteWithColor(Color4F c1, float textureWidth, float textureHeight)
{
	 
	Sprite* noise = Sprite::create("noise444.png");
	BlendFunc blendFunc = {GL_DST_COLOR, GL_ZERO};
	noise->setBlendFunc(blendFunc);
	 
	noise->setPosition(textureWidth * 0.5f , textureHeight * 0.5f);
	noise->setFlippedY(true);
	
	auto rt = RenderTexture::create(textureWidth, textureHeight);
	 /*
	rt->setKeepMatrix(true);
	Size pixelSize = Director::getInstance()->getWinSizeInPixels();
	rt->setVirtualViewport(Point(0, 0), Rect(0, 0, _screenSize.width, _screenSize.height), Rect(0, 0, pixelSize.width, pixelSize.height));
	  */
	rt->beginWithClear(c1.r, c1.g, c1.b, c1.a);
	noise->visit();
	rt->end();
	auto sprite = Sprite::createWithTexture(rt->getSprite()->getTexture());

	return sprite;
}

Sprite* HelloWorld::stripedSpriteWithColor1(Color4F c1, Color4F c2, float textureWidth, float textureHeight, int nStripes)
{
	auto render = Director::getInstance()->getRenderer();
	 
	Sprite* noise = Sprite::create("noise333.png");
	BlendFunc blendFunc = {GL_DST_COLOR, GL_ZERO};
	noise->setBlendFunc(blendFunc);
	noise->setPosition(Point(textureWidth / 2 , textureHeight / 2));
  
	auto rt = RenderTexture::create(textureWidth, textureHeight);
	rt->beginWithClear(c1.r, c1.g, c1.b, c1.a);

	_customCommand.init(rt->getGlobalZOrder());
	_customCommand.func = CC_CALLBACK_0(HelloWorld::render, this, c2, textureWidth, textureHeight, nStripes);
	render->addCommand(&_customCommand);

	noise->visit();
	rt->end();

	auto sprite = Sprite::createWithTexture(rt->getSprite()->getTexture());
	//auto sprite = Sprite::createWithTexture(noise->getTexture());

	return sprite;
}

Color4F HelloWorld::randomBrightColor()
{
	while(true)
	{
		float requiredBrightness = 192;
		Color4B randomColor = 
			Color4B(std::rand() % 255, std::rand() % 255, std::rand() % 255, 255);
		if(randomColor.r > requiredBrightness ||
		   randomColor.g > requiredBrightness ||
		   randomColor.b > requiredBrightness)
		{
			return Color4F(randomColor);
		}
	}
}

void HelloWorld::genBackGround()
{
	srand(time(NULL));
 
	auto bgColor = this->randomBrightColor();
	auto color2 = this->randomBrightColor();
 
	_backGround = this->initSpriteWithColor(bgColor, 512, 512);
 
	_backGround->setPosition(Point(_screenSize.width / 2, _screenSize.height / 2));
 
	//Texture2D::TexParams tp = {GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE};
	Texture2D::TexParams  tp = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};

	_backGround->getTexture()->setTexParameters(&tp);

	_backGround->setScaleX(_screenSize.width / _backGround->getContentSize().width);
	_backGround->setScaleY(_screenSize.height / _backGround->getContentSize().height);
 
 	this->addChild(_backGround, -1);
 
	Color4F color3 = this->randomBrightColor();
	Color4F color4 = this->randomBrightColor();

	Sprite* stripes = this->stripedSpriteWithColor1(color3, color4, 512, 512, 4);
	stripes->setPosition(Point(_screenSize.width / 2, _screenSize.height / 2));
	Texture2D::TexParams tp2 = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_CLAMP_TO_EDGE};
	stripes->getTexture()->setTexParameters(&tp2);
	_terrain->set_stripes(stripes);
}

void HelloWorld::createTestBodyAtPosition(Point position)
{
	b2BodyDef testBodyDef;
	testBodyDef.type = b2_dynamicBody;
	testBodyDef.position.Set(position.x/PTM_RATIO, position.y/PTM_RATIO);
	b2Body* testBody = _world->CreateBody(&testBodyDef);

	b2CircleShape testBodyShape;
	b2FixtureDef testFixtureDef;
	testBodyShape.m_radius = 25.0/PTM_RATIO;
	testFixtureDef.shape = &testBodyShape;
	testFixtureDef.density = 1.0;
	testFixtureDef.friction = 0.2;
	testFixtureDef.restitution = 0.5;
	//testFixtureDef.isSensor = false;
	testBody->CreateFixture(&testFixtureDef);
}


void HelloWorld::setupWorld()
{
	b2Vec2 gravity = b2Vec2(0.0f, -7.0f);
	bool doSleep = true;
	_world = new b2World(gravity);
	_world->SetAllowSleeping(doSleep);
}

void HelloWorld::onEnter()
{
	log("entered");
	Node::onEnter();
}

void HelloWorld::onTouchesBegan(const std::vector<Touch*>& touches, Event *unused_event)
{	
	//Point pos = touches->getLocation();
	//Point posInTerrainSpace = _terrain->convertToNodeSpace(pos);
	//this->createTestBodyAtPosition(posInTerrainSpace);

	//_backGround->removeFromParentAndCleanup(true);
	
	//this->genBackGround();

	_tapDown = true;
//	_hero->stopAndRun();
}

void HelloWorld::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	_tapDown = false;
}
void HelloWorld::onTouchesCancelled(const std::vector<Touch*>&touches, Event *unused_event)
{
	_tapDown = false;
}

 
void HelloWorld::render(Color4F c2, float textureWidth, float textureHeight, int nStripes)
{
	/*
	log("HelloWorld::render()");
	//draw a rect casually

	int _nHillVertices = 0;

	Point _hillVertices[100];
	Color4F _hillTexColor[100];

	float halfLengthOfSide = 512/2;

	_hillVertices[_nHillVertices] = Point(textureWidth / 2 - halfLengthOfSide, textureHeight / 2 - halfLengthOfSide);
	//_hillTexCoords[_nHillVertices++] = Point(pt0.x/512, 1.0f);
	_hillTexColor[_nHillVertices++] = Color4F(0, 1.0f, 0, 1.0f);

	_hillVertices[_nHillVertices] = Point(textureWidth / 2 + halfLengthOfSide, textureHeight / 2 - halfLengthOfSide);
	//_hillTexCoords[_nHillVertices++] = Point(pt0.x/512, 1.0f);
	_hillTexColor[_nHillVertices++] = Color4F(0, 1.0f, 0, 1.0f);

	_hillVertices[_nHillVertices] = Point(textureWidth / 2 - halfLengthOfSide, textureHeight / 2 + halfLengthOfSide);
	//_hillTexCoords[_nHillVertices++] = Point(pt0.x/512, 1.0f);
	_hillTexColor[_nHillVertices++] = Color4F(0, 1.0f, 0, 1.0f);

	_hillVertices[_nHillVertices] = Point(textureWidth / 2 + halfLengthOfSide, textureHeight / 2 + halfLengthOfSide);
	//_hillTexCoords[_nHillVertices++] = Point(pt0.x/512, 1.0f);
	_hillTexColor[_nHillVertices++] = Color4F(0, 1.0f, 0, 1.0f);

	//kmGLPushMatrix();
	//kmGLLoadMatrix(&transform);

	this->setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_COLOR));

	CC_NODE_DRAW_SETUP();


	log("_nHillVertices");
	log(" == %d", _nHillVertices);
	 
	log("\n\n********************************");
	//check if the array has null point 
	for(int i = 0; i < _nHillVertices; i++)
	{
		log(" _hillVertices[_nHillVertices].x  y  i =======  %d", i);
		log("x  == %f  y == %f", _hillVertices[i].x, _hillVertices[i].y);
	}
	log("********************************\n\n");

//	GL::bindTexture2D(_stripes->getTexture()->getName());
	//GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORDS);
	GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	
	//DrawPrimitives::setDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, _hillVertices);
	//glVertexAttribPointer(GL::VERTEX_ATTRIB_FLAG_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, _hillTexCoords);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, 0, _hillTexColor);
   
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)_nHillVertices);
	
	glBlendFunc(GL_DST_COLOR, GL_ZERO);
	*/

	this->setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_COLOR));

	CC_NODE_DRAW_SETUP();
 
	Point* vertices = new Point[nStripes * 6];
	Color4F* colors = new Color4F[nStripes * 6];
	
	int nVertices = 0;
 
	float x1 = -textureWidth;
	float x2;
	float y1 = textureWidth;
	float y2 = 0;
	float dx = textureWidth / nStripes * 2;

	float stripeWidth = dx / 2;
	for(int i = 0; i < nStripes; i++)//changed _nStripes
	{
		x2 = x1 + textureWidth;

		vertices[nVertices] = Point(x1, y1);
		colors[nVertices++] = Color4F(c2.r, c2.g, c2.b, c2.a);

		vertices[nVertices] = Point(x1 + stripeWidth, y1);
		colors[nVertices++] = Color4F(c2.r, c2.g, c2.b, c2.a);

		vertices[nVertices] = Point(x2, y2);
		colors[nVertices++] = Color4F(c2.r, c2.g, c2.b, c2.a);

		vertices[nVertices] = vertices[nVertices - 2];
		colors[nVertices++] = Color4F(c2.r, c2.g, c2.b, c2.a);

		vertices[nVertices] = vertices[nVertices - 2];
		colors[nVertices++] = Color4F(c2.r, c2.g, c2.b, c2.a);

		vertices[nVertices] = Point(x2 + stripeWidth, y2);
		colors[nVertices++] = Color4F(c2.r, c2.g, c2.b, c2.a);

		x1 += dx;
	}
    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);


	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, 0, colors);

	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)nVertices);
 
	float gradientAlpha = 0.7f;
	//float gradientAlpha = 0;

	nVertices = 0;

	vertices[nVertices] = Point(0, 0);
	colors[nVertices++] = Color4F(0, 0, 0, 0);

	vertices[nVertices] = Point	(textureWidth , 0);
	colors[nVertices++] = Color4F(0, 0, 0, 0);

	vertices[nVertices] = Point	(0, textureHeight);
	colors[nVertices++] = Color4F(0, 0, 0, gradientAlpha);

	vertices[nVertices] = Point	(textureWidth, textureHeight);
	colors[nVertices++] = Color4F(0, 0, 0, gradientAlpha);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, 0, colors);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);

	float borderHeight = textureHeight / 16;
	float boardAlpha = 0.3f;
	//float boardAlpha = 0;

	nVertices = 0;

	vertices[nVertices] = Point(0, 0);
	colors[nVertices++] = Color4F(1, 1, 1, boardAlpha);

	vertices[nVertices] = Point(textureWidth, 0);
	colors[nVertices++] = Color4F(1, 1, 1, boardAlpha);

	vertices[nVertices] = Point(0, borderHeight);
	colors[nVertices++] = Color4F(0, 0, 0, 0);

	vertices[nVertices] = Point(textureWidth, borderHeight);
	colors[nVertices++] = Color4F(0, 0, 0, 0);

	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, 0, colors);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)nVertices);
	glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);

	glBlendFunc(GL_DST_COLOR, GL_ZERO);
}

float HelloWorld::distanceOfSegments(float distance, float segmentLength)
{
	return (int)(distance / segmentLength) * segmentLength;
}


void HelloWorld::update(float dt)
{
	float frameRate = 1/dt;
	/*
	if(frameRate < 30)
	{
		log("fps ");

		log("  == %f", 1/dt);
	}*/
	
	float scale = (_screenSize.height*3/4) / _hero->getPosition().y;
	if(scale > 1) scale = 1;
	_terrain->setScale(scale);
	

 
	if(_tapDown)
	{
		if(!_hero->get_awake())
		{
			_hero->wake();
			_tapDown = false;
		}else
		{
			_hero->dive();
		}
	}
	_hero->limitVelocity();
 

	Size textureSize = _backGround->getTextureRect().size;
 
	static double UPDATE_INTERVAL = 1.0f/60.0f;

/*
	static double MAX_CYCLES_PRE_FRAME = 5;
	static double timeAccumulator = 0;

	timeAccumulator += dt;
	if(timeAccumulator > (MAX_CYCLES_PRE_FRAME * UPDATE_INTERVAL))
	{
		timeAccumulator = UPDATE_INTERVAL;
	}

		int velocityIteration = 8;
		int positionInteration = 3;
		while(timeAccumulator >= UPDATE_INTERVAL)
		{
			timeAccumulator -= UPDATE_INTERVAL;
			_world->Step(UPDATE_INTERVAL, velocityIteration, positionInteration);
			_world->ClearForces();
		}
 
	 */
	int velocityIteration = 8;
	int positionInteration = 3;
	_world->Step(UPDATE_INTERVAL, velocityIteration, positionInteration);
 
	_hero->update();

	float PIXELS_PER_SECOND = 100;
	static float offset1 = 0; /// hero pos % (background_texture放大后的宽度 *2)
	static float offset2 = 0;
	static float offset3 = 0;
	float bmsFactor = 1/4.0f;//background Movement Scale Factor
                   
	offset1	=_hero->getPosition().x -  (int)(_hero->getPosition().x / (textureSize.width * _backGround->getScaleX() / bmsFactor)) * textureSize.width * _backGround->getScaleX() / bmsFactor;

	offset2	= _hero->getPosition().x;
	
	//offset3 = _screenSize.width - _hero->getPosition().x + (int)(_hero->getPosition().x / _screenSize.width / 2) * _screenSize.width * 2 + _backGround->getContentSize().width / 2 * _backGround->getScaleX() + _screenSize.width/8;
	
	//offset3 = _screenSize.width - _hero->getPosition().x + distanceOfSegments(_hero->getPosition().x, _screenSize.width + _helloSprite->getContentSize().width * _helloSprite->getScaleX()) + _helloSprite->getContentSize().width / 2 * _helloSprite->getScaleX();


	//origin
	/*
	offset3 = _screenSize.width - _hero->getPosition().x + distanceOfSegments(_hero->getPosition().x, _screenSize.width + _helloSprite->getContentSize().width) + _helloSprite->getContentSize().width / 2;

	_helloSprite->setPosition(Point(offset3, _screenSize.height / 2));
	*/

	/*
	static int helloPosX = 0;
	
	helloPosX += 8;

	if(_helloSprite->getPosition().x - _helloSprite->getContentSize().width/2 > _screenSize.width)
	{
		helloPosX = -1 * _helloSprite->getContentSize().width/2;
	}

	_helloSprite->setPosition(Point(helloPosX, _screenSize.height/2));
	*/

	//_backGround->setPosition(Point(offset3, _screenSize.height / 2));

	 
	if(offset1 >= textureSize.width * _screenSize.width / _backGround->getContentSize().width / bmsFactor)
	{
		offset1 = 0;
	}
 


/*
	if(-1 * this->getPosition().x - _helloSprite->getPosition().x > _helloSprite->getContentSize().width / 2)
	{
		_helloSprite->setPosition(Point(-1 * this->getPosition().x + _screenSize.width + _helloSprite->getContentSize().width / 2, _helloSprite->getPosition().y));
	}

	
	Point newPos;

	newPos = Point(_screenSize.width / 8 - _hero->getPosition().x, this->getPosition().y);

	this->setPosition(newPos);
	*/

	_backGround->setTextureRect(Rect(offset1 / _screenSize.width * _backGround->getContentSize().width * bmsFactor, 0, textureSize.width, textureSize.height ));
 
	_terrain->setOffsetX(offset2);
		//this->setScale(0.25);
}
 
