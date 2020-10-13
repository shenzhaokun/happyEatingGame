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
	void  DrawMap(HDC &hdc);              // ���Ƶ�ͼ
	void  DrawPeas(HDC &hdc);             // ���ƶ���
	virtual ~GMap() = 0;
	GMap() = default;
	friend class GObject;                   // ����������ʹ��ֱ�ߵ������յ����Ϣ����ײ���
	friend class PacMan;                    // ����"����"���ʶ��ӵ�ͼ
};

class Stage_1 : public GMap
{
private:
	bool static initData[MAPLENTH][MAPLENTH];   //��ͼ����
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
