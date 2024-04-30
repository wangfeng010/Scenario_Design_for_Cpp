# pragma once
#include "scene_straight.h"
#include "scene_obs_pass.h"

enum PlanType//类型
{
	//直行
	StraightStopObsType,//停障
	StraightStationType,//停靠站点
	StraightFollowType,//跟车
	StraightCrosswalkType,//通过斑马线

	//绕障
	ObsPassStaticType,//静态绕障
	ObsPassOvertakeType,//超车
	ObsPassMeetingType,//会车
};