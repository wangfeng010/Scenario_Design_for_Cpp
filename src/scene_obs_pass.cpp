#include "scene_obs_pass.h"

/******************************************静态绕障******************************************/
StaticObs::StaticObs()//静态绕障
{
	road0 = make_unique<RoadNormal>(250.0);
	cone = make_unique<Cone>(SHEIGHT / 2.0, SWIDTH / 2.0, 50.0);

	car0 = make_unique<CarNormal>(SWIDTH / 2.0, SHEIGHT - 70.0);
	car0->speed = -4.0;

	car0->coutInfo();
	showScene();
	system("pause");
}

void StaticObs::showScene()//显示
{
	BeginBatchDraw();
	cleardevice();

	road0->showRoad();
	cone->showCone();
	car0->showCar(BLACK);

	if (SHOWCIRCLE && car0->p_center)//绘制轨迹线
	{
		car0->showCircle();
	}

	EndBatchDraw();
	delay(DELAYTIME);
}

bool StaticObs::planning_process()
{
	double start_line = cone->p_center->y + cone->r + start_dis + car0->car_length;//开始绕障的位置
	uniformStraight(car0->pmidr->y - start_line);

	//计算目标点
	double dis_l_L = cone->p_center->x - cone->r - road0->left_boundary;//左间距
	double dis_r_L = road0->right_boundary - cone->p_center->x - cone->r;//右间距
	double target_l_x = road0->left_boundary + dis_l_L / 2.0;//左目标点x
	double target_r_x = road0->right_boundary - dis_r_L / 2.0;//右目标点x
	double target_y = cone->p_center->y;//左间距或右间距中点的y
	Point target_l_point(target_l_x, target_y);//左目标点
	Point target_r_point(target_r_x, target_y);//右目标点
	cout << "dis_l_L = " << dis_l_L << ", dis_r_L = " << dis_r_L << ", target_l_x= " << target_l_x << ", target_r_x= " << target_r_x << ", target_y= " << target_y << endl;

	if (dis_l_L > car0->car_width * 1.2)//如果左边够宽，优先从左绕
	{
		laneChange(target_l_point, LaneChangeType::doubleType);
	}
	else//左边不够宽
	{
		if (dis_r_L > car0->car_width * 1.2)//如果右边够宽，从右绕
		{
			laneChange(target_r_point, LaneChangeType::doubleType);
		}
		else//右边也不够宽
		{
			cout << "左右宽度不够通行，减速停障" << endl;
			double stopline = cone->p_center->y + cone->r + 50.0;
			uniformAccByDis(car0->pmidf->y - stopline, 0.0);
			return false;
		}
	}

	car0->updatePmidr();
	uniformStraight(car0->pmidr->y - 0.0);
	return true;
}

/******************************************超车******************************************/
OvertakeObs::OvertakeObs()//超车
{
	double Rwidth = 100.0;
	road0 = make_unique<RoadDoubleLane>(Rwidth);
	carObs = make_unique<CarNormal>(SWIDTH / 2.0 + Rwidth / 2.0, SHEIGHT - 400.0, 0.0, 50.0, 100.0);
	car0 = make_unique<CarNormal>(SWIDTH / 2.0 + Rwidth / 2.0, SHEIGHT - 70.0, 0.0, 40.0, 80.0);

	carObs->speed = -2.0;
	car0->speed = -6.0;

	carObs->updateStraightInfo();//让carObs准备直行

	car0->coutInfo();
	carObs->coutInfo();

	showScene();
	system("pause");
}

void OvertakeObs::obsMoveStep()
{
	carObs->moveStraightStep();
}

void OvertakeObs::showScene()//显示
{
	BeginBatchDraw();
	cleardevice();

	road0->showRoad();
	carObs->showCar(RED);
	car0->showCar(BLACK);

	if (SHOWCIRCLE && car0->p_center)//绘制轨迹线
	{
		car0->showCircle();
	}

	EndBatchDraw();
	delay(DELAYTIME);
}

