#include "GameMap.h"
#include "GamePlayer.h"
#include "GameBullet.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

#define BLUTTE_SPEED 0.15f

GameMap::GameMap(void):scoreLabel(NULL), scores(0),explosionSpr(NULL), 
					m_bullet(NULL), m_enemy(NULL), m_topBar(NULL), resideOpps(3)
{
}

GameMap::~GameMap(void)
{
}

//����map
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

//�����ļ���ǰ׺�õ�һϵ�е�����֡����
//@singleName: �ļ���ǰ׺
//@framesNum: ����֡֡��
//@return  ��������
CCAnimation* GameMap::explosionAnimation(const char* preName, int framesNum) 
{
	CCAnimation * animation = CCAnimation::create();  
	for (int i = 1; i < framesNum; i++)  
	{
		const char *name = CCString::createWithFormat("%s%d.png", preName, i)->getCString();
		animation->addSpriteFrameWithFileName(name);  
	}
	return animation;
}

void GameMap::update(float dt)
{

	CCArray *bl = m_bullet->getBulletArray();
	for (unsigned int i = 0; i < bl->count(); i++)
	{
		CCSprite *bullet = static_cast<CCSprite *>(bl->objectAtIndex(i));
		//�����ӵ�����������
		CCRect bulletRect = CCRectMake(bullet->getPositionX() - bullet->getContentSize().width * bullet->getAnchorPoint().x, 
			bullet->getPositionY() - bullet->getContentSize().height * bullet->getAnchorPoint().y, 
			bullet->getContentSize().width - 10, bullet->getContentSize().height - 20);

		//��������ϵ//���������
		bulletRect.origin.y -= CCDirector::sharedDirector()->getWinSize().height / 2 - 10;
		bulletRect.origin.x + 5;
		//�˴����ÿ���л����ӵ�����ײ
		CCArray *ene = m_enemy->getEnemyArray();
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
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("snd_explosion2.wav");
				CCAnimation * animation = this->explosionAnimation("spr_explosion1_", 7);  
				//ÿ֡�ļ��
				animation->setDelayPerUnit(1.2f/20.0f);  
				//animation->setRestoreOriginalFrame(false);  
				//����ɿ�ִ�е�action
				CCAnimate *eff=CCAnimate::create(animation);  

				//���õ�һ����֡
				CCSprite *explosionSpr = CCSprite::create("spr_explosion1_1.png");
				explosionSpr->setPosition(ccpAdd(existScreenEnemy->getPosition(), ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2)));
				this->addChild(explosionSpr);
				//��������
				CCSequence *seq = CCSequence::create(eff, CCCallFuncO::create(this, callfuncO_selector(GameMap::setExplosionSpr), explosionSpr),NULL);

				explosionSpr->runAction(seq);

				//�л����ӵ���ײ���Ƴ���Ӧ�Ķ���
				ene->removeObject(existScreenEnemy);
				bl->removeObject(bullet);
				//�Ƴ���Ӧ��child
				m_enemy->getEnemySprite()->removeChild(existScreenEnemy, true);
				m_bullet->getBulletSprite()->removeChild(bullet, true);
				//������10
				scores += 10;
			}

		}
	}
}

void GameMap::setExplosionSpr(CCObject *obj)
{
	//CCLOG("set explosion status!");
	CCSprite *explosionSpr = (CCSprite *)obj;
	explosionSpr->setVisible(false);
	this->removeChild(explosionSpr, true);
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
	scoreLabel = CCLabelTTF::create(CCString::createWithFormat("%d", scores)->getCString(), "Arial", 17);
	scoreLabel->setColor(ccYELLOW);
	scoreLabel->setPosition(ccpAdd(m_topBar->getPosition(), ccp(-55, 20)));
	this->addChild(scoreLabel);



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
	
	//ʣ�����
	resideSpr = CCSprite::create("bar_blue.png");
	resideSpr->setAnchorPoint(ccp(0, 0.5));
	resideSpr->setScaleX(1.70);
	resideSpr->setScaleY(0.55);
	resideSpr->setPosition(ccpSub(m_topBar->getPosition(), ccp(m_topBar->getContentSize().width / 4 + 71.5, 12)));
	this->addChild(resideSpr);

	
	//���붨ʱ������ʱˢ��
	this->schedule(schedule_selector(GameMap::autoLauchBullet), BLUTTE_SPEED);
	this->schedule(schedule_selector(GameMap::autoCreateEnemy), BLUTTE_SPEED * 10);
	this->schedule(schedule_selector(GameMap::checkPlayerHitEnemy), BLUTTE_SPEED);
	this->schedule(schedule_selector(GameMap::freshScore), BLUTTE_SPEED);
	this->scheduleUpdate();

	this->setTouchEnabled(true);
	return true;
}
//ˢ�·���
void GameMap::freshScore(float dt)
{
	scoreLabel->setString(CCString::createWithFormat("%d", scores)->getCString());
}

