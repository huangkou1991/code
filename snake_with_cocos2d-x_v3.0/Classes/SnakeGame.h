#ifndef __SnakeGame_SCENE_H__
#define __SnakeGame_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

typedef enum{LEFT, RIGHT, UP, DOWN} DIR;

class SnakeGame : public cocos2d::Layer
{
public:

	SnakeGame();
	~SnakeGame();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
	// implement the "static create()" method manually
	CREATE_FUNC(SnakeGame);

private:
	//�ߵ�����
	std::vector<Sprite *> snake;
	//ÿ�γ��ֵ�ʳ��
	Sprite *food;
	//��ǰ�ߵķ���
	DIR snakeDir;
	//ÿ���ƶ�ά��һ�����ڵ��λ��
	Point snakeTailPos;
private:
	//ͨ�õ��ƶ�����
	void commonMove(DIR dir, float speed);
	//���ƶ�
	void snakeMove(float delayTime);
	//�߳�ʳ��
	void snakeEat();
	//��ȡnode���ڵľ�������
	Rect getRectZone(Node *node);
	//�䶯ʳ��λ��
	void foodRandMove();
	//������ɵ����λ���Ƿ�Ϸ�
	bool checkLocation(Point p);
	//����Ƿ���磬���߳Ե��Լ�
	bool checkBoundsOrKillSelf();
	//��ʾ������Ϣ
	void SnakeGame::showGameOver();
};

#endif // __SnakeGame_SCENE_H__
