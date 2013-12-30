#include "GameBullet.h"
#include "GamePlayer.h"

GameBullet::GameBullet() : m_Bullets(NULL), m_BulltesSprite(NULL)
{

}

GameBullet::~GameBullet()
{
	//�ͷŶ�Ӧ���ӵ�����
	CC_SAFE_RELEASE(m_Bullets);
}

static GameBullet *bullet;

GameBullet *GameBullet::sharedBullet()
{
	if (!bullet)
	{
		bullet = GameBullet::create();
	}
	return bullet;
}

bool GameBullet::init()
{
	if (!CCLayer::init())
		return false;
	initBullet();
	return true;
}

void GameBullet::initBullet()
{
	//�����ӵ�����
	m_Bullets = CCArray::create();
	m_Bullets->retain();

	//�����ӵ���ͼ
	m_BulltesSprite = CCSpriteBatchNode::create("spr_bullet.png");
	m_BulltesSprite->setPosition(CCPointZero);
	this->addChild(m_BulltesSprite);
	
	//��ʱ�ƶ��ӵ�
	schedule(schedule_selector(GameBullet::moveBullet));
}

//�����µ��ӵ�
void GameBullet::createNewBullet()
{
	CCLOG("Create New Bullet");
	CCSprite *newBullet = CCSprite::createWithTexture(m_BulltesSprite->getTexture());
	newBullet->setPosition(ccpSub(GamePlayer::shareGamePlayer()->getPosition(), ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 0)));
	m_BulltesSprite->addChild(newBullet);

	m_Bullets->addObject(newBullet);
}

void GameBullet::moveBullet(float delay)
{
	for (unsigned int i = 0; i < m_Bullets->count(); i++)
	{
		CCSprite *bullet = static_cast<CCSprite *>(m_Bullets->objectAtIndex(i));

		//�ƶ��µ�λ��
		bullet->setPositionY(bullet->getPositionY() + 200.0 * delay);
		
		if (bullet->getPositionY() >= CCDirector::sharedDirector()->getWinSize().height - 
			GameMap::sharedGameMap()->m_topBar->getContentSize().height * 2 + 30)
		{
			CCLOG("Bullets Out Of Bounds");
			m_Bullets->removeObjectAtIndex(i);
			m_BulltesSprite->removeChild(bullet, true);
		}
	}
}