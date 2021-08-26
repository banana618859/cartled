#if !defined(AFX_MCOMMCTRLOR_H__EAF52ECF_B1D6_4A0F_BB60_E525F19E96FE__INCLUDED_)
#define AFX_MCOMMCTRLOR_H__EAF52ECF_B1D6_4A0F_BB60_E525F19E96FE__INCLUDED_

#pragma once
#ifdef DLL_EXPORT
#define EXT_FUNC _declspec(dllexport)
#else
#define EXT_FUNC _declspec(dllimport)
#endif

//#include "windows.h"
#include "afxwin.h"
#include "gts.h"
#include "atlstr.h"
#include "string.h"
#include "stdio.h"
#include "iostream"

#define OFFSET_N 2000
#define CLAMP_DISTANCE_LIMIT 5000

extern "C" EXT_FUNC void Commandhandler(CString command, short error);
extern "C" EXT_FUNC int add_dll(int a, int b);
//运动控制器
extern "C" EXT_FUNC int LTE_init();											//运动控制卡初始化
extern "C" EXT_FUNC void LTE_close();										//关闭运动控制卡

//多轴控制
extern "C" EXT_FUNC void LTE_Stop();										//紧急轴停
extern "C" EXT_FUNC void LTE_SmoothStop();									//平滑轴停
extern "C" EXT_FUNC void LTE_ClrSts();										//状态清除
extern "C" EXT_FUNC short LTE_Axis_Enable_all(bool enable = false);			//使能

//DIO
extern "C" EXT_FUNC long LTE_GetDI(short diType);							//ditype：2轴正限(0)，2轴负限(1)，2轴原点(3), 16个GPI(4)
extern "C" EXT_FUNC long LTE_GetDO();
extern "C" EXT_FUNC short LTE_GetDI_bit(short bit, short diType = MC_GPI);
extern "C" EXT_FUNC void LTE_SetDO_bit(short bit, short status, short diType = MC_GPO);

//运动
extern "C" EXT_FUNC void LTE_Jog(short axis, double vel, bool direction = true, double acc = 0.1, double dec = 0.1);							//匀速运动
extern "C" EXT_FUNC int LTE_PointMove(short axis, double vel, long pos, bool P_N_CHECK = true, double acc = 1, double dec = 1);					//定点运动
extern "C" EXT_FUNC int LTE_GoHome(short axis, double vel, double acc = 0.25, double dec = 0.25, long offset = OFFSET_N);						//复位
extern "C" EXT_FUNC int LTE_GoLimitN(short Axis, double vel, long offset = OFFSET_N);															//移动到N极附近的偏移位置
extern "C" EXT_FUNC int LTE_CLAMP(short axis, bool direction = true, double vel = 2, long distance_limit = CLAMP_DISTANCE_LIMIT);				//夹手（步进电机）

//单轴控制
extern "C" EXT_FUNC void LTE_Axis_RESET_DRV_ALARM(short axis);				//清除驱动器报警
extern "C" EXT_FUNC void LTE_Axis_Stop(short axis);							//平滑停止
extern "C" EXT_FUNC void LTE_Axis_Enable(short axis, bool enable = false);	//使能
extern "C" EXT_FUNC void LTE_ZeroPos(short axis);							//清零位置
extern "C" EXT_FUNC long LTE_ReadPrfPos(short axis);						//读取规划位置
extern "C" EXT_FUNC long LTE_ReadEncPos(short axis);						//读取当前位置
extern "C" EXT_FUNC long LTE_GetPrfMode(short axis);						//运动模式查询
extern "C" EXT_FUNC void LTE_Axis_ClrSts(short axis);						//状态清除
extern "C" EXT_FUNC long LTE_Axis_GetSts(short axis);						//状态查询
extern "C" EXT_FUNC void LTE_SyncPrfPos(short axis);						//位置同步

//灯和蜂鸣器检查
extern "C" EXT_FUNC void LTE_LightCheck();

//查、设参数
extern "C" EXT_FUNC bool Get_MotionStop_sts();
extern "C" EXT_FUNC void Set_MotionStop_sts(bool sts);

extern "C" EXT_FUNC long Get_LimitN_soft_X();
extern "C" EXT_FUNC void Set_LimitN_soft_X(long limit);
extern "C" EXT_FUNC long Get_LimitP_soft_X();
extern "C" EXT_FUNC void Set_LimitP_soft_X(long limit);

extern "C" EXT_FUNC long Get_LimitN_soft_Z();
extern "C" EXT_FUNC void Set_LimitN_soft_Z(long limit);
extern "C" EXT_FUNC long Get_LimitP_soft_Z();
extern "C" EXT_FUNC void Set_LimitP_soft_Z(long limit);

//printer
extern "C" EXT_FUNC void Commandhandler(CString command, short error);

CString FIND_CFG_FILE(CString path);

bool bMotionStop;										//急停状态
long LimitN_soft_X = -9999999;							//X轴软限位
long LimitP_soft_X = 9999999;
long LimitN_soft_Z = -45000;							//Z轴软限位
long LimitP_soft_Z = 1105000;
	
#endif