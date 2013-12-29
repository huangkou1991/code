#include "GameMap.h"
#include "GamePlayer.h"

USING_NS_CC;

#define BLUTTE_SPEED 0.15f

GameMap::GameMap(void)
{
}

GameMap::~GameMap(void)
{
}

static GameMap *map;

GameMap *GameMap::sharedGameMap()
{
	if (!map)
	{
		map = GameMap::create();
	}
	return map;
}

CCScene *GameMap::createGameScence()
{
	//������ͼ��������delegate�м���
	CCScene *scence = CCScene::create();
	GameMap *map = GameMap::sharedGameMap();
	scence->addChild(map);
	return scence;
}



//��ʼ����ͼ�ϵĸ��ֶ��� 
bool GameMap::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	//С��ͼ����
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			CCSprite *temp = CCSprite::create("back_scence.png");
			temp->setAnchorPoint(ccp(0, 0));
			this->addChild(temp);
			temp->setPosition(ccp(temp->getContentSize().width * i, temp->getContentSize().height * j));
		}
	}

	//���ܷ�����ʾ
	m_topBar = CCSprite::create("spr_bottom.png");
	m_topBar->setScaleX(0.75);
	this->addChild(m_topBar);
	m_topBar->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 
						CCDirector::sharedDirector()->getWinSize().height - m_topBar->getContentSize().height / 2));

	//��Ϸ��ҷɻ�
	GamePlayer *player = GamePlayer::shareGamePlayer();
	this->addChild(player);
	player->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, player->getContentSize().height / 2));

	//����ӵ�
	m_bullet = GameBullet::create();
	m_bullet->setPosition(player->getPosition());
	this->addChild(m_bullet);

	//��ӵл�
	m_enemy = GameEnemy::create();
	m_enemy->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
	this->addChild(m_enemy);

	this->schedule(schedule_selector(GameMap::autoLauchBullet), BLUTTE_SPEED);
	this->schedule(schedule_selector(GameMap::autoCreateEnemy), BLUTTE_SPEED * 10);
	this->setTouchEnabled(true);
}
//�Զ������ӵ�
void GameMap::autoLauchBullet(float dt)
{
	m_bullet->createNewBullet();
}
void GameMap::autoCreateEnemy(float dt)
{
	m_enemy->createNewEnemy();
}


bool GameMap::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLOG("Touch Map!");
	return true;
}
void GameMap::onEnter()
{
	CCLayer::onEnter();
	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameMap::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}