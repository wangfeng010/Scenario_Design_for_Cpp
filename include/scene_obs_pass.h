#pragma once
#include "scene_base.h"

class StaticObs : public SceneBase//静态绕障
{
public:
	StaticObs();
	void showScene();//显示
	bool planning_process() override;//整个过程

public:
	unique_ptr<Cone> cone;//锥桶
	double start_dis = 200.0;//开始绕障的距离
};

class OvertakeObs : public SceneBase//超车
{
public:
	OvertakeObs();
	void obsMoveStep();
	void showScene();//显示
	bool planning_process() override;//整个过程

public:
	unique_ptr<CarNormal> carObs;//前障碍车
	double start_dis = 0.0;//开始绕障的距离
};

class MeetingObs : public SceneBase//会车
{
public:
	MeetingObs();
	void obsMoveStep();
	void showScene();//显示
	bool planning_process() override;//整个过程

public:
	unique_ptr<CarNormal> carObs;//障碍车
	double start_dis = 200.0;//开始绕障的距离
};