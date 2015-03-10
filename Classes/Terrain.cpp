#include "Terrain.h"

void Terrain::resetBox2DBody()
{
 /*	
	if(_body)
	{
		_world->DestroyBody(_body);
	}

	b2BodyDef bd;
	bd.position.Set(0, 0);
	_body = _world->CreateBody(&bd);

	b2EdgeShape shape;

	b2Vec2 p1, p2;
	for(int i = 0; i < _nBorderVertices - 1; i++)
	{
		p1 = b2Vec2(_borderVertices[i].x/PTM_RATIO, _borderVertices[i].y/PTM_RATIO);
		p2 = b2Vec2(_borderVertices[i+1].x/PTM_RATIO, _borderVertices[i+1].y/PTM_RATIO);
		shape.Set(p1, p2);
	}
	_body->CreateFixture(&shape, 0);

 */
  
	if(_body) return;
	
	Point p0 = _hillKeyPoints[0];
	Point p1 = _hillKeyPoints[kMaxHillKeyPoints - 1];

	b2BodyDef bd;
	bd.position.Set(0, 0);
	_body = _world->CreateBody(&bd);

	float minY = 0;
	if(_screenSize.height > 480)
	{
		minY = (1136 - 1024)/4;
	}
 
	b2EdgeShape shape;

	b2Vec2 ep1 = b2Vec2(p0.x/PTM_RATIO, minY/PTM_RATIO);
	b2Vec2 ep2 = b2Vec2(p1.x/PTM_RATIO, minY/PTM_RATIO);
	shape.Set(ep1, ep2);

	b2FixtureDef bf;
	bf.shape = &shape;
	//bf.isSensor = false;
	bf.density = 0;
	bf.friction = 0;

	_body->CreateFixture(&bf);
}


void Terrain::generateHills()
{
/* old algorithm
	Size winSize = Director::getInstance()->getWinSize();
	float x = 0;
	float y = winSize.height / 2;
	for(int i = 0; i < kMaxHillKeyPoints; i++)
	{
		_hillKeyPoints[i] = Point(x, y);
		x += winSize.width / 2;
		y = rand() % ((int)winSize.height);
	}
*/
	float minDX = 160;
	float minDY = 60;
	int rangeDX = 80;
	int rangeDY = 40;

	float x = -minDX;
	float y = _screenSize.height / 2 ;

	float dy, ny;
	float sign = 1;// +1 going up -1 going down
	float paddingTop = 20;
	float paddingBottom = 20;
 
	for(int i = 0; i < kMaxHillKeyPoints; i++)
	{
		_hillKeyPoints[i] = Point(x, y);
		if(i == 0)
		{
			x = 0;
			y = _screenSize.height / 2 ;
		}else{
			x += rand() % rangeDX + minDX;
			while(true)
			{
				dy = rand() % rangeDY + minDY;
				ny = y + dy * sign;

				if((ny < (_screenSize.height - paddingTop)) && (ny > paddingBottom))
				{
					break;
				}
			}
			y = ny;
		}
		sign *= -1;
	}
}

void Terrain::setupDebugDraw()
{
	_debugDraw = new GLESDebugDraw(PTM_RATIO);
	_world->SetDebugDraw(_debugDraw);
	_debugDraw->SetFlags(GLESDebugDraw::e_shapeBit | GLESDebugDraw::e_jointBit);
}
Terrain* Terrain::createTerrainWithWorld(b2World* world)
{
	Terrain* terrain = new Terrain();

	terrain->initWithWorld(world);

	terrain->autorelease();

	return terrain;
}

bool
 Terrain::initWithWorld(b2World* world)
{
	_world = world;	
	_screenSize = Director::getInstance()->getWinSize();
  

   
	//this->setupDebugDraw();//tocommentout
	this->generateHills();
	this->resetHillVertices();
	this->generateBorderVertices();
	this->createHillBody();

	this->setShaderProgram(ShaderCache::getInstance()->getProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE)); 

	//_batchNode = SpriteBatchNode::create("TinySeal.png");

	//this->addChild(_batchNode);
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("TinySeal.plist");

	return true;
}
 
void Terrain::debugRender(const Mat4& transform, uint32_t flags)
{
//	_world->DrawDebugData();//tocommentout
}

 
void Terrain::draw(Renderer *renderer, const Mat4& transform, uint32_t flags)
{
 
	_customCommand.init(this->getGlobalZOrder());
	_customCommand.func = CC_CALLBACK_0(Terrain::render, this, transform, flags);
	renderer->addCommand(&_customCommand);
 
	/*
	_debugCommand.init(this->getGlobalZOrder());
	_debugCommand.func = CC_CALLBACK_0(Terrain::debugRender, this, transform, transformUpdated);
	renderer->addCommand(&_debugCommand);
	*/
} 
 
