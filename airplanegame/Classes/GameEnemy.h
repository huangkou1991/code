#ifndef _GAME_ENEMY_H
#define _GAME_ENEMY_H

#include "cocos2d.h"
USING_NS_CC;

class GameEnemy : public CCLayer
{
	//�л�����
	CC_SYNTHESIZE(CCArray *, m_Enemys, EnemyArray);
	//�л�sprite
	CC_SYNTHESIZE(CCSprite *, m_EnemySprite, EnemySprite);
public:
	GameEnemy();
	~GameEnemy();
	virtual bool init();
	void createNewEnemy();//�����µĵ���
	
	CREATE_FUNC(GameEnemy);
private:
	
	

	void initEnemy();
	void moveEnemy(float delta);
};

#endif