#ifndef _GAME_ENEMY_H
#define _GAME_ENEMY_H

#include "cocos2d.h"
USING_NS_CC;

class GameEnemy : public CCLayer
{
public:
	CCArray *m_Enemys;//敌机数组
	CCSprite *m_EnemySprite;
	GameEnemy();
	~GameEnemy();
	virtual bool init();
	void createNewEnemy();//创建新的敌人
	
	CREATE_FUNC(GameEnemy);
private:
	
	

	void initEnemy();
	void moveEnemy(float delta);
};

#endif