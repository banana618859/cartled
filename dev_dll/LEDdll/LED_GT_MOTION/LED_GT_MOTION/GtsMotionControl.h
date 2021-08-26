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
//�˶�������
extern "C" EXT_FUNC int LTE_init();											//�˶����ƿ���ʼ��
extern "C" EXT_FUNC void LTE_close();										//�ر��˶����ƿ�

//�������
extern "C" EXT_FUNC void LTE_Stop();										//������ͣ
extern "C" EXT_FUNC void LTE_SmoothStop();									//ƽ����ͣ
extern "C" EXT_FUNC void LTE_ClrSts();										//״̬���
extern "C" EXT_FUNC short LTE_Axis_Enable_all(bool enable = false);			//ʹ��

//DIO
extern "C" EXT_FUNC long LTE_GetDI(short diType);							//ditype��2������(0)��2�Ḻ��(1)��2��ԭ��(3), 16��GPI(4)
extern "C" EXT_FUNC long LTE_GetDO();
extern "C" EXT_FUNC short LTE_GetDI_bit(short bit, short diType = MC_GPI);
extern "C" EXT_FUNC void LTE_SetDO_bit(short bit, short status, short diType = MC_GPO);

//�˶�
extern "C" EXT_FUNC void LTE_Jog(short axis, double vel, bool direction = true, double acc = 0.1, double dec = 0.1);							//�����˶�
extern "C" EXT_FUNC int LTE_PointMove(short axis, double vel, long pos, bool P_N_CHECK = true, double acc = 1, double dec = 1);					//�����˶�
extern "C" EXT_FUNC int LTE_GoHome(short axis, double vel, double acc = 0.25, double dec = 0.25, long offset = OFFSET_N);												//��λ
extern "C" EXT_FUNC int LTE_GoLimitN(short Axis, double vel, long offset = OFFSET_N);															//�ƶ���N��������ƫ��λ��
extern "C" EXT_FUNC int LTE_CLAMP(short axis, bool direction = true, double vel = 2, long distance_limit = CLAMP_DISTANCE_LIMIT);				//���֣����������

//�������
extern "C" EXT_FUNC void LTE_Axis_RESET_DRV_ALARM(short axis);				//�������������
extern "C" EXT_FUNC void LTE_Axis_Stop(short axis);							//ƽ��ֹͣ
extern "C" EXT_FUNC void LTE_Axis_Enable(short axis, bool enable = false);	//ʹ��
extern "C" EXT_FUNC void LTE_ZeroPos(short axis);							//����λ��
extern "C" EXT_FUNC long LTE_ReadPrfPos(short axis);						//��ȡ�滮λ��
extern "C" EXT_FUNC long LTE_ReadEncPos(short axis);						//��ȡ��ǰλ��
extern "C" EXT_FUNC long LTE_GetPrfMode(short axis);						//�˶�ģʽ��ѯ
extern "C" EXT_FUNC void LTE_Axis_ClrSts(short axis);						//״̬���
extern "C" EXT_FUNC long LTE_Axis_GetSts(short axis);						//״̬��ѯ
extern "C" EXT_FUNC void LTE_SyncPrfPos(short axis);						//λ��ͬ��

//�ƺͷ��������
extern "C" EXT_FUNC void LTE_LightCheck();

//�顢�����
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

bool bMotionStop;										//��ͣ״̬
long LimitN_soft_X = -9999999;							//X������λ
long LimitP_soft_X = 9999999;
long LimitN_soft_Z = -45000;							//Z������λ
long LimitP_soft_Z = 1105000;
	
#endif