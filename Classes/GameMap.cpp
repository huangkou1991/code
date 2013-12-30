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
	//创建地图场景，在delegate中加载
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
		//计算子弹所处的区域
		CCRect bulletRect = CCRectMake(bullet->getPositionX() - bullet->getContentSize().width * bullet->getAnchorPoint().x, 
			bullet->getPositionY() - bullet->getContentSize().height * bullet->getAnchorPoint().y, 
			bullet->getContentSize().width, bullet->getContentSize().height);

		//修正坐标系//问题待修正
		bulletRect.origin.y -= CCDirector::sharedDirector()->getWinSize().height / 2;

		//此处检测每个敌机和子弹的碰撞
		CCArray *ene = m_enemy->m_Enemys;
		for (unsigned int j = 0; j < ene->count(); j++)
		{
			CCSprite *existScreenEnemy = static_cast<CCSprite *>(ene->objectAtIndex(j));
			//计算敌机所处的矩形区域
			CCRect eneRect = CCRectMake(existScreenEnemy->getPositionX() - existScreenEnemy->getContentSize().width * existScreenEnemy->getAnchorPoint().x, 
				existScreenEnemy->getPositionY() - existScreenEnemy->getContentSize().height * existScreenEnemy->getAnchorPoint().y, 
				existScreenEnemy->getContentSize().width, existScreenEnemy->getContentSize().height);
			
			//如果二个矩形相交，碰撞
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


//初始化地图上的各种东西 
bool GameMap::init()
{
	if (!CCLayer::init())
	{
		return false;
	}
	//小地图铺满
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

	//底总分数显示
	m_topBar = CCSprite::create("spr_bottom.png");
	m_topBar->setScaleX(0.75);
	this->addChild(m_topBar);
	m_topBar->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 
						CCDirector::sharedDirector()->getWinSize().height - m_topBar->getContentSize().height / 2));
	score = CCLabelTTF::create(CCString::createWithFormat("%d", scores)->getCString(), "Arial", 17);
	score->setColor(ccYELLOW);
	score->setPosition(ccpAdd(m_topBar->getPosition(), ccp(-55, 20)));
	this->addChild(score);



	//游戏玩家飞机
	GamePlayer *player = GamePlayer::shareGamePlayer();
	this->addChild(player);
	player->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, player->getContentSize().height / 2));

	//添加子弹
	m_bullet = GameBullet::sharedBullet();
	m_bullet->setPosition(player->getPosition());
	this->addChild(m_bullet);

	//添加敌机
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
//刷新分数
void GameMap::freshScore(float dt)
{
	score->setString(CCString::createWithFormat("%d", scores)->getCString());
}

//自动发射子弹
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