void Terrain::render(const Mat4& transform, uint32_t flags)
{
	kmGLPushMatrix();
	kmGLLoadMatrix(&transform);
 
	for(int i = _fromKeyPointI; i < _toKeyPointI; ++i)
	{
		/*
		DrawPrimitives::setDrawColor4F(1.0f, 0, 0, 1.0f);
		DrawPrimitives::drawLine(_hillKeyPoints[i - 1], _hillKeyPoints[i]);

		DrawPrimitives::setDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
		*/
		/*
		Point p0 = _hillKeyPoints[i-1];
	//	log("p0.x");
	//	log("i - 1 == %d", i-1);
	//	log(" == %f", p0.x);
		Point p1 = _hillKeyPoints[i];
	//	log("p1.x");
	//	log(" == %f", p1.x);
		int hSegments = floorf((p1.x - p0.x)/kHillSegmentWidth);
//	log("hSegments");
	//	log(" == %d", hSegments);
		float dx = (p1.x - p0.x) / hSegments;
		float da = M_PI / hSegments;
		float ymid = (p0.y + p1.y) / 2;
		float ampl = (p0.y - p1.y) / 2;

		Point pt0, pt1;
		pt0 = p0;
		for(int j = 0; j < hSegments + 1; ++j)
		{
			pt1.x = p0.x + j * dx;
			pt1.y = ymid + ampl * cosf(da*j);
			
			DrawPrimitives::drawLine(pt0, pt1);
			
			pt0 = pt1;
		}
		*/
	}
	 
 
	CC_NODE_DRAW_SETUP();
	
	GL::bindTexture2D(_stripes->getTexture()->getName());
	GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_TEX_COORD); 
	//GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX); 
	//GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
	 
	//DrawPrimitives::setDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, _hillVertices);
	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORDS, 2, GL_FLOAT, GL_FALSE, 0, _hillTexCoords);
//	glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_TRUE, 0, _hillTexColor);
 
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)_nHillVertices);
  
    kmGLPopMatrix();

}

void Terrain::setOffsetX(float newOffsetX)
{
	_OffsetX = newOffsetX;
	log("screen size w = %f", _screenSize.width);
	//1，屏幕的坐标不变(镜头不动)terrain更新机制   // 看下面的"解释"
	this->setPosition(Point(_screenSize.width/8 - 1 * _OffsetX * this->getScale(), 0));
	this->resetHillVertices();
}

