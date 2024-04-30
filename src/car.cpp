#include "car.h"

/******************************************车基类******************************************/
void CarBase::initCar(const double& pos_x, const double& pos_y, const double& heading, const double& width, const double& length)//初始化
{
	car_width = width;
	car_length = length;
	heading_theta = heading;

	R0 = hypot(car_width / 2.0, car_length / 2.0);
	theta0 = atan(car_length / car_width);

	pmid = make_unique<Point>(pos_x, pos_y);//中心点		
	plf = make_unique<Point>(pmid->x - car_width / 2.0, pmid->y - car_length / 2.0, PI - theta0, R0);
	prf = make_unique<Point>(pmid->x + car_width / 2.0, pmid->y - car_length / 2.0, theta0, R0);
	plr = make_unique<Point>(pmid->x - car_width / 2.0, pmid->y + car_length / 2.0, PI + theta0, R0);
	prr = make_unique<Point>(pmid->x + car_width / 2.0, pmid->y + car_length / 2.0, -theta0, R0);

	plf->pointTurn(*pmid, heading_theta);
	prf->pointTurn(*pmid, heading_theta);
	plr->pointTurn(*pmid, heading_theta);
	prr->pointTurn(*pmid, heading_theta);

	updatePmidf();
	updatePmidr();
}

void CarBase::updatePmidf()//更新前中点xy值
{
	double x = (plf->x + prf->x) / 2.0;
	double y = (plf->y + prf->y) / 2.0;
	if (pmidf)
	{
		pmidf->x = x;
		pmidf->y = y;
	}
	else
	{
		pmidf = make_unique<Point>(x, y);
	}
}

void CarBase::updatePmidr()//更新后中点xy值
{
	double x = (plr->x + prr->x) / 2.0;
	double y = (plr->y + prr->y) / 2.0;
	if (pmidr)
	{
		pmidr->x = x;
		pmidr->y = y;
	}
	else
	{
		pmidr = make_unique<Point>(x, y);
	}
}

void CarBase::updatePmid()//更新几何中点xy值
{
	double x = (plf->x + prr->x) / 2.0;
	double y = (plf->y + prr->y) / 2.0;
	if (pmid)
	{
		pmid->x = x;
		pmid->y = y;
	}
	else
	{
		pmid = make_unique<Point>(x, y);
	}
}

void CarBase::showCar(const COLORREF& color)//绘制车辆矩形
{
	setlinestyle(PS_SOLID, 4);	// 设置线的样式
	setlinecolor(color);
	line(plf->x, plf->y, prf->x, prf->y);
	line(prf->x, prf->y, prr->x, prr->y);
	line(prr->x, prr->y, plr->x, plr->y);
	line(plr->x, plr->y, plf->x, plf->y);
}

void CarBase::showCircle()//绘制轨迹线
{
	setlinestyle(PS_DOT, 2);
	setlinecolor(MAGENTA);
	circle(p_center->x, p_center->y, Rof);
	circle(p_center->x, p_center->y, Ror);
	circle(p_center->x, p_center->y, Rif);
	circle(p_center->x, p_center->y, Rir);
}

void CarBase::coutInfo()//打印信息
{
	cout << "pmidr->x= " << pmidr->x << " pmidr->y= " << pmidr->y << " pmidr->Rp= " << pmidr->Rp << " pmidr->thetaP= " << pmidr->thetaP << endl;
	cout << "pmidf->x= " << pmidf->x << " pmidf->y= " << pmidf->y << " pmidf->Rp= " << pmidf->Rp << " pmidf->thetaP= " << pmidf->thetaP << endl;
	cout << "pmid->x= " << pmid->x << " pmid->y= " << pmid->y << " pmid->Rp= " << pmid->Rp << " pmid->thetaP= " << pmid->thetaP << endl;
	cout << "plf->x= " << plf->x << " plf->y= " << plf->y << " plf->Rp= " << plf->Rp << " plf->thetaP= " << plf->thetaP << endl;
	cout << "prf->x= " << prf->x << " prf->y= " << prf->y << " prf->Rp= " << prf->Rp << " prf->thetaP= " << prf->thetaP << endl;
	cout << "plr->x= " << plr->x << " plr->y= " << plr->y << " plr->Rp= " << plr->Rp << " plr->thetaP= " << plr->thetaP << endl;
	cout << "prr->x= " << prr->x << " prr->y= " << prr->y << " prr->Rp= " << prr->Rp << " prr->thetaP= " << prr->thetaP << endl;
	cout << "speed = " << speed << ", a = " << a << ", delta_theta = " << delta_theta / PI << ", delta_theta_rot = " << delta_theta_rot / PI << ", heading_theta = " << heading_theta / PI << endl;
}

