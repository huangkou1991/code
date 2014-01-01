#ifndef _GAME_BULLET_H
#define _GAME_BULLET_H

#include "cocos2d.h"

USING_NS_CC;

class GameBullet : public CCLayer
{
	//�����е��ӵ�������Ļ�ϵ�
	CC_SYNTHESIZE(CCArray *, m_Bullets, BulletArray);
	//�����ӵ�sprite BatchNode
	CC_SYNTHESIZE(CCSpriteBatchNode *, m_BulltesSprite, BulletSprite);
public:
	GameBullet();
	~GameBullet();
	CREATE_FUNC(GameBullet);
	virtual bool init();
	void createNewBullet();//����һ���µ��ӵ�

private:

	void initBullet();//��ʼ���ӵ�
	void moveBullet(float delay);//�ƶ��ӵ�

};


#endif