void Terrain::resetHillVertices()
{
	static int preFromKeyPointI = -1;  
	static int preToKeyPointI = -1;
//	log("point .x =  %f", _hillKeyPoints[1].x);
//	_hillKeyPoints[_fromKeyPointI + 1].x;
	//log("_hillKeyPoints[_fromKeyPointI + 1].x < _OffsetX - _screenSize.width / 8 / this->getScale()");
//	log("_hillKeyPoints[1].x");
//	log(" == %f", _hillKeyPoints[1].x);
//	log("_hillKeyPoints[_fromKeyPointI + 1].x ");
//	log(" = %f", _hillKeyPoints[_fromKeyPointI + 1].x );
//	log(" == %d", _hillKeyPoints[_fromKeyPointI + 1].x < _OffsetX - _screenSize.width / 8 / this->getScale());
 
	//解释：这个是单向动态背景顶点更新机制，其实一OffsetX(hero的x坐标)作为参考点(原点)来计算的。
	
	//整个机制的核心是：参考点向前屏幕就向后移动，参考点就能够相对于镜头不变，因此terrain的x坐标应该是-hero.x。terrain->setposition(-hero.x, 某y)
	//1，屏幕的坐标不变(镜头不动)terrain更新机制：首先terrain是addchild到屏幕(layer)上的，因此若想屏幕镜头不动，游戏地图向前滚动的画就只能更新terrain的坐标了。首先要确定的是terrain移动的参考点(看2)，
	
	//2, 然后确定起始位置。
	//2.1 首先让三者x坐标重叠(三者就是屏幕，terrain，hero)。让参考点的起始位置(0，屏幕高度一半)，并且terrain的起始位置也是(0, 某y)，也就是一开始先让terrain.x与hero.x重叠(相等)，方便后面的位移推算。
	//即，hero->setposition(0, 屏幕一半) , terrain->setposition(0, 某y)
	
	//2.2 然后对terrain进行位移。位移参考点到理想位置，固定参考点在屏幕上的位置不变。若这里移动hero.x相对于terrain的坐标 到屏幕左方八分之一处，而terrain相对于屏幕的坐标不同，这样的唯一参考点是没有意义的。
	//如：hero->setposition(screen/8, 高度一半)，terrain->setposition(-hero.x, 某y)，hero向前移动了，但terrain也向后移动了，没意义。最后画面显示的结果是hero还是在屏幕的最左边而不是左方八分之一。
	//因此方法应该是改变terrain的坐标，而不是改变hero的坐标，这样来制造出固定在屏幕左方八分之一的效果。也即是：hero要在屏幕上向x方向移动八分之一，其实就是只要让terrain相对屏幕向x方向移动八分之一，因为terrain是相对屏幕的，而hero是相对terrain的，因此hero也会移动。也就是一开始terrain.pos = poaition(0 + screenwidth/8 , 某y)。
	
	//2.3 根据上面的机制核心公式，合成初始位置的坐标，得出最终位置更新公式。
	//即：hero->setposition(屏幕一半) ， terrain->setposition(-hero.x + screenwidth/8 , 某y);
	
		
	//3，根据hero计算屏幕边缘坐标。
	//计算屏幕两边缘相对于hero坐标的位置：hero是添加到terrain里面的，所以hero的坐标是相对terrain来说的，无论terrain怎么移动(terrain->setposition())，都不会改变hero相对terrain的坐标(其实也就是hero.x)。因此keypoint和hero.x都用回正常的，因为keypoint和hero.x都是属于屏幕的，但屏幕就不是了，屏幕现在的坐标应该是相对于hero来说的，而不是一屏幕左下角作为参考点了，屏幕的边缘是更新的阈值，边缘的坐标必须计算出来。具体计算就看下面的while。
	

 
	while(_hillKeyPoints[_fromKeyPointI + 1].x < _OffsetX - _screenSize.width / 8 / this->getScale())
	{
		_fromKeyPointI++;
	}
	while(_hillKeyPoints[_toKeyPointI].x < _OffsetX + _screenSize.width / 8 * 12 / this->getScale())
	{
		_toKeyPointI++;
	}
	
	
	 // texture the triangle hill
     /*
	float minY = 0;
	if(_screenSize.height > 480)
	{
		minY = (1136 - 1024) / 4;
	}

	_nHillVertices = 0;

	Point p0, p1;

	for(int i = _fromKeyPointI ; i < _toKeyPointI + 1; i++)
	{
		p0 = _hillKeyPoints[i];

		_hillVertices[_nHillVertices] = Point(p0.x, 0 + minY);
		_hillTexCoords[_nHillVertices++] = Point(p0.x/512, 1.0f);

		_hillVertices[_nHillVertices] = Point(p0.x, p0.y);
		_hillTexCoords[_nHillVertices++] = Point(p0.x/512, 0);

	}
 */
 
 
	float minY = 0;
	/*if(_screenSize.height > 480)
	{
		minY = (1136 - 1024) / 4;
	}*/
 
	if(preFromKeyPointI != _fromKeyPointI || preToKeyPointI != _toKeyPointI)// if fromKeyPoint or toKeyPoint updated
	{
		_nHillVertices = 0;
		//_nBorderVertices = 0;//tocommentout
		Point p0, p1, pt0, pt1;
		p0 = _hillKeyPoints[_fromKeyPointI];

		int hSegments = floorf((p1.x - p0.x) / kHillSegmentWidth);

		///mark 这里的p1.x中的p1没有初始化就用来计算，又可能会导致hsegment变成一个非常非常大的数，而导致后面的循环次数极大
		float dx = (p1.x - p0.x) / hSegments;
		float da = M_PI / hSegments;
		float ymid = (p0.y + p1.y) / 2;
		float ampl = (p0.y - p1.y) / 2;
		pt0 = p0;
 
		for(int i = _fromKeyPointI + 1; i < _toKeyPointI + 1; i++)
		{
			p1 = _hillKeyPoints[i];

			int hSegments = floorf((p1.x - p0.x) / kHillSegmentWidth);
			float dx = (p1.x - p0.x) / hSegments;
			float da = M_PI / hSegments;
			float ymid = (p0.y + p1.y) / 2;
			float ampl = (p0.y - p1.y) / 2;
			pt0 = p0;
			//_borderVertices[_nBorderVertices++] = pt0;//tocommentout

			for(int j = 1; j < hSegments + 1; j++)
			{
				pt1.x = p0.x + j * dx;
				pt1.y = ymid + ampl * cosf(da * j);
				//_borderVertices[_nBorderVertices++] = pt1;//tocommentout

				_hillVertices[_nHillVertices] = Point(pt0.x, 0 + minY);
				_hillTexCoords[_nHillVertices++] = Point(decimalPreservedDiv(pt0.x, 512), 1.0f);
				//_hillTexColor[_nHillVertices++] = Color4F(1.0f, 1.0f, 0, 1.0f);
				
				_hillVertices[_nHillVertices] = Point(pt1.x, 0 + minY);
				_hillTexCoords[_nHillVertices++] = Point(decimalPreservedDiv(pt1.x, 512), 1.0f);
				//_hillTexColor[_nHillVertices++] = Color4F(1.0f, 1.0f, 0, 1.0f);

				_hillVertices[_nHillVertices] = Point(pt0.x, pt0.y);
				_hillTexCoords[_nHillVertices++] = Point(decimalPreservedDiv(pt0.x, 512), 0);
				//_hillTexColor[_nHillVertices++] = Color4F(1.0f, 1.0f, 0, 1.0f);

				_hillVertices[_nHillVertices] = Point(pt1.x, pt1.y);
				_hillTexCoords[_nHillVertices++] = Point(decimalPreservedDiv(pt1.x, 512), 0);
				//_hillTexColor[_nHillVertices++] = Color4F(1.0f, 1.0f, 0, 1.0f);

				pt0 = pt1;
			}
			p0 = p1;
		}
	} 

	preFromKeyPointI = _fromKeyPointI;
	preToKeyPointI = _toKeyPointI;
 
	//this->resetBox2DBody();
}
 