void CarBase::moveStraightStep()//单帧直行
{
	plf->pointMove(speed_x, speed_y);
	prf->pointMove(speed_x, speed_y);
	plr->pointMove(speed_x, speed_y);
	prr->pointMove(speed_x, speed_y);
	pmidf->pointMove(speed_x, speed_y);
	pmidr->pointMove(speed_x, speed_y);
	pmid->pointMove(speed_x, speed_y);
}

void CarBase::carTurnStep()//单帧转向
{
	pmidr->pointTurn(*p_center, delta_theta);
	plf->pointTurn(*p_center, delta_theta);
	prf->pointTurn(*p_center, delta_theta);
	plr->pointTurn(*p_center, delta_theta);
	prr->pointTurn(*p_center, delta_theta);
	heading_theta += delta_theta;
}

void CarBase::updateRinRout(const double& R)//更新4个半径
{
	Ror = R + car_width / 2.0;
	Rir = R - car_width / 2.0;
	Rof = hypot(Ror, car_length); //sqrt(pow(Ror, 2.0) + pow(car_length, 2.0));
	Rif = hypot(Rir, car_length); //sqrt(pow(Rir, 2.0) + pow(car_length, 2.0));
}

void CarBase::updateTurnInfo(const int& turn_state, const double& R)//更新转向信息
{
	double x = 0.0;
	double y = 0.0;
	updateRinRout(R);//更新4个半径
	cout << "Rof = " << Rof << ", Rif = " << Rif << ", Ror = " << Ror << ", Rir = " << Rir << endl;

	if (turn_state == TurnDirection::TurnRight)//右转
	{
		//计算转向中心坐标
		x = pmidr->x + R * cos(heading_theta);
		y = pmidr->y - R * sin(heading_theta);

		//更新5个点角度和半径
		pmidr->thetaP = heading_theta + PI;
		pmidr->Rp = R;

		plr->thetaP = pmidr->thetaP;
		plr->Rp = Ror;

		prr->thetaP = pmidr->thetaP;
		prr->Rp = Rir;

		plf->thetaP = pmidr->thetaP - atan(car_length / Ror);
		plf->Rp = Rof;

		prf->thetaP = pmidr->thetaP - atan(car_length / Rir);
		prf->Rp = Rif;
	}
	else//左转
	{
		//计算转向中心坐标
		x = pmidr->x - R * cos(heading_theta);
		y = pmidr->y + R * sin(heading_theta);

		//更新5个点角度和半径
		pmidr->thetaP = heading_theta;
		pmidr->Rp = R;

		plr->thetaP = pmidr->thetaP;
		plr->Rp = Rir;

		prr->thetaP = pmidr->thetaP;
		prr->Rp = Ror;

		plf->thetaP = pmidr->thetaP + atan(car_length / Rir);
		plf->Rp = Rif;

		prf->thetaP = pmidr->thetaP + atan(car_length / Ror);
		prf->Rp = Rof;
	}
	cout << "center_turn.x= " << x << ", center_turn.y= " << y << endl;

	//更新转向中心
	if (p_center)
	{
		p_center->x = x;
		p_center->y = y;
	}
	else
	{
		p_center = make_unique<Point>(x, y);
	}
}

void CarBase::updateXYva()//更新x和y方向的速度和加速度
{
	speed_x = speed * sin(heading_theta);
	speed_y = speed * cos(heading_theta);
	a_x = a * sin(heading_theta);
	a_y = a * cos(heading_theta);
	cout << "speed_x = " << speed_x << ", speed_y = " << speed_y << ", a_x = " << a_x << ", a_y = " << a_y << endl;
}

void CarBase::updateStraightInfo()//更新直行信息
{
	updatePmidr();
	updatePmidf();
	updatePmid();
	updateXYva();  //更新X，Y方向的速度和加速度
	p_center.reset();//指针指向空

	Ror = 0.0;
	Rir = 0.0;
	Rof = 0.0;
	Rif = 0.0;

	pmidr->thetaP = 0.0;
	pmidr->Rp = 0.0;

	pmidf->thetaP = 0.0;
	pmidf->Rp = 0.0;

	pmid->thetaP = 0.0;
	pmid->Rp = 0.0;

	plr->thetaP = 0.0;
	plr->Rp = 0.0;

	prr->thetaP = 0.0;
	prr->Rp = 0.0;

	plf->thetaP = 0.0;
	plf->Rp = 0.0;

	prf->thetaP = 0.0;
	prf->Rp = 0.0;

	delta_theta = 0.0;
	delta_theta_rot = 0.0;
}

/******************************************一般车******************************************/
CarNormal::CarNormal(const double& pos_x, const double& pos_y, const double& heading, const double& width, const double& length) //一般车
{
	initCar(pos_x, pos_y, heading, width, length);
}