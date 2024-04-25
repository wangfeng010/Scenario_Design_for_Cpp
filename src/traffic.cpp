#include "traffic.h"

/******************************************障碍物******************************************/
Cone::Cone(const double& pos_x, const double& pos_y, const double& R) : r(R)//锥桶
{
	p_center = make_unique<Point>(pos_x, pos_y);
}

void Cone::showCone()//绘制
{
	setfillcolor(RGB(255, 127, 0));//橙色
	solidcircle(p_center->x, p_center->y, r);
}