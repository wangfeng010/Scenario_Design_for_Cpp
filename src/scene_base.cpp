#include "scene_base.h"

void SceneBase::showScene()//显示，虚函数；
//这是默认情况，只画了road和car，如果画障碍物，需要重写这个虚函数
{
	BeginBatchDraw();  //开始绘制
	cleardevice();

	road0->showRoad();
	car0->showCar(BLACK);

	if (SHOWCIRCLE && car0->p_center)//绘制轨迹线 是否显示轨迹线+开始转向了
	{
        // showCircle()里需要p_center
		car0->showCircle(); //circle(p_center->x, p_center->y, Rof);
	}
	EndBatchDraw();  //结束绘制，全部绘制完 统一画这一帧
	delay(DELAYTIME);  
}

/******************************************直行******************************************/
void SceneBase::uniformStraight(const double& total_s)//直行，行驶指定距离 //total_s为正，为累计驶过的距离
{
	car0->updateStraightInfo();//更新直行信息

	double s = 0.0;
	while (s < total_s)
	{
		s += fabs(car0->speed);
		car0->moveStraightStep();
		obsMoveStep();
		showScene();//显示
	}
	car0->coutInfo();
}

void SceneBase::uniformAccBySpeed(const double& target_speed_y)//直行，已知加速度，行驶到指定的速度，匀加（减）速直线运动
{
	while (car0->pmidr->y > 0.0)
	{
		car0->moveStraightStep();
		obsMoveStep();
		if (fabs(car0->speed_y - target_speed_y) > fabs(car0->a_y))//当前车速与目标车速相差足够大
		{
			car0->speed_y += car0->a_y;
		}
		else
		{
			car0->speed_y = target_speed_y;
			car0->a_y = 0.0;

			if (target_speed_y == 0.0)//如果是停车，需要跳出循环，不然p0移动不到0.0的位置
			{
				break;
			}
		}
		showScene();//显示
	}
	car0->coutInfo();
}

void SceneBase::uniformAccByDis(const double& dis, const double& target_speed_y)//直行，行驶指定的距离时速度达到指定速度，匀加（减）速直线运动
{
	//计算加速度
	car0->a_y = (pow(car0->speed_y, 2) - pow(target_speed_y, 2)) / dis / 2.0; // 加速度
	cout << "a_y = " << car0->a_y << ", dis = " << dis << endl;

	uniformAccBySpeed(target_speed_y);
}

void SceneBase::uniformAccByTime(const double& target_speed_y, const double& target_time)//直行，在目标时间内达到目标速度，匀加（减）速直线运动
{
	//计算加速度
	double freq = target_time * 1000 / DELAYTIME;//进行wihile循环的次数
	car0->a_y = (target_speed_y - car0->speed_y) / freq;//加速度 //每次while循环中速度的变化值
	cout << "a_y = " << car0->a_y << endl;

	uniformAccBySpeed(target_speed_y);
}

/******************************************转向******************************************/
void SceneBase::carTurn(const int& turn_state, const double& R, const double& total_theta)//转向 //total_theta为正，为累计转过的角度
{
	car0->updateTurnInfo(turn_state, R);//更新转向信息

	double theta = 0.0;
	while (theta < total_theta)
	{
		theta += fabs(car0->delta_theta);
		correctAngleError(car0->delta_theta, theta - total_theta);//误差修正
		car0->carTurnStep();
		obsMoveStep();
		showScene();
	}
	car0->coutInfo();
}

void SceneBase::laneChange(const Point& target_point, const int& type, const double& s)//变道，单移线或双移线
{
	double dis = car0->pmidr->distanceTo(target_point);
	Vec2d vec0(dis, car0->heading_theta + PI / 2.0);
	Vec2d vec(*car0->pmidr, target_point);
	double L = vec0.crossProd(vec) / dis / 2.0;
	double H = vec0.innerProd(vec) / dis / 2.0;

	if (fabs(L) < 1e-10)//target_point在车辆直行方向上，不需要做绕行动作
	{
		uniformStraight(car0->pmidr->distanceTo(target_point));
		return;
	}

	double R = (pow(L, 2) + pow(H, 2)) / fabs(L) / 2.0;//转向半径 //L为0的情况在上面已经return了
	double target_theta = asin(H / R);//目标转角
	double target_delta_theta = fabs(car0->speed / R);//角速度绝对值
	cout << "dis = " << dis << ", L = " << L << ", H = " << H << ", R = " << R << ", target_theta = " << target_theta / PI << ", target_delta_theta = " << target_delta_theta / PI << endl;

	if (L > 0.0)//左绕
	{
		car0->delta_theta = target_delta_theta;
		carTurn(TurnDirection::TurnLeft, R, target_theta);

		if (type == singleType)
		{
			car0->delta_theta = -target_delta_theta;
			carTurn(TurnDirection::TurnRight, R, target_theta);
		}
		else
		{
			car0->delta_theta = -target_delta_theta;
			carTurn(TurnDirection::TurnRight, R, target_theta);

			uniformStraight(s);

			car0->delta_theta = -target_delta_theta;
			carTurn(TurnDirection::TurnRight, R, target_theta);

			car0->delta_theta = target_delta_theta;
			carTurn(TurnDirection::TurnLeft, R, target_theta);
		}
	}
	else if (L < 0.0)//右绕
	{
		car0->delta_theta = -target_delta_theta;
		carTurn(TurnDirection::TurnRight, R, target_theta);

		if (type == singleType)
		{
			car0->delta_theta = target_delta_theta;
			carTurn(TurnDirection::TurnLeft, R, target_theta);
		}
		else
		{
			car0->delta_theta = target_delta_theta;
			carTurn(TurnDirection::TurnLeft, R, target_theta);

			uniformStraight(s);//如果是超车，这里需要直行一段距离

			car0->delta_theta = target_delta_theta;
			carTurn(TurnDirection::TurnLeft, R, target_theta);

			car0->delta_theta = -target_delta_theta;
			carTurn(TurnDirection::TurnRight, R, target_theta);
		}
	}
}