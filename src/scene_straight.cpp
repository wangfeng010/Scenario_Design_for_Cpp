#include "scene_straight.h"

/******************************************停障******************************************/
StraightStopObs::StraightStopObs()//停障
{
	road0 = make_unique<RoadNormal>();
	cone = make_unique<Cone>(SWIDTH / 2.0, SWIDTH / 4.0, 50.0);//放置锥桶

	car0 = make_unique<CarNormal>(SWIDTH / 2.0, SHEIGHT - 70.0);
	car0->speed_y = -5.0;

	car0->coutInfo();
	showScene();
	system("pause");
}

void StraightStopObs::showScene()//显示
{
	BeginBatchDraw();
	cleardevice();

	road0->showRoad();
	cone->showCone();
	car0->showCar(BLACK);

	EndBatchDraw();
	delay(DELAYTIME);
}

bool StraightStopObs::planning_process()//整个过程
{
	double stopline = cone->p_center->y + cone->r + safedis;
	uniformAccByDis(car0->pmidf->y - stopline, 0.0);
	return true;
}

/******************************************停靠站点******************************************/
StraightStation::StraightStation()//停靠站点
{
	road0 = make_unique<RoadNormal>();
	station = make_unique<Point>(SWIDTH / 2.0, SHEIGHT / 2.0);

	car0 = make_unique<CarNormal>(SWIDTH / 2.0, SHEIGHT - 70.0);
	car0->speed_y = -5.0;

	car0->coutInfo();
	showScene();
	system("pause");
}

void StraightStation::showScene()//显示
{
	BeginBatchDraw();
	cleardevice();

	road0->showRoad();
	station->showPoint();
	car0->showCar(BLACK);

	EndBatchDraw();
	delay(DELAYTIME);
}

bool StraightStation::planning_process()//整个过程
{
	uniformAccByDis(car0->pmid->y - station->y, 0.0);//减速进站
	delay(stop_time * 1000);//停站等待
	uniformAccByTime(speedlimit, 2.0);//出站
	return true;
}

/******************************************跟车******************************************/
StraightFollow::StraightFollow()//跟车
{
	road0 = make_unique<RoadNormal>();
	carObs = make_unique<CarNormal>(SWIDTH / 2.0, SHEIGHT / 2.0, 0.0, 50.0, 100.0);
	car0 = make_unique<CarNormal>(SWIDTH / 2.0, SHEIGHT - 70.0);
	carObs->speed_y = -2.0;
	car0->speed_y = -5.0;

	car0->coutInfo();
	carObs->coutInfo();
	showScene();
	system("pause");
}

void StraightFollow::showScene()//显示
{
	BeginBatchDraw();
	cleardevice();

	road0->showRoad();
	carObs->showCar(RED);
	car0->showCar(BLACK);

	EndBatchDraw();
	delay(DELAYTIME);
}

bool StraightFollow::planning_process()//整个过程
{
	double dis = car0->pmidf->y - carObs->pmidr->y;//初始间距
	double delta_dis = dis - safedis;//初始间距-目标间距
	double delta_speed_y = car0->speed_y - carObs->speed_y;//速度差
	if (dis <= 0.0 || delta_dis <= 0.0 || delta_speed_y > 0.0)//只考虑主车车速更快、需要减速的情况
	{
		return false;
	}

	car0->a_y = pow(delta_speed_y, 2) / delta_dis / 2.0;
	cout << "car0->a_y = " << car0->a_y << endl;

	while (car0->pmidr->y > 0.0)
	{
		car0->moveStraightStep();
		carObs->moveStraightStep();
		if (fabs(car0->speed_y - carObs->speed_y) > fabs(car0->a_y))//速度差足够大
		{
			car0->speed_y += car0->a_y;
		}
		else
		{
			car0->speed_y = carObs->speed_y;
			car0->a_y = 0.0;
		}
		showScene();//显示		
	}
	car0->coutInfo();
	return true;
}

/******************************************通过斑马线******************************************/
StraightCrosswalk::StraightCrosswalk()//通过斑马线
{
	road0 = make_unique<RoadCrosswalk>();
	car0 = make_unique<CarNormal>(SWIDTH / 2.0, SHEIGHT - 70.0);
	car0->speed_y = -4.0;

	for (int i = 0; i < people_num; i++)
	{
		unique_ptr<Person> ps = make_unique<Person>(road0->right_boundary + 20 * (i * 3 + 1), road0->getMidLine());
		ps->speed = -2;
		people.emplace_back(move(ps));
	}

	car0->coutInfo();
	showScene();//显示
	system("pause");
}

bool StraightCrosswalk::peopleInCross()//斑马线是否有人
{
	for (auto& i : people)
	{
		if (i->p_center->x >= road0->left_boundary - i->r && i->p_center->x <= road0->right_boundary + i->r)
		{
			return true;
		}
	}
	return false;
}

void StraightCrosswalk::showScene()//显示
{
	BeginBatchDraw();
	cleardevice();

	road0->showRoad();
	car0->showCar(BLACK);

	car0->moveStraightStep();
	car0->speed_y += car0->a_y;

	for (auto& i : people)
	{
		i->showPerson();
		i->personMove();
	}

	EndBatchDraw();
	delay(DELAYTIME);
}

bool StraightCrosswalk::planning_process()
{
	//进入斑马线场景，一律先减速
	double dis = car0->pmidf->y - road0->getDownLine();
	car0->a_y = pow(car0->speed_y, 2) / 2.0 / dis;

	while (dis > 0.0)
	{
		dis = car0->pmidf->y - road0->getDownLine();
		if (!peopleInCross())//如果斑马线没人
		{
			if (car0->speed_y >= speedlimit_cross)//斑马线限速
			{
				car0->a_y = 0.0;//变为匀速运动
			}
		}
		else
		{
			if (dis <= 7.0)//停止线前让车辆完全停下，防止慢速挪动或倒车
			{
				car0->speed_y = 0.0;
				car0->a_y = 0.0;
				break;
			}
		}

		cout << "dis = " << dis << ", car speed_y = " << car0->speed_y << ", a_y = " << car0->a_y << endl;
		showScene();//显示
	}

	//限速通过斑马线
	while (car0->pmidr->y > road0->getUpLine())//斑马线区间
	{
		if (!peopleInCross())//如果斑马线没人了
		{
			if (car0->speed_y > speedlimit_cross)//没达到斑马线限速
			{
				car0->a_y = -0.05;//加速
			}
			else //达到斑马线限速
			{
				car0->a_y = 0.0;//变为匀速运动
			}
		}

		cout << "car speed_y = " << car0->speed_y << ", a_y = " << car0->a_y << endl;
		showScene();//显示
	}

	//过了斑马线，继续加速，直到达到道路限速
	while (car0->pmidr->y > 0.0)
	{
		if (car0->speed_y > speedlimit)//没达到道路限速
		{
			car0->a_y = -0.05;//加速
		}
		else
		{
			car0->a_y = 0.0;//变为匀速运动
		}

		cout << "car speed_y = " << car0->speed_y << ", a_y = " << car0->a_y << endl;
		showScene();//显示
	}

	return true;
}