bool OvertakeObs::planning_process()
{
	double delta_speed = fabs(car0->speed) - fabs(carObs->speed);//速度差，在本场景中保持不变
	cout << "delta_speed = " << delta_speed << endl;
	if (delta_speed <= 0.0)//主车更慢，无法实现超车
	{
		cout << "主车更慢，无法实现超车" << endl;
		return false;
	}

	double dis_l_L = carObs->plr->x - road0->left_boundary;//左间距
	double target_l_x = road0->left_boundary + dis_l_L / 2.0;//左目标点x
	cout << "dis_l_L = " << dis_l_L << ", target_l_x = " << target_l_x << endl;
	if (dis_l_L <= car0->car_width * 1.2)
	{
		cout << "左侧宽度不够超车" << endl;
		return false;
	}

	start_dis = car0->car_length * 3.5;
	double dis0 = car0->pmidr->y - carObs->pmidr->y;//初始距离
	cout << "start_dis = " << start_dis << ", dis0 = " << dis0 << endl;
	if (dis0 < start_dis)
	{
		cout << "距离前车太近，变道距离不够" << endl;
		return false;
	}

	uniformStraight(dis0 - start_dis);//直行到变道起点
	double time_lane_change = dis0 / delta_speed;//行驶到并排位置花的时间（循环次数）,变道时间
	double dis_target = carObs->car_length;//目标距离，拉开1个车身长度
	double target_y = car0->pmidr->y + car0->speed * time_lane_change;//行驶到并排的位置，左目标点y
	double time_straight = dis_target / delta_speed;//直行时间
	double s = fabs(car0->speed) * time_straight;//直行的距离
	cout << "dis_target = " << dis_target << ", target_y = " << target_y << ", s = " << s << endl;

	Point target_l_point(target_l_x, target_y);//左目标点
	laneChange(target_l_point, LaneChangeType::doubleType, s);//行驶到并排位置后，再向前直行拉开1个车身，再向右变道

	car0->updatePmidr();
	uniformStraight(car0->pmidr->y - 0.0);
	return true;
}

/******************************************会车******************************************/
MeetingObs::MeetingObs()//会车
{
	road0 = make_unique<RoadNormal>(100.0);
	carObs = make_unique<CarNormal>(SWIDTH / 2.0, 50.0, PI, 50.0, 100.0);
	car0 = make_unique<CarNormal>(SWIDTH / 2.0, SHEIGHT - 70.0, 0.0, 40.0, 80.0);
	carObs->speed = -3.0;
	car0->speed = -4.0;

	carObs->updateStraightInfo();//让carObs准备直行

	car0->coutInfo();
	carObs->coutInfo();
	showScene();
	system("pause");
}

void MeetingObs::obsMoveStep()
{
	carObs->moveStraightStep();
}

void MeetingObs::showScene()//显示
{
	BeginBatchDraw();
	cleardevice();

	road0->showRoad();
	carObs->showCar(RED);
	car0->showCar(BLACK);

	if (SHOWCIRCLE && car0->p_center)//绘制轨迹线
	{
		car0->showCircle();
	}

	EndBatchDraw();
	delay(DELAYTIME);
}

bool MeetingObs::planning_process()
{
	double total_speed = fabs(car0->speed) + fabs(carObs->speed);//速度和，在本场景中保持不变
	double dis0 = fabs(car0->pmidf->y - carObs->pmidf->y);//初始距离
	if (total_speed <= 0.0)
	{
		cout << "两车都静止" << endl;
		return false;
	}

	double dis_r_L = road0->right_boundary - carObs->plr->x;//右间距
	double target_r_x = road0->right_boundary - dis_r_L / 2.0;//右目标点x
	cout << "dis_r_L = " << dis_r_L << ", target_r_x = " << target_r_x << endl;
	if (dis_r_L <= car0->car_width * 1.2)
	{
		cout << "右侧宽度不够超车" << endl;
		return false;
	}

	double time = dis0 / total_speed;//到碰撞的时间，循环次数
	double meeting_point_y = car0->pmidf->y + car0->speed * time;//碰撞点位置y
	double s0 = car0->pmidf->y - (meeting_point_y + start_dis);//直行到绕障点的距离
	cout << "total_speed = " << total_speed << ", dis0 = " << dis0 << ", meeting_point_y = " << meeting_point_y << ", s0 = " << s0 << endl;
	if (s0 < 0.0)
	{
		cout << "距离太近，不够绕开" << endl;
		return false;
	}

	uniformStraight(s0);//直行到变道起点
	Point target_r_point(target_r_x, meeting_point_y);//右目标点
	laneChange(target_r_point, LaneChangeType::doubleType);

	car0->updatePmidr();
	uniformStraight(car0->pmidr->y - 0.0);
	return true;
}