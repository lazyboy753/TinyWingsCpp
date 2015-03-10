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
	//1����Ļ�����겻��(��ͷ����)terrain���»���   // �������"����"
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
 
	//���ͣ�����ǵ���̬����������»��ƣ���ʵһOffsetX(hero��x����)��Ϊ�ο���(ԭ��)������ġ�
	
	//�������Ƶĺ����ǣ��ο�����ǰ��Ļ������ƶ����ο�����ܹ�����ھ�ͷ���䣬���terrain��x����Ӧ����-hero.x��terrain->setposition(-hero.x, ĳy)
	//1����Ļ�����겻��(��ͷ����)terrain���»��ƣ�����terrain��addchild����Ļ(layer)�ϵģ����������Ļ��ͷ��������Ϸ��ͼ��ǰ�����Ļ���ֻ�ܸ���terrain�������ˡ�����Ҫȷ������terrain�ƶ��Ĳο���(��2)��
	
	//2, Ȼ��ȷ����ʼλ�á�
	//2.1 ����������x�����ص�(���߾�����Ļ��terrain��hero)���òο������ʼλ��(0����Ļ�߶�һ��)������terrain����ʼλ��Ҳ��(0, ĳy)��Ҳ����һ��ʼ����terrain.x��hero.x�ص�(���)����������λ�����㡣
	//����hero->setposition(0, ��Ļһ��) , terrain->setposition(0, ĳy)
	
	//2.2 Ȼ���terrain����λ�ơ�λ�Ʋο��㵽����λ�ã��̶��ο�������Ļ�ϵ�λ�ò��䡣�������ƶ�hero.x�����terrain������ ����Ļ�󷽰˷�֮һ������terrain�������Ļ�����겻ͬ��������Ψһ�ο�����û������ġ�
	//�磺hero->setposition(screen/8, �߶�һ��)��terrain->setposition(-hero.x, ĳy)��hero��ǰ�ƶ��ˣ���terrainҲ����ƶ��ˣ�û���塣�������ʾ�Ľ����hero��������Ļ������߶������󷽰˷�֮һ��
	//��˷���Ӧ���Ǹı�terrain�����꣬�����Ǹı�hero�����꣬������������̶�����Ļ�󷽰˷�֮һ��Ч����Ҳ���ǣ�heroҪ����Ļ����x�����ƶ��˷�֮һ����ʵ����ֻҪ��terrain�����Ļ��x�����ƶ��˷�֮һ����Ϊterrain�������Ļ�ģ���hero�����terrain�ģ����heroҲ���ƶ���Ҳ����һ��ʼterrain.pos = poaition(0 + screenwidth/8 , ĳy)��
	
	//2.3 ��������Ļ��ƺ��Ĺ�ʽ���ϳɳ�ʼλ�õ����꣬�ó�����λ�ø��¹�ʽ��
	//����hero->setposition(��Ļһ��) �� terrain->setposition(-hero.x + screenwidth/8 , ĳy);
	
		
	//3������hero������Ļ��Ե���ꡣ
	//������Ļ����Ե�����hero�����λ�ã�hero����ӵ�terrain����ģ�����hero�����������terrain��˵�ģ�����terrain��ô�ƶ�(terrain->setposition())��������ı�hero���terrain������(��ʵҲ����hero.x)�����keypoint��hero.x���û������ģ���Ϊkeypoint��hero.x����������Ļ�ģ�����Ļ�Ͳ����ˣ���Ļ���ڵ�����Ӧ���������hero��˵�ģ�������һ��Ļ���½���Ϊ�ο����ˣ���Ļ�ı�Ե�Ǹ��µ���ֵ����Ե������������������������Ϳ������while��
	

 
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

		///mark �����p1.x�е�p1û�г�ʼ�����������㣬�ֿ��ܻᵼ��hsegment���һ���ǳ��ǳ�������������º����ѭ����������
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