void Terrain::generateBorderVertices()
{
	_nBorderVertices = 0;
 
	Point p0, p1, pt0, pt1;
	p0 = _hillKeyPoints[0];
 
	pt0 = p0;
 
	for(int i = 1; i < 200; i++)
	{
		p1 = _hillKeyPoints[i];

		int hSegments = floorf((p1.x - p0.x) / kHillSegmentWidth);
		float dx = (p1.x - p0.x) / hSegments;
		float da = M_PI / hSegments;
		float ymid = (p0.y + p1.y) / 2;
		float ampl = (p0.y - p1.y) / 2;
		pt0 = p0;
		_borderVertices[_nBorderVertices++] = pt0;

		for(int j = 1; j < hSegments + 1; j++)
		{
			pt1.x = p0.x + j * dx;
			pt1.y = ymid + ampl * cosf(da * j);
			_borderVertices[_nBorderVertices++] = pt1;
			pt0 = pt1;
		}
		p0 = p1;
	}

}



void Terrain::createHillBody()
{
 
	b2BodyDef bd;
	bd.position.Set(0, 0);
	_body = _world->CreateBody(&bd);

	b2EdgeShape shape;
 
	b2Vec2 p1, p2;
	for(int i = 0; i < _nBorderVertices - 1; i++)
	{
		p1 = b2Vec2(_borderVertices[i].x/PTM_RATIO, _borderVertices[i].y/PTM_RATIO);
		p2 = b2Vec2(_borderVertices[i+1].x/PTM_RATIO, _borderVertices[i+1].y/PTM_RATIO);

		shape.Set(p1, p2);
			_body->CreateFixture(&shape, 0);

	}
 

	//test
	/*
	b2BodyDef bdt;
	bdt.position.Set(0, 0);
	_bodyTest = _world->CreateBody(&bdt);

	b2EdgeShape shapeT;

	b2Vec2 pt1 = b2Vec2(0, 0);

	b2Vec2 pt2 = b2Vec2(_borderVertices[_nBorderVertices - 2].x/PTM_RATIO, 0);
	
	shapeT.Set(pt1, pt2);
	
	_bodyTest->CreateFixture(&shapeT, 0);
	*/
 
}
 
inline float Terrain::decimalPreservedDiv(float dividend, float divisor)
{
	return (dividend - (int)(dividend / divisor) * divisor) / divisor;
}



Terrain::Terrain(void):
_fromKeyPointI(1)
,_toKeyPointI(0)
{
	_stripes = NULL;
	_body = NULL;
}


Terrain::~Terrain(void)
{
	log("terrain destruct");
	_stripes->release();
	_stripes = NULL;
}