//�Զ������ӵ�
void GameMap::autoLauchBullet(float dt)
{
	m_bullet->createNewBullet();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("snd_explosion3.wav");
}
//��������ײ
void GameMap::checkPlayerHitEnemy(float dt)
{

	for (int i = 0; i < m_enemy->getEnemyArray()->count(); i++)
	{
		CCSprite *enemy = static_cast<CCSprite *>(m_enemy->getEnemyArray()->objectAtIndex(i));

		GamePlayer *player = GamePlayer::shareGamePlayer();
		CCRect eneRect = CCRectMake(enemy->getPositionX() - enemy->getContentSize().width * enemy->getAnchorPoint().x, 
			enemy->getPositionY() - enemy->getContentSize().height * enemy->getAnchorPoint().y,
			enemy->getContentSize().width, enemy->getContentSize().height);
		CCRect playerRect = CCRectMake(player->getPositionX() - player->getContentSize().width * player->getAnchorPoint().x, 
			player->getPositionY() - player->getContentSize().height * player->getAnchorPoint().y,
			player->getContentSize().width, player->getContentSize().height);

		playerRect.origin.y -= CCDirector::sharedDirector()->getWinSize().height / 2 + 15;
		playerRect.origin.x -= CCDirector::sharedDirector()->getWinSize().width / 2;

		if (eneRect.intersectsRect(playerRect))
		{
			CCLOG("######################HIT#########################");
			m_enemy->getEnemyArray()->removeObject(enemy);
			m_enemy->getEnemySprite()->removeChild(enemy, true);

			CCAnimation * animation = this->explosionAnimation("spr_explosion2_", 7);  
			//ÿ֡�ļ��
			animation->setDelayPerUnit(1.2f/20.0f);  
			//animation->setRestoreOriginalFrame(false);  
			//����ɿ�ִ�е�action
			CCAnimate *eff=CCAnimate::create(animation);  

			//���õ�һ����֡
			CCSprite *explosionSpr = CCSprite::create("spr_explosion1_1.png");
			explosionSpr->setPosition(player->getPosition());
			this->addChild(explosionSpr);
			//��������
			CCSequence *seq = CCSequence::create(eff, CCCallFuncO::create(this, callfuncO_selector(GameMap::setExplosionSpr), explosionSpr),NULL);

			explosionSpr->runAction(seq);


			player->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, player->getContentSize().height / 2));
			player->setScale(0.3);
			player->setScale(1.0);
			CCScaleTo *scaleTo = CCScaleTo::create(1.0, 1.0);
			player->runAction(scaleTo);
			resideOpps--;//ÿ����ײ��ʣ���������

			//ˢ��ʣ�����
			freshResideOpps();
		}
	}

}
void GameMap::freshResideOpps()
{
	if (resideOpps == 2)
	{
		CCSprite *t = CCSprite::create("bar_yellow.png");
		resideSpr->setDisplayFrame(t->displayFrame());
		resideSpr->setScaleX(1.0);
	}
	else if (resideOpps == 1)
	{
		CCSprite *t = CCSprite::create("bar_red.png");
		resideSpr->setDisplayFrame(t->displayFrame());
		resideSpr->setScaleX(0.6);
	}
}

//��ʱ��������
void GameMap::autoCreateEnemy(float dt)
{
	m_enemy->createNewEnemy();
}


bool GameMap::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	//CCLOG("Touch Map!");
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