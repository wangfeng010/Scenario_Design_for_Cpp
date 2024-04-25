#include "road.h"

RoadNormal::RoadNormal(const double& r_width)//一般道路
{
	Rwidth = r_width;
	left_boundary = SWIDTH / 2.0 - Rwidth;
	right_boundary = SWIDTH / 2.0 + Rwidth;
}

void RoadNormal::showRoad()//绘制道路
{
	setlinestyle(PS_SOLID, 4);
	setlinecolor(BLACK);
	line(left_boundary, 0.0, left_boundary, SHEIGHT);
	line(right_boundary, 0.0, right_boundary, SHEIGHT);
}