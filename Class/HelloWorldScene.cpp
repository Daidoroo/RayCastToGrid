#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

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

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	/*
    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
	*/
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/
    
    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
    
    
}

void HelloWorld::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		//Vec2 prelocation = touch->getPreviousLocationInView();
		Size visibleSize = Director::getInstance()->getVisibleSize();

		m_vTouchBegin = touch->getLocation();
		m_vTouchEnd = m_vTouchBegin;
	}

}

void HelloWorld::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{
	for (auto &item : touches)
	{
		auto touch = item;
		//Vec2 prelocation = touch->getPreviousLocationInView();
		Size visibleSize = Director::getInstance()->getVisibleSize();

		m_vTouchEnd = touch->getLocation();
	}

	makePath();
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event)
{

	for (auto &item : touches)
	{
		auto touch = item;
		//Vec2 prelocation = touch->getPreviousLocationInView();
		Size visibleSize = Director::getInstance()->getVisibleSize();

		m_vTouchEnd = touch->getLocation();
	}

	makePath();
}

void HelloWorld::makePath()
{
	int fSize = 50.0f;
	int nMaxCol = 20;
	int nMaxRow = 20;

	Vec2 vOrigin = m_vTouchBegin;
	Vec2 vOriginGrid = vOrigin;
	Vec2 m_vDir2D = m_vTouchEnd - m_vTouchBegin;
	m_vDir2D.normalize();

	float fCol = vOrigin.x / fSize;
	float fRow = vOrigin.y / fSize;

	int nCol = (fCol >= 0.0f) ? fCol : fCol - 1.0f;
	int nRow = (fRow >= 0.0f) ? fRow : fRow - 1.0f;

	int m_nCurIdxPos[2];	// 0: col 1: row
	m_nCurIdxPos[eAxisIndex::X] = nCol;
	m_nCurIdxPos[eAxisIndex::Z] = nRow;

	Vec2 vLeftBottom((int)(nCol * fSize), (int)(nRow * fSize));
	Vec2 vRightBottom(vLeftBottom.x + fSize, vLeftBottom.y);
	Vec2 vLeftTop(vLeftBottom.x, vLeftBottom.y + fSize);
	Vec2 vRightTop(vLeftBottom.x + fSize, vLeftBottom.y + fSize);

	Vec2* vSquarevCorner[2][2] = { 0, };
	vSquarevCorner[0][0] = &vLeftBottom;
	vSquarevCorner[0][1] = &vRightBottom;
	vSquarevCorner[1][0] = &vLeftTop;
	vSquarevCorner[1][1] = &vRightTop;

	Vec2 vDirReverse2D(-m_vDir2D.x, -m_vDir2D.y);
	// 기준이될 점을 하나 구함.
	int nX = (vDirReverse2D.x > 0) ? 1 : 0;
	int nZ = (vDirReverse2D.y > 0) ? 1 : 0;

	Vec2& refCornerByReverse = *vSquarevCorner[nZ][nX];
	Vec2  vDelta = vOriginGrid - refCornerByReverse;
	if (vDelta.x < 0.0f)
		vDelta.x = -vDelta.x;

	if (vDelta.y < 0.0f)
		vDelta.y = -vDelta.y;

	float fLengthToAxisX = FLT_MAX;
	float fLengthToAxisZ = FLT_MAX;

	if (vDirReverse2D.y != 0.0f)
		fLengthToAxisX = fabs(vDelta.y / m_vDir2D.y);

	if (vDirReverse2D.x != 0.0f)
		fLengthToAxisZ = fabs(vDelta.x / m_vDir2D.x);

	float fLengthToNearAxis = (fLengthToAxisX <= fLengthToAxisZ) ? fLengthToAxisX : fLengthToAxisZ;

	m_vBackPos = vOrigin + (vDirReverse2D * fLengthToNearAxis);
	Vec2 vMovePerIdx = (vDirReverse2D * fLengthToNearAxis);
	vMovePerIdx.x /= fSize;
	vMovePerIdx.y /= fSize;

	Vec2 m_vCurIdxPos;
	m_vCurIdxPos.x = fCol + vMovePerIdx.x;
	m_vCurIdxPos.y = fRow + vMovePerIdx.y;

	// 여기까지왔으면 시작 지점을 구했음
	// 이제부터 이동비율이 큰축쪽을 먼저 이동하고 작은축쪽을 1씩 이동. 
	float fMoveZRatePerX = FLT_MAX;
	float fMoveXRatePerZ = FLT_MAX;

	float m_fRateMoveLength;
	unsigned char m_btRateMoveAxis;

	if (m_vDir2D.x != 0.0f && m_vDir2D.y != 0.0f)
	{
		fMoveZRatePerX = fabs(m_vDir2D.y / m_vDir2D.x);
		fMoveXRatePerZ = fabs(m_vDir2D.x / m_vDir2D.y);
	}

	if (fabs(m_vDir2D.x) >= fabs(m_vDir2D.y))
	{
		m_fRateMoveLength = fMoveXRatePerZ;
		m_btRateMoveAxis = eAxisIndex::X;
	}
	else
	{
		m_fRateMoveLength =  fMoveZRatePerX;
		m_btRateMoveAxis = eAxisIndex::Z;
	}

	unsigned char m_btUnitMoveAxis = !m_btRateMoveAxis;

	float* pvCurIdxPos = (float*)&m_vCurIdxPos;
	// UnitAxis의 위치에 따라 m_fRateMoveLength를 결정할거. (처음에만. ) 
	int m_nRateMoveSign = 1;
	int m_nUnitMoveSign = 1;
	float* pvDir2D = (float*)&m_vDir2D;
	int m_nRateMoveCnt = 0;

	if (pvDir2D[m_btRateMoveAxis] < 0)
	{
		m_nRateMoveSign = -1;
	}

	if (pvDir2D[m_btUnitMoveAxis] < 0)
	{
		m_nUnitMoveSign = -1;
	}


	// 이동축에 딱떨어져서.. 무한대 이동일경우.
	if (m_fRateMoveLength == FLT_MAX)
	{
		// 이동값을 그리드의 끝까지 초과하도록 설정.
		if (m_fRateMoveLength >= 0)
			m_fRateMoveLength = nMaxCol - 1 - pvCurIdxPos[m_btRateMoveAxis];
		else
			m_fRateMoveLength = pvCurIdxPos[m_btRateMoveAxis];

		m_nRateMoveCnt = m_fRateMoveLength;
	}
	else
	{
		// 기준점에서 얼마나 떨어졌냐를 비교해야됨.
		float* pvCorner = (float*)&refCornerByReverse;
		float fCorner = pvCorner[m_btUnitMoveAxis] / fSize;

		float fCorrect1 = fabs( fCorner - pvCurIdxPos[m_btUnitMoveAxis]);
		fCorrect1 = (fCorrect1 * m_fRateMoveLength);
		// 처음에만 unitmove축의 값이 0이 아닐수 있어서 비율만큼이동하면 안됨.
		// 그다음부터는 moveToNext()에서 비율대로 이동.
		float fForFirstMove = m_fRateMoveLength - fCorrect1;
		if (m_nRateMoveSign < 0)
			fForFirstMove = -fForFirstMove;

		pvCurIdxPos[m_btUnitMoveAxis] += m_nUnitMoveSign;
		pvCurIdxPos[m_btRateMoveAxis] += fForFirstMove;

		int nRateNext = pvCurIdxPos[m_btRateMoveAxis];

		m_nRateMoveCnt = abs(nRateNext - m_nCurIdxPos[m_btRateMoveAxis]);
	}

	if (m_nRateMoveSign < 0)
		m_fRateMoveLength = -m_fRateMoveLength;

	m_Pathf.clear();
	m_Pathf.push_back(m_vCurIdxPos);

	m_Path.clear();
	m_Path.push_back(Vec2(nCol, nRow));

	while (nCol >= 0 && nCol < nMaxCol && nRow >= 0 && nRow < nMaxRow)
	{
		if (m_nRateMoveCnt > 0)
		{
			m_nCurIdxPos[m_btRateMoveAxis] += m_nRateMoveSign;	// 1 or -1
			m_nRateMoveCnt--;

			nCol = m_nCurIdxPos[eAxisIndex::X];
			nRow = m_nCurIdxPos[eAxisIndex::Z];

			m_Path.push_back(Vec2(nCol, nRow));
			continue;
		}

		// UnitMoveAxis기준으로 다음 칸 교점까지 이동시킴.
		// 시작 이동 기준을 갱신하고.
		float* pvCurIdxPos = (float*)&m_vCurIdxPos;
		int nRateOld = pvCurIdxPos[m_btRateMoveAxis];
		// 시작점을 옴김.
		pvCurIdxPos[m_btUnitMoveAxis] += m_nUnitMoveSign;
		pvCurIdxPos[m_btRateMoveAxis] += m_fRateMoveLength;

		int nRateNext = pvCurIdxPos[m_btRateMoveAxis];

		// 이동 카운트 구한다.
		m_nRateMoveCnt = abs(nRateNext - nRateOld);

		// 현재 인덱스를 바꾸고.
		m_nCurIdxPos[m_btUnitMoveAxis] += m_nUnitMoveSign;

		nCol = m_nCurIdxPos[eAxisIndex::X];
		nRow = m_nCurIdxPos[eAxisIndex::Z];

		m_Pathf.push_back(m_vCurIdxPos);
		m_Path.push_back(Vec2(nCol, nRow));
	}
}

void HelloWorld::draw(Renderer* renderer, const cocos2d::Mat4 &transform, uint32_t flags)
{
	cmd.init(_globalZOrder);
	cmd.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&cmd);
}

void HelloWorld::onDraw(const cocos2d::Mat4 &transform, uint32_t flags)
{
	int nSize = 50;
	int nHaff = nSize / 2;
	int nCount = 20;
	int nW = nSize * nCount;
	int nH = nSize * nCount;

	DrawPrimitives::setDrawColor4B(255, 0, 0, 255);
	glLineWidth(1.0f);
	for (int i = 0; i <= nCount; i++)
	{
		int y = i * nSize;
		DrawPrimitives::drawLine(Point(0, y), Point(getContentSize().width, y));
	}

	for (int i = 0; i <= nCount; i++)
	{
		int x = i * nSize;
		DrawPrimitives::drawLine(Point(x, 0), Point(x, getContentSize().height));
	}

	
	DrawPrimitives::setDrawColor4B(0, 255, 0, 255);
	DrawPrimitives::drawLine(Point(m_vTouchBegin.x, m_vTouchBegin.y), Point(m_vTouchEnd.x, m_vTouchEnd.y));
	
	DrawPrimitives::setDrawColor4B(0, 0, 255, 255);
	if (m_Path.empty() == false)
	{
		for (auto it = m_Path.begin(); it != m_Path.end(); it++)
		{
			Vec2 vIndex = *it;

			Vec2 vOrigin = Vec2(vIndex.x * nSize, vIndex.y * nSize);
			Vec2 vCenter = Vec2(vOrigin.x + nHaff, vOrigin.y + nHaff);
			Vec2 v0 = vCenter - Vec2(5, 5);
			Vec2 v1 = vCenter + Vec2(5, 5);
			DrawPrimitives::drawRect(v0, v1);
		}

		DrawPrimitives::setDrawColor4B(255, 255, 255, 255);
		Vec2 v0 = m_vBackPos - Vec2(2, 2);
		Vec2 v1 = m_vBackPos + Vec2(2, 2);
		DrawPrimitives::drawRect(v0, v1);
	}

	DrawPrimitives::setDrawColor4B(255, 255, 0, 255);
	if (m_Pathf.empty() == false)
	{
		for (auto it = m_Pathf.begin(); it != m_Pathf.end(); it++)
		{
			Vec2 vIndex = *it;

			Vec2 vOrigin = Vec2(vIndex.x * nSize, vIndex.y * nSize);
			Vec2 v0 = vOrigin - Vec2(2, 2);
			Vec2 v1 = vOrigin + Vec2(2, 2);
			DrawPrimitives::drawRect(v0, v1);
		}
	}
}


