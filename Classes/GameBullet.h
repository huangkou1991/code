#ifndef _GAME_BULLET_H
#define _GAME_BULLET_H

#include "cocos2d.h"

USING_NS_CC;

class GameBullet : public CCLayer
{
public:
	CCArray *m_Bullets;//放所有的子弹，在屏幕上的
	CCSpriteBatchNode *m_BulltesSprite;//构建子弹sprite
	GameBullet();
	~GameBullet();
	CREATE_FUNC(GameBullet);
	virtual bool init();
	void createNewBullet();//创建一个新的子弹

private:

	void initBullet();//初始化子弹
	void moveBullet(float delay);//移动子弹

};


#endif