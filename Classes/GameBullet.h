#ifndef _GAME_BULLET_H
#define _GAME_BULLET_H

#include "cocos2d.h"

USING_NS_CC;

class GameBullet : public CCLayer
{
public:
	GameBullet();
	~GameBullet();
	CREATE_FUNC(GameBullet);
	virtual bool init();
	void createNewBullet();//����һ���µ��ӵ�

private:
	CCArray *m_Bullets;//�����е��ӵ�������Ļ�ϵ�
	CCSpriteBatchNode *m_BulltesSprite;//�����ӵ�sprite

	void initBullet();//��ʼ���ӵ�
	void moveBullet(float delay);//�ƶ��ӵ�

};


#endif