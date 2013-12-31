
#ifndef _GAME_MAP_H
#define _GAME_MAP_H

#include "cocos2d.h"
#include "GameBullet.h"
#include "GameEnemy.h"
USING_NS_CC;

class GameMap : public cocos2d::CCLayer
{
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
	CCAnimation* explosionEffect(const char* singleName) ;//创建动画帧序列，爆炸
	void autoCreateEnemy(float dt);
	void autoLauchBullet(float dt);//自动发子弹
	void freshScore(float dt);
	GameBullet *m_bullet;
	GameEnemy *m_enemy;
	CCLabelTTF *score;
	CCSprite *explosionSpr;
	int scores;
public:
	CCSprite *m_topBar;
};


#endif // !_GAME_MAP_H





