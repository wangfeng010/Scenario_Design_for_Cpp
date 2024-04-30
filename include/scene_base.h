# pragma once
#include "car.h"
#include "road.h"

/******************************************状态机******************************************/
enum LaneChangeType//换道类型
{
	singleType,//单移线
	doubleType//双移线
};



class SceneBase//场景基类
{
public:
	virtual ~SceneBase() = default;//虚析构
	virtual void showScene();//显示，虚函数，子类如果不重写，就调用这里父类的函数
	virtual void obsMoveStep() {}//虚函数，障碍物单帧移动，描述障碍物的简单移动行为。如果要描述复杂的行为
	virtual bool planning_process() = 0;//整个过程，纯虚函数

	/******************************************直行******************************************/
	void uniformStraight(const double& total_s);//直行，行驶指定距离 //total_s为正，为累计驶过的距离
	void uniformAccBySpeed(const double& target_speed_y);//直行，已知加速度，行驶到指定的速度，匀加（减）速直线运动
	void uniformAccByDis(const double& dis, const double& target_speed_y);//直行，行驶指定的距离时速度达到指定速度，匀加（减）速直线运动
	void uniformAccByTime(const double& target_speed, const double& target_time);//直行，在目标时间内达到目标速度，匀加（减）速直线运动

	/******************************************转向******************************************/
	void carTurn(const int& turn_state, const double& R, const double& total_theta);//转向，已知半径和角度 //total_theta为正，为累计转过的角度
	void laneChange(const Point& target_point, const int& type, const double& s = 0.0);//变道，单移线或双移线

public:
	unique_ptr<RoadBase> road0;//道路父类指针
	unique_ptr<CarBase> car0;//主车父类指针
	double speedlimit = -6.0;//道路限速，可正负
};
