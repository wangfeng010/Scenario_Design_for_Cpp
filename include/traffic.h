# pragma once
#include "planning_base.h"
#include "car.h"

/******************************************障碍物******************************************/
class Cone//锥桶
{
public:
	Cone() = default;
	Cone(const double& pos_x, const double& pos_y, const double& R = 20.0);
	void showCone();//绘制

public:
	unique_ptr<Point> p_center;//中心点
	double r = 20.0;//半径
};

class Person//行人
{
public:
	Person() = default;
	Person(const double& pos_x, const double& pos_y);
	void personMove();//行人移动
	void showPerson();//绘制

public:
	unique_ptr<Point> p_center;//中心点
	double r = 20.0;//半径
	double speed = 0.0;//速度
};