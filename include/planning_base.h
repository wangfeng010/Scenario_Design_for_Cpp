# pragma once
#include <iostream>
#include <graphics.h>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
using namespace std;

/*********************************全局变量***********************************/
constexpr auto SWIDTH = 1200.0;//窗口宽
constexpr auto SHEIGHT = 1200.0;//窗口高
constexpr auto PI = 3.14159265358979323846;//圆周率
constexpr auto SHOWCIRCLE = false;//是否绘制轨迹
constexpr auto DELAYTIME = 20;//间隔时间，ms
constexpr auto CHANGETIME = 1000;//换档时间，ms

/******************************************点******************************************/
class Point//点
{
public:
	Point() = default;
	Point(const double& p_x, const double& p_y, const double& p_theta = 0.0, const double& p_R = 0.0);
	void showPoint();//绘制点
	void pointMove(const double& speed_x, const double& speed_y);//点的移动
	void pointTurn(const Point& center, const double& turn_speed);//点绕center旋转，turn_speed为角速度
	double distanceTo(const Point& p) const;//距离
	double thetaTo(const Point& p) const;//this->相对于p角度

public:
	double x;
	double y;
	double thetaP = 0.0;//角度
	double Rp = 0.0;//旋转半径
	int r = 5;//绘制半径
};

/******************************************向量******************************************/
class Vec2d
{
public:
	Vec2d() = default;
	Vec2d(const double& new_x, const double& new_y, const bool& flag);//通过两个值构造向量，flag用于区分另一个构造函数
	Vec2d(const Point& p_start, const Point& p_end);//通过两个点构造向量
	Vec2d(const double& length, const double& angle);//通过长度和方向构造向量
	double length();//模
	double crossProd(const Vec2d& other) const;//叉乘
	double innerProd(const Vec2d& other) const;//点乘

public:
	double x;
	double y;
};

/******************************************全局函数******************************************/
void delay(const int& time); //延时函数，单位ms
double normalizeAngle(const double& theta); //角度修正到[-PI, PI)之间
void correctAngleError(double& target_theta, const double& delta_theta);//角度误差修正
double disPointToLine(const Point& p, const Point& p_start, const Point& p_end);//点到直线的距离