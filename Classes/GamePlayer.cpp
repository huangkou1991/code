
#include "GamePlayer.h"

GamePlayer::GamePlayer():PlayerIsSelect(false)
{
	CCLOG("GamePlayer Constructor!");
	
}

GamePlayer::~GamePlayer()
{
	CCLOG("GamePlayer Destructor!");
	
}

//����
void GamePlayer::onEnter()
{
	CCLOG("GamePlayer OnEnter");
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
}
//�˳�
void GamePlayer::onExit()
{
	CCLOG("GamePlayer OnExit");
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

static GamePlayer *player;
//������һ��player
GamePlayer *GamePlayer::shareGamePlayer()
{
	if (!player)
	{
		player = GamePlayer::create();
		CCSprite *t = CCSprite::create("spr_plane.png");
		player->setDisplayFrame(t->displayFrame());


		return player;
	}
	return player;
}
//����player�ƶ�
void GamePlayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
	CCLOG("GamePlayer Is Moving!");
	CCPoint lastPoint = touch->getLocationInView();//��ȡ��Ӧ��ͼ�ϵľ���λ��
	lastPoint = CCDirector::sharedDirector()->convertToGL(lastPoint);//ת����gl����
	//X�᷶Χ����
	(lastPoint.x < this->getContentSize().width / 2) && (lastPoint.x = this->getContentSize().width / 2);
	(lastPoint.x > CCDirector::sharedDirector()->getWinSize().width - this->getContentSize().width / 2)
	&& (lastPoint.x = CCDirector::sharedDirector()->getWinSize().width - this->getContentSize().width / 2);
	//Y�᷶Χ����
	(lastPoint.y < this->getContentSize().height / 2) && (lastPoint.y = this->getContentSize().height / 2);
	(lastPoint.y > CCDirector::sharedDirector()->getWinSize().height - this->getContentSize().height / 2 
		- GameMap::sharedGameMap()->m_topBar->getContentSize().height)
	&& (lastPoint.y = CCDirector::sharedDirector()->getWinSize().height - this->getContentSize().height / 2
	- GameMap::sharedGameMap()->m_topBar->getContentSize().height);


	if (this->PlayerIsSelect)//���ѡ����player
		this->setPosition(lastPoint);//����playerλ��
}
//��ʼ���
bool GamePlayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	CCLOG("GamePlayer Is Press!");
	
	CCPoint temp = touch->getLocation();
	CCPoint p = convertToNodeSpace(temp);
	//���GamePlayer�����ھ���
	CCRect rec = CCRectMake(	getPositionX() - getContentSize().width * getAnchorPoint().x,
							getPositionY() - getContentSize().height * getAnchorPoint().y,
							getContentSize().width, getContentSize().height); 
	rec.origin = CCPointZero;
	if (rec.containsPoint(p))
	{
		this->PlayerIsSelect = true;
	}
	
	return true;
}
void GamePlayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCLOG("Reset Select Status");
	//����ѡ��״̬
	this->PlayerIsSelect = false;
	
}