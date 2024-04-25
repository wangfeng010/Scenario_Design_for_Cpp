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