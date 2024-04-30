#include "planning_base.h"

/******************************************点******************************************/
Point::Point(const double& p_x, const double& p_y, const double& p_theta, const double& p_R) : x(p_x), y(p_y), thetaP(p_theta), Rp(p_R)//点
{

}

void Point::showPoint()//绘制点
{
	setfillcolor(BLACK);
	solidcircle(x, y, r);
}

void Point::pointMove(const double& speed_x, const double& speed_y)//点的移动
{
	x += speed_x;
	y += speed_y;
}

void Point::pointTurn(const Point& center, const double& turn_speed)//点的旋转（p绕center旋转）
{
	thetaP += turn_speed;//turn_speed>0，为绕着center逆时针行驶，<0则为绕着center顺时针行驶
	x = Rp * cos(thetaP) + center.x;
	y = -Rp * sin(thetaP) + center.y;//考虑到eaxyX坐标系，加负号
}

double Point::distanceTo(const Point& p) const//距离
{
	return hypot(x - p.x, y - p.y);
}

double Point::thetaTo(const Point& p) const//this->相对于p的角度
{
	if (x >= p.x && y == p.y)//x轴正轴上，包括与p重合
	{
		return 0.0;
	}
	else if (x < p.x && y == p.y)//x负轴上
	{
		return PI;
	}
	else if (x == p.x && y > p.y)//y负轴上
	{
		return -PI / 2.0;
	}
	else if (x == p.x && y < p.y)//y正轴上
	{
		return PI / 2.0;
	}
	else if (x > p.x)//第一、四象限
	{
		return -atan((y - p.y) / (x - p.x));//考虑到eaxyX坐标系，取负号
	}
	else if (x < p.x)//第二、三象限
	{
		return PI - atan((y - p.y) / (x - p.x));
	}

	return 0.0;
}

/******************************************向量******************************************/
Vec2d::Vec2d(const double& new_x, const double& new_y, const bool& flag) : x(new_x), y(new_y)//通过两个值构造向量，flag用于区分另一个构造函数
{

}

Vec2d::Vec2d(const Point& p_start, const Point& p_end)//通过两个点构造向量
{
	x = p_end.x - p_start.x;
	y = -(p_end.y - p_start.y);//向量的值切换为y轴向上的直角坐标
}

Vec2d::Vec2d(const double& length, const double& angle)//通过长度和方向构造向量
{
	x = length * cos(angle);
	y = length * sin(angle);//向量的值切换为y轴向上的直角坐标
}

double Vec2d::length()//模
{
	return hypot(x, y);
}

double Vec2d::crossProd(const Vec2d& other) const //叉乘
{
	return x * other.y - y * other.x;
}

double Vec2d::innerProd(const Vec2d& other) const //点乘
{
	return x * other.x + y * other.y;
}

/******************************************全局函数******************************************/
void delay(const int& time) //延时函数，单位ms
{
	clock_t  now = clock();
	while (clock() - now < time)
	{

	}
}

double normalizeAngle(const double& theta)//角度修正到[-PI, PI)之间
{
	double theta_new = fmod(theta + PI, 2.0 * PI);//把角度对2PI求模
	if (theta_new < 0.0)
	{
		theta_new += (2.0 * PI);//如果小于0，则加2PI
	}
	return theta_new - PI;//然后减去PI
}

void correctAngleError(double& target_theta, const double& delta_theta)//角度误差修正
{
	if (delta_theta > 0.0)
	{
		if (target_theta > 0)//逆时针
		{
			target_theta -= delta_theta;
		}
		else if (target_theta < 0)//顺时针
		{
			target_theta += delta_theta;
		}
	}
}

double disPointToLine(const Point& p, const Point& p_start, const Point& p_end)//点到直线的距离
{
	Vec2d line(p_start, p_end);
	Vec2d line_p(p_start, p);
	if (line.length() == 0.0)
	{
		return line_p.length();
	}
	return fabs(line.crossProd(line_p)) / line.length();
}