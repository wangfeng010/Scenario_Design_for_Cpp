#include "main.h"

bool process(const int& type)//总过程
{
	switch (type)
	{
	case PlanType::StraightStopObsType:
	{
		unique_ptr<SceneBase> plan_obj = make_unique<StraightStopObs>();
		return plan_obj->planning_process();
	}
	case PlanType::StraightStationType:
	{
		unique_ptr<SceneBase> plan_obj = make_unique<StraightStation>();
		return plan_obj->planning_process();
	}
	case PlanType::StraightFollowType:
	{
		unique_ptr<SceneBase> plan_obj = make_unique<StraightFollow>();
		return plan_obj->planning_process();
	}
	case PlanType::StraightCrosswalkType:
	{
		unique_ptr<SceneBase> plan_obj = make_unique<StraightCrosswalk>();
		return plan_obj->planning_process();
	}
	default:
		break;
	}
	cout << "plan type is not right" << endl;
	return false;
}

int main()
{
	initgraph(SWIDTH, SHEIGHT, EW_SHOWCONSOLE);//图形窗口
	setbkcolor(WHITE);//设置背景颜色
	cleardevice();

	if (process(PlanType::StraightCrosswalkType))
	{
		cout << "完成" << endl;
	}

	system("pause");
	closegraph();
	return 0;
}