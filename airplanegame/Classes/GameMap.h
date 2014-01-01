
#ifndef _GAME_MAP_H
#define _GAME_MAP_H

#include "cocos2d.h"
#include "GameBullet.h"
#include "GameEnemy.h"
USING_NS_CC;

class GameMap : public cocos2d::CCLayer
{
	//������
	CC_SYNTHESIZE(CCSprite *, m_topBar, TopBar);
public:
	GameMap(void);
	~GameMap(void);

	static GameMap *sharedGameMap();
	virtual bool init();
	static cocos2d::CCScene *createGameScence();
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void update(float dt);
	virtual void onEnter();
	virtual void onExit();
	void setExplosionSpr(CCObject *obj);
	CREATE_FUNC(GameMap);
private:
	void freshResideOpps();//ˢ��ʣ����������
	void checkPlayerHitEnemy(float dt);
	CCAnimation* explosionAnimation(const char* singleName, int frameNum) ;//��������֡���У���ը
	void autoCreateEnemy(float dt);//�����л�
	void autoLauchBullet(float dt);//�Զ����ӵ�
	void freshScore(float dt);//ˢ�·���
	
	
	GameBullet *m_bullet;//��ͼ�ϴ��ڵ��ӵ�
	GameEnemy *m_enemy;//��ͼ�ϴ��ڵĵл�
	CCLabelTTF *scoreLabel;//����label
	CCSprite *resideSpr;//ʣ�����
	CCSprite *explosionSpr;//��ը��һ֡
	int scores;//����
	int resideOpps;//ʣ�����
};


#endif // !_GAME_MAP_H





