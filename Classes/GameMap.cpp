#include "GameMap.h"
#include "GamePlayer.h"
#include "GameBullet.h"

USING_NS_CC;

#define BLUTTE_SPEED 0.15f

GameMap::GameMap(void):score(NULL), scores(0)
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

void GameMap::update(float dt)
{

	CCArray *bl = m_bullet->m_Bullets;
	for (unsigned int i = 0; i < bl->count(); i++)
	{
		CCSprite *bullet = static_cast<CCSprite *>(bl->objectAtIndex(i));
		//�����ӵ�����������
		CCRect bulletRect = CCRectMake(bullet->getPositionX() - bullet->getContentSize().width * bullet->getAnchorPoint().x, 
			bullet->getPositionY() - bullet->getContentSize().height * bullet->getAnchorPoint().y, 
			bullet->getContentSize().width, bullet->getContentSize().height);

		//��������ϵ//���������
		bulletRect.origin.y -= CCDirector::sharedDirector()->getWinSize().height / 2;

		//�˴����ÿ���л����ӵ�����ײ
		CCArray *ene = m_enemy->m_Enemys;
		for (unsigned int j = 0; j < ene->count(); j++)
		{
			CCSprite *existScreenEnemy = static_cast<CCSprite *>(ene->objectAtIndex(j));
			//����л������ľ�������
			CCRect eneRect = CCRectMake(existScreenEnemy->getPositionX() - existScreenEnemy->getContentSize().width * existScreenEnemy->getAnchorPoint().x, 
				existScreenEnemy->getPositionY() - existScreenEnemy->getContentSize().height * existScreenEnemy->getAnchorPoint().y, 
				existScreenEnemy->getContentSize().width, existScreenEnemy->getContentSize().height);
			
			//������������ཻ����ײ
			if (eneRect.intersectsRect(bulletRect))
			{
				CCLOG("====================Hit Enemy==================");
				ene->removeObject(existScreenEnemy);
				bl->removeObject(bullet);
				m_enemy->m_EnemySprite->removeChild(existScreenEnemy, true);
				m_bullet->m_BulltesSprite->removeChild(bullet, true);
				scores += 10;
			}

		}
	}
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
	score = CCLabelTTF::create(CCString::createWithFormat("%d", scores)->getCString(), "Arial", 17);
	score->setColor(ccYELLOW);
	score->setPosition(ccpAdd(m_topBar->getPosition(), ccp(-55, 20)));
	this->addChild(score);



	//��Ϸ��ҷɻ�
	GamePlayer *player = GamePlayer::shareGamePlayer();
	this->addChild(player);
	player->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, player->getContentSize().height / 2));

	//����ӵ�
	m_bullet = GameBullet::sharedBullet();
	m_bullet->setPosition(player->getPosition());
	this->addChild(m_bullet);

	//��ӵл�
	m_enemy = GameEnemy::sharedGameEnemy();
	m_enemy->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
	this->addChild(m_enemy);

	this->schedule(schedule_selector(GameMap::autoLauchBullet), BLUTTE_SPEED);
	this->schedule(schedule_selector(GameMap::autoCreateEnemy), BLUTTE_SPEED * 10);
	this->schedule(schedule_selector(GameMap::freshScore), BLUTTE_SPEED);
	this->scheduleUpdate();

	this->setTouchEnabled(true);
	return true;
}
//ˢ�·���
void GameMap::freshScore(float dt)
{
	score->setString(CCString::createWithFormat("%d", scores)->getCString());
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
	//m_bullet->createNewBullet();
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