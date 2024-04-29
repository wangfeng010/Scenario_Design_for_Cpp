# pragma once
#include "scene_base.h"

class StraightStopObs : public SceneBase //停障
{
public:
	StraightStopObs();
	void showScene();//显示
	bool planning_process() override;

public:
	unique_ptr<Cone> cone;//锥桶
	double safedis = 50.0;//停止距离
};

class StraightStation : public SceneBase //停靠站点
{
public:
	StraightStation();
	void showScene();//显示
	bool planning_process() override;

public:
	unique_ptr<Point> station;//站点
	int stop_time = 3;//停站时间，s
};

class StraightFollow : public SceneBase //跟车
{
public:
	StraightFollow();
	void showScene();//显示
	bool planning_process() override;

public:
	unique_ptr<CarNormal> carObs;//障碍车
	double safedis = 120.0;//保持车距
};

class StraightCrosswalk : public SceneBase //通过斑马线
{
public:
	StraightCrosswalk();
	bool peopleInCross();//斑马线是否有人
	void showScene();//显示
	bool planning_process() override;

public:
	int people_num = 5;//人数
	vector<unique_ptr<Person>> people;//行人
	double speedlimit_cross = -3.0;//斑马线限速
};