#pragma once
#include<list>
#include "stdafx.h"

#define MAPLENTH 19
#define P_ROW 10
#define P_ARRAY 9
#define E_ROW 8
#define E_ARRAY 9

class GMap {
protected:
	static int LD;
	static int PD;
	void InitOP();
	bool mapData[MAPLENTH][MAPLENTH];
	bool peaMapData[MAPLENTH][MAPLENTH];
	COLORREF color;
public:
	void  DrawMap(HDC &hdc);              // 绘制地图
	void  DrawPeas(HDC &hdc);             // 绘制豆子
	virtual ~GMap() = 0;
	GMap() = default;
	friend class GObject;                   // 允许物体类使用直线的起点和终点的信息做碰撞检测
	friend class PacMan;                    // 允许"大嘴"访问豆子地图
};

class Stage_1 : public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];   //地图数据
public:
	Stage_1();
};

class Stage_2 : public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];
public:
	Stage_2();
};

class Stage_3 : public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];
public:
	Stage_3();
};
