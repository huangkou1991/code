#include "GameMap.h"
#include "GamePlayer.h"
#include "GameBullet.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

#define BLUTTE_SPEED 0.15f

GameMap::GameMap(void):score(NULL), scores(0),explosionSpr(NULL)
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

CCAnimation* GameMap::explosionEffect( const char* singleName ) 
{
	
	CCSpriteFrameCache* spriteFrameCache = CCSpriteFrameCache::sharedSpriteFrameCache();

	int index = 1;
	CCSpriteFrame* frame = NULL;

	CCArray* frameArray = CCArray::create();
	do 
	{
		frame = spriteFrameCache->spriteFrameByName(
			CCString::createWithFormat("%s%d.png", singleName, index)->getCString());

		if(frame == NULL) {
			break;
		}

		frameArray->addObject(frame);
		index++;

	} while (true);

	return CCAnimation::createWithSpriteFrames(frameArray);
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
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("snd_explosion2.wav");
				CCAnimation * animation=CCAnimation::create();  
				for (int i = 1; i < 7; i++)  
				{  
					char name[40];  
					sprintf(name,"spr_explosion1_%d.png",i);  
					animation->addSpriteFrameWithFileName(name);  
				}  
				animation->setDelayPerUnit(1.2f/20.0f);  
				//animation->setRestoreOriginalFrame(false);  

				CCAnimate *eff=CCAnimate::create(animation);  


				CCSprite *explosionSpr = CCSprite::createWithSpriteFrameName("spr_explosion1_1.png");
				explosionSpr->setPosition(ccpAdd(bullet->getPosition(), ccp(CCDirector::sharedDirector()->getWinSize().width / 2 - 5, 30)));
				this->addChild(explosionSpr);
				CCSequence *seq = CCSequence::create(eff, CCCallFuncO::create(this, callfuncO_selector(GameMap::setExplosionSpr), explosionSpr),NULL);

				explosionSpr->runAction(seq);


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

void GameMap::setExplosionSpr(CCObject *obj)
{
	CCLOG("set explosion status!");
	CCSprite *explosionSpr = (CCSprite *)obj;
	explosionSpr->setVisible(false);
	//this->removeChild(explosionSpr, true);
}
//初始化地图上的各种东西 
bool GameMap::init()
{

	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("game_res.plist");
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
	m_bullet = GameBullet::create();
	m_bullet->setPosition(player->getPosition());
	this->addChild(m_bullet);

	//添加敌机
	m_enemy = GameEnemy::create();
	m_enemy->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
	this->addChild(m_enemy);
	
	//加入定时器，定时刷新
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
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("snd_explosion3.wav");
}
//定时创建敌人
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
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("Battle.mp3", true);
	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

void GameMap::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}