
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

	virtual void onEnter();
	virtual void onExit();

	CREATE_FUNC(GameMap);
private:
	void autoCreateEnemy(float dt);
	void autoLauchBullet(float dt);//自动发子弹
	GameBullet *m_bullet;
	GameEnemy *m_enemy;
public:
	CCSprite *m_topBar;
};


#endif // !_GAME_MAP_H





