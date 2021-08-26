
#define DLL_EXPORT

#include "pch.h"
#include "GtsMotionControl.h"
#include "stdlib.h"


//#include "GtsControl.h"
//#include "GtsControlDlg.h"


#define MC_LIMIT_POSITIVE               0//����������
#define MC_LIMIT_NEGATIVE               1//����������
#define MC_ALARM                        2//�ᱨ������
#define MC_HOME                         3//ԭ������
#define MC_GPI                          4//ͨ������
#define MC_ARRIVE                       5//�����λ�ź�����
#define MC_ENABLE						10//(�ú궨��Ϊ 10)�� ������ʹ�ܡ�
#define MC_CLEAR						11//(�ú궨��Ϊ 11)�� ���������
#define MC_GPO							12//(�ú궨��Ϊ 12)�� ͨ�������

#define BUZZER							0//������
#define RED_LIGHT						1//���
#define YELLOW_LIGHT					2//�Ƶ�
#define GREEN_LIGHT						3//�̵�
#define AXIS_Z_BRAKE					4//Z�ᱧբ

short error;

//Z���ԭ����
// ����home������������
#define Z_SEARCH_HOME 200000
// ���嵽home����λ�õ�ƫ����
#define Z_HOME_OFFSET 2000
// ����index������������
#define Z_SEARCH_INDEX -150000
// ���嵽index����λ�õ�ƫ����
#define Z_INDEX_OFFSET -2000

//X���ԭ����
// ����home������������
#define X_SEARCH_HOME 200000
// ���嵽home����λ�õ�ƫ����
#define X_HOME_OFFSET 2000
// ����index������������
#define X_SEARCH_INDEX -15000
// ���嵽index����λ�õ�ƫ����
#define X_INDEX_OFFSET -2000

#define XAxis		1
#define ZAxis		2
#define OutOn		0
#define OutOff		1

int add_dll(int a, int b) {
	return a + b;
}

void Commandhandler(CString command, short error)
{
	// ���ָ��ִ�з���ֵΪ��0��˵��ָ��ִ�д�������Ļ���������
	if (error != 0)
	{
		//MessageBox(command);//
		//CString strCommand;
		std::wcout << std::endl << "error>>>>>>>" << (LPCTSTR)command << std::endl;
		//strCommand.Format("%s:ָ��ִ�д���(Error:%d)",command,error);
		//AfxMessageBox(strCommand,0);//�Ͼֵģ�MFCһ����AfxMessageBox

	}
}


void LTE_SyncPrfPos(short axis)
{
	if (LTE_ReadPrfPos(axis) != LTE_ReadEncPos(axis))
	{
		GT_SetPrfPos(axis, LTE_ReadEncPos(axis));
	}
}

void LTE_close() {
	LTE_Stop();
	LTE_Axis_Enable_all(false);
	LTE_SetDO_bit(AXIS_Z_BRAKE, OutOff);//Z�ᱧբ
	error = GT_Close();
	Commandhandler(_T("GT_CLOSE"), error);
}

CString FIND_CFG_FILE(CString path) {
	CString filepath = path;
	CFileFind finder;
	filepath += _T("\\*.*");
	Commandhandler(filepath, 1);
	BOOL bFinderResult = finder.FindFile(filepath);
	while (bFinderResult != false)
	{
		bFinderResult = finder.FindNextFileW();
		if (finder.IsDots()) {
			continue;
		}
		CString filename = finder.GetFileName();
		if (filename.Find(_T("cartLed.cfg")) == 0) {
			filename = _T("\\") + filename;
			filename = path + filename;
			return filename;
		}
		if (finder.IsDirectory()) {
			CString directory = finder.GetFilePath();
			CString filepath = FIND_CFG_FILE(directory);
			if (filepath != _T("")) {
				return filepath;
			}
		}
	}
	return _T("");
}

int LTE_init()
{
	error = GT_Open();// ���˶�������
	if (error == 0)
	{
		error = GT_Reset();// ��λ�˶�������
		Commandhandler(_T("GT_Reset"), error);
		//CString cfg_path = FIND_CFG_FILE(_T(".."));
		//Commandhandler(cfg_path, 1);
		CString cfg_path = _T("C:\\cartledcfg\\cartLed.cfg");
		int len = WideCharToMultiByte(CP_ACP, 0, cfg_path, cfg_path.GetLength(), NULL, 0, NULL, NULL);
		char * pFileName = new char[len + 1];           
		WideCharToMultiByte(CP_ACP, 0, cfg_path, cfg_path.GetLength() + 1, pFileName, len + 1, NULL, NULL);
		pFileName[len] = '\0'; 
		error = GT_LoadConfig(pFileName); // ���������ļ�???ʹ���˶�������������� MCT2008 ���ɴ������ļ���
		if (error != 0) {
			Commandhandler(_T("GT_LoadConfig fail"), error);
			return 1;
		}
		error = GT_ClrSts(1, 4); // ���״̬
		Commandhandler(_T("GT_ClrSts"), error);
		LTE_SetDO_bit(0, OutOn, MC_CLEAR);//�������
		LTE_SetDO_bit(1, OutOn, MC_CLEAR);
		Sleep(300);
		LTE_SetDO_bit(0, OutOff, MC_CLEAR);
		LTE_SetDO_bit(1, OutOff, MC_CLEAR);
		Sleep(100);
		GT_SetStopDec(XAxis, 4, 3000);
		GT_SetStopDec(ZAxis, 4, 3000);
		LTE_SetDO_bit(AXIS_Z_BRAKE, OutOn);//�ɿ�Z�ᱧբ
		error = LTE_Axis_Enable_all(true);
		if (error != 0) {
			Commandhandler(_T("LTE_Axis_Enable_all fail"), error);
			return 2;
		}
		Commandhandler(_T("init finish"), error);
		Sleep(1200);
	}
	else
	{
		Commandhandler(_T("�˶����ƿ���GT_Open"), error);
	}
	return 0;
}


void LTE_Axis_Enable(short axis, bool enable) {
	if (true == enable) {
		error = GT_AxisOn(axis);
		Commandhandler(_T("GT_AxisOn"), error);
	}
	else {
		error = GT_AxisOff(axis);
		Commandhandler(_T("GT_AxisOff"), error);
	}
}

short LTE_Axis_Enable_all(bool enable) {
	if (false == enable) {
		for (int i = 1; i < 5; i++)
		{
			error = GT_AxisOff(i);
			Commandhandler(_T("GT_AxisOff"), error);
			if (0 != error) {
				return 1;
			}
		}
	}
	else {
		for (int i = 1; i < 5; i++)
		{
			error = GT_AxisOn(i);
			Commandhandler(_T("GT_AxisOn"), error);
			if (0 != error) {
				return 1;
			}
		}
	}
	return 0;
}

//ֹͣ������
void LTE_Stop()
{
	error = GT_Stop(1 << 8 | 15, 1 << 8 | 15);//��ֹͣ����ϵ�е�������
	Commandhandler(_T("GT_Stop_��ͣ"), error);
}

//ƽ��ֹͣ������
void LTE_SmoothStop()
{

	error = GT_Stop(1 << 8 | 15, 0);
	Commandhandler(_T("GT_Stop_ƽ��ֹͣ"), error);
}

//JOG�˶�
void LTE_Jog(short axis, double vel, bool direction, double acc, double dec)
{
	double vel_direction = 0;
	vel_direction = (true == direction) ? vel : vel * (-1);
	TJogPrm jog;
	error = GT_ClrSts(axis, 1);
	Commandhandler(_T("GT_ClrSts"), error);
	error = GT_PrfJog(axis);//���л�ΪJogģʽ
	Commandhandler(_T("GT_PrfJog"), error);
	error = GT_GetJogPrm(axis, &jog);// ��ȡJog�˶�����
	jog.acc = acc;
	jog.dec = dec;
	jog.smooth = 0.5;
	error = GT_SetJogPrm(axis, &jog);// ����Jog�˶�����
	Commandhandler(_T("GT_SetJogPrm"), error);
	error = GT_SetVel(axis, vel_direction);// �������Ŀ���ٶȣ���ֵΪ������
	Commandhandler(_T("GT_SetVel"), error);
	error = GT_Update(1 << (axis - 1));// �������JOG�˶�
	Commandhandler(_T("GT_Update"), error);
}

//�ƶ�������λ������һ��ƫ��λ��
int LTE_GoLimitN(short Axis, double vel, long offset) {
	int Re = LTE_PointMove(Axis, vel, -9999999, false);
	if ((TRUE == Re) && (false == bMotionStop))
	{
		long limit_sts = 0;
		while ((!(0x40 & limit_sts)) && (false == bMotionStop))
		{
			limit_sts = LTE_Axis_GetSts(Axis);
			Sleep(20);
		}
		Sleep(20);
	}
	Sleep(200);
	if (false == bMotionStop) {
		LTE_ZeroPos(Axis);
		Sleep(20);
		Re = LTE_PointMove(Axis, vel, offset);
		if (0 == Re) {
			Sleep(200);
			error = GT_ClrSts(Axis, 1);				//��������λ����
			Sleep(1000);								//��Ͳ���С��700�������ѯsts������
			long qsts = LTE_Axis_GetSts(Axis);
			return 0;
		}
		else {
			return Re;
		}
	}
	else {
		LTE_Stop();
		return 2;
	}
}

//��ԭ
int LTE_GoHome(short axis, double vel, double acc, double  dec, long offset) {
	// ���巵��ֵ����
	short sRtn;
	// �ƶ������޸���
	sRtn = LTE_GoLimitN(axis, vel, offset);
	if (sRtn != 0) {
		return 1;
	}
	// ����λ��
	LTE_ZeroPos(axis);
	// ����״̬
	short capture;
	// ��λ�˶������ṹ��
	TTrapPrm trapPrm;

	// ��״̬
	long status;
	// ����λ��
	long pos;
	// �ֱ��ǹ滮λ�ã�������λ�ã���Ĺ滮λ�ã���ı�����λ��
	double prfPos, encPos, axisPrfPos, axisEncPos;
	// �������home������
	sRtn = GT_SetCaptureMode(axis, CAPTURE_HOME);
	Commandhandler(_T("GT_SetCaptureMode"), sRtn);
	// ������Ϊ��λ�˶�ģʽ
	sRtn = GT_PrfTrap(axis);
	Commandhandler(_T("GT_PrfTrap"), sRtn);
	// ��ȡ��λ�˶�����
	sRtn = GT_GetTrapPrm(axis, &trapPrm);
	Commandhandler(_T("GT_GetTrapPrm"), sRtn);
	trapPrm.acc = acc;
	trapPrm.dec = dec;
	// ���õ�λ�˶�����
	sRtn = GT_SetTrapPrm(axis, &trapPrm);
	Commandhandler(_T("GT_SetTrapPrm"), sRtn);
	// ����Ŀ���ٶ�
	sRtn = GT_SetVel(axis, vel);
	Commandhandler(_T("GT_SetVel"), sRtn);
	// ����Ŀ��λ��
	sRtn = GT_SetPos(axis, Z_SEARCH_HOME);
	Commandhandler(_T("GT_SetPos"), sRtn);
	// �����˶����ȴ�home�źŴ���
	sRtn = GT_Update(1 << (axis - 1));
	Commandhandler(_T("GT_Update"), sRtn);
	//Sleep(2000);
	printf("\nWaiting for home signal...\n");
	// ���home����û�д�������һֱѭ��
	do
	{
		// ��ȡ��״̬
		sRtn = GT_GetSts(axis, &status);
		// ��ȡ����״̬
		sRtn = GT_GetCaptureStatus(axis, &capture, &pos);
		// ��ȡ�滮λ��
		sRtn = GT_GetPrfPos(axis, &prfPos);
		// ��ȡ������λ��
		sRtn = GT_GetEncPos(axis, &encPos);
		printf("capture=%d prfPos=%-10.1lf encPos=%-10.1lf\r", capture, prfPos, encPos);
		// ����Ѿ�ֹͣ��˵���������������� home �ź�һֱû�д���
		if (0 == (status & 0x400))
			//if (10 > abs(Z_SEARCH_HOME - prfPos))
		{
			printf("\nno home found");
			return 1;
		}
		// ���home�ź��Ѿ����������˳�ѭ��������λ���Ѿ���pos�����б���
	} while ((1 != capture) && (false == bMotionStop));
	if (true == bMotionStop) {
		LTE_Stop();
		return 2;
	}
	printf("\ncapture pos = %ld\n", pos);
	// �趨Ŀ��λ��Ϊ����λ��+ƫ����
	sRtn = GT_SetPos(axis, pos + Z_HOME_OFFSET);
	Commandhandler(_T("GT_SetPos"), sRtn);
	// �����˶�
	sRtn = GT_Update(1 << (axis - 1));
	Commandhandler(_T("GT_Update"), sRtn);
	do
	{
		// ��ȡ��״̬
		sRtn = GT_GetSts(axis, &status);
		// ��ȡ�滮λ��
		sRtn = GT_GetPrfPos(axis, &prfPos);
		// ��ȡ������λ��
		sRtn = GT_GetEncPos(axis, &encPos);
		printf("status=0x%-8lx prfPos=%-10.1lf encPos=%-10.1lf\r", status, prfPos, encPos);
	}//while ((10 < abs(pos + Z_HOME_OFFSET - prfPos) && (false == bMotionStop)));
	while ((status & 0x400) && (false == bMotionStop));
	if (true == bMotionStop) {
		LTE_Stop();
		return 2;
	}
	Sleep(200);
	sRtn = GT_GetPrfPos(axis, &prfPos);
	Commandhandler(_T("GT_GetPrfPos"), sRtn);
	if (prfPos != pos + Z_HOME_OFFSET)
	{
		printf("\nmove to home pos error");
		return 1;
	}
	// home�������
	printf("\nHome finish\n");
	Sleep(200);
	// ����index����
	sRtn = GT_SetCaptureMode(axis, CAPTURE_INDEX);
	Commandhandler(_T("GT_SetCaptureMode"), sRtn);
	// ���õ�ǰλ��+index ��������ΪĿ��λ��
	long index_pos = (long)prfPos + Z_SEARCH_INDEX;
	sRtn = GT_SetPos(axis, index_pos);
	Commandhandler(_T("GT_SetPos"), sRtn);
	// �����˶�
	sRtn = GT_Update(1 << (axis - 1));
	Commandhandler(_T("GT_Update"), sRtn);
	// �ȴ�index�����źŴ���
	printf("\nWaiting for index signal...\n");
	do
	{
		// ��ȡ��״̬
		sRtn = GT_GetSts(axis, &status);
		Commandhandler(_T("GT_GetSts"), sRtn);
		// ��ȡ����״̬
		sRtn = GT_GetCaptureStatus(axis, &capture, &pos);
		Commandhandler(_T("GT_GetCaptureStatus"), sRtn);
		// ��ȡ�滮λ��
		sRtn = GT_GetPrfPos(axis, &prfPos);
		Commandhandler(_T("GT_GetPrfPos"), sRtn);
		// ��ȡ������λ��
		sRtn = GT_GetEncPos(axis, &encPos);
		Commandhandler(_T("GT_GetEncPos"), sRtn);
		printf("capture=%d prfPos=%-10.1lf encPos=%-10.1lf\r", capture, prfPos, encPos);
		// ����Ѿ�ֹͣ��˵���������������� index �ź�һֱû�д���
		if (0 == (status & 0x400))
			//if (10 > abs(index_pos - prfPos))
		{
			printf("\nno index found\n");
			//getch();
			return 1;
		}
		// ���index�ź��Ѿ����������˳�ѭ��������λ���Ѿ���pos�����б���
	} while ((0 == capture) && (false == bMotionStop));
	if (true == bMotionStop) {
		LTE_Stop();
		return 2;
	}
	printf("\ncapture pos = %ld\n", pos);
	// ���ò���λ��+indexƫ����ΪĿ��λ��
	sRtn = GT_SetPos(axis, pos + Z_INDEX_OFFSET);
	Commandhandler(_T("GT_SetPos"), sRtn);
	// �����˶�
	sRtn = GT_Update(1 << (axis - 1));
	Commandhandler(_T("GT_Update"), sRtn);
	do
	{
		// ��ȡ��״̬
		sRtn = GT_GetSts(axis, &status);
		Commandhandler(_T("GT_GetSts"), sRtn);
		// ��ȡ�滮λ��
		sRtn = GT_GetPrfPos(axis, &prfPos);
		Commandhandler(_T("GT_GetPrfPos"), sRtn);
		// ��ȡ������λ��
		sRtn = GT_GetEncPos(axis, &encPos);
		Commandhandler(_T("GT_GetEncPos"), sRtn);
		printf("status=0x%-8lx prfPos=%-10.1lf encPos=%-10.1lf\r", status, prfPos, encPos);
	}// while ((10 < abs(pos + Z_INDEX_OFFSET - prfPos) && (false == bMotionStop)));
	while ((status & 0x400) && (false == bMotionStop));
	if (true == bMotionStop) {
		LTE_Stop();
		return 2;
	}
	Sleep(200);
	sRtn = GT_GetPrfPos(axis, &prfPos);
	Commandhandler(_T("GT_GetPrfPos"), sRtn);
	if (prfPos != pos + Z_INDEX_OFFSET)
	{
		printf("\nmove to index pos error\n");
		//getch();
		return 1;
	}
	// home+index�������
	printf("\nHome+Index finish\n");
	printf("\nPress any key to set pos as 0...\n");
	//gets_s();
	Sleep(200);
	// ����ԭ��λ��
	sRtn = GT_ZeroPos(axis);
	Commandhandler(_T("GT_ZeroPos"), sRtn);
	printf("GT_ZeroPos");
	sRtn = GT_GetPrfPos(axis, &prfPos);
	Commandhandler(_T("GT_GetPrfPos"), sRtn);
	sRtn = GT_GetEncPos(axis, &encPos);
	Commandhandler(_T("GT_GetEncPos"), sRtn);
	sRtn = GT_GetAxisPrfPos(axis, &axisPrfPos);
	Commandhandler(_T("GT_GetAxisPrfPos"), sRtn);
	sRtn = GT_GetAxisEncPos(axis, &axisEncPos);
	Commandhandler(_T("GT_GetAxisEncPos"), sRtn);
	printf("\nprfPos=%-10.0lf encPos=%-10.0lf axisPrfPos=%-10.0lf axisEncPos=%-10.0lf",
		prfPos, encPos, axisPrfPos, axisEncPos);
	//getch();
	return 0;
}

void LTE_ZeroPos(short axis)
{
	Sleep(200);
	error = GT_ZeroPos(axis);
	Commandhandler(_T("GT_ZeroPos"), error);
}


//���õ�λ�˶�
int LTE_PointMove(short axis, double vel, long pos, bool P_N_CHECK, double acc, double dec)
{
	long limit_P;
	long limit_N;
	if (XAxis == axis) {
		limit_P = LimitP_soft_X;
		limit_N = LimitN_soft_X;
	}
	else {
		limit_P = LimitP_soft_Z;
		limit_N = LimitN_soft_Z;
	}
	if (P_N_CHECK) {
		if ((pos < limit_N) || (pos > limit_P)) {
			Commandhandler(_T("out of range!!"), 1);
			return 1;
		}
	}
	long sts;
	LTE_SyncPrfPos(axis);
	TTrapPrm trapPrm;
	// �л�����λ�˶�ģʽ
	error = GT_PrfTrap(axis);
	Commandhandler(_T("GT_PrfTrap"), error);
	// ��ȡ��λģʽ�˶�����
	error = GT_GetTrapPrm(axis, &trapPrm);
	Commandhandler(_T("GT_GetTrapPrm"), error);
	trapPrm.acc = acc;
	trapPrm.dec = dec;
	trapPrm.smoothTime = 1;
	// ���õ�λģʽ�˶�����
	error = GT_SetTrapPrm(axis, &trapPrm);
	Commandhandler(_T("GT_SetTrapPrm"), error);
	// ���õ�λģʽĿ���ٶ�
	error = GT_SetVel(axis, vel);
	Commandhandler(_T("GT_SetVel"), error);
	error = GT_SetPos(axis, pos);
	Commandhandler(_T("GT_SetPos"), error);
	error = GT_Update(1 << (axis - 1));
	Commandhandler(_T("GT_Update"), error);
	for (;;)
	{
		// ��ȡAXIS���״̬
		error = GT_GetSts(axis, &sts);
		Sleep(20);

		if (!(sts & 0x400))
		{
			if (pos == LTE_ReadPrfPos(axis))
			{
				//CString encpos;
				//encpos.Format(_T("%d"), SE_ReadEncPos(axis));
				//Commandhandler(encpos, 1);
				return 0;
			}
		}
		if ((sts & 0x40) || (sts & 0x20)) {
			if (P_N_CHECK) {
				continue;
			}
			else return 1;
		}
		if (true == bMotionStop) {
			LTE_Stop();
			return 2;
		}

	}
}

long LTE_Axis_GetSts(short axis)
{
	long lAxisStatus;
	error = GT_ClrSts(axis, 1); // ���״̬
	Commandhandler(_T("GT_CltSts"), error);
	error = GT_GetSts(axis, &lAxisStatus);
	Commandhandler(_T("GT_GetSts"), error);
	return lAxisStatus;
}

//��������ǰλ��
long LTE_ReadPrfPos(short axis)
{
	double pos;
	error = GT_GetPrfPos(axis, &pos);
	Commandhandler(_T("GT_GetPrfPos"), error);
	return (long)pos;
}

//��������ǰλ��
long LTE_ReadEncPos(short axis)
{
	double enc_pos;
	error = GT_GetEncPos(axis, &enc_pos);
	Commandhandler(_T("GT_GetEncPos"), error);
	return (long)enc_pos;
}

//��ȡDI
short LTE_GetDI_bit(short io, short diType)
{
	long lGpiValue = 0;
	error = GT_GetDi(diType, &lGpiValue);
	//Commandhandler("GT_GetDi", error);
	if (lGpiValue & (1 << io))
		return 1;
	else
		return 0;
}

//��ȡDI all
long LTE_GetDI(short diType)
{
	long lGpiValue = 0;
	error = GT_GetDi(diType, &lGpiValue);
	return lGpiValue;
}

//����DO
void LTE_SetDO_bit(short io, short status, short diType)
{
	error = GT_SetDoBit(diType, io + 1, status);
	Commandhandler(_T(" GT_SetDoBit"), error);
}
//��ȡDO
long LTE_GetDO()
{
	long outStatus;
	error = GT_GetDo(MC_GPO, &outStatus);
	Commandhandler(_T(" GT_GetDo"), error);
	return outStatus;
}

long LTE_GetPrfMode(short axis)
{
	long pValue;
	error = GT_GetPrfMode(axis, &pValue, 1, NULL);
	Commandhandler(_T("GT_CrdStatus"), error);
	return pValue;
}

void LTE_ClrSts()
{
	error = GT_ClrSts(1, 4);
	Commandhandler(_T("GT_ClrSts"), error);
}

void LTE_Axis_ClrSts(short axis)
{
	error = GT_ClrSts(axis); // ���״̬
	Commandhandler(_T("GT_ClrSts"), error);
}



bool Get_MotionStop_sts()
{
	return  bMotionStop;
}

void Set_MotionStop_sts(bool sts)
{
	bMotionStop = sts;
}

void LTE_LightCheck()
{
	LTE_SetDO_bit(BUZZER, OutOn, MC_GPO);
	LTE_SetDO_bit(RED_LIGHT, OutOn, MC_GPO);
	LTE_SetDO_bit(YELLOW_LIGHT, OutOn, MC_GPO);
	LTE_SetDO_bit(GREEN_LIGHT, OutOn, MC_GPO);
	Sleep(500);
	LTE_SetDO_bit(BUZZER, OutOff, MC_GPO);
	LTE_SetDO_bit(RED_LIGHT, OutOff, MC_GPO);
	LTE_SetDO_bit(YELLOW_LIGHT, OutOff, MC_GPO);
	LTE_SetDO_bit(GREEN_LIGHT, OutOff, MC_GPO);
}

//direction:0����ת����ԭ��1����ת���н���
int LTE_CLAMP(short axis, bool direction, double vel, long distance_limit) {
	long c_pos = LTE_ReadEncPos(axis);
	short io_read;
	LTE_Jog(axis, vel, direction);
	io_read = (true == direction) ? (axis + 1) : (axis + 2);
	int i = 0;
	long sts;
	long dist = 0;
	short re = 0;
	for (;;)
	{
		// ��ȡAXIS���״̬
		error = GT_GetSts(axis, &sts);
		if (false == bMotionStop)
		{
			re = LTE_GetDI_bit(io_read, MC_GPI);
			if (1 == re) {
				LTE_Axis_Stop(axis);
				return 0;
			}
			dist = LTE_ReadPrfPos(axis) - c_pos;
			if (distance_limit < abs(dist))
			{
				LTE_Axis_Stop(axis);
				return 1;
			}
		}
		else {
			LTE_Stop();
			return 2;
		}
		Sleep(100);
		i++;
	}
}

void LTE_Axis_RESET_DRV_ALARM(short axis) {
	LTE_SetDO_bit(axis, OutOn, MC_CLEAR);
	Sleep(300);
	LTE_SetDO_bit(axis, OutOff, MC_CLEAR);
}

void LTE_Axis_Stop(short axis) {

	error = GT_Stop(1 << 8 | 1 << (axis - 1), 0);
	Commandhandler(_T("���ֵ�λ"), error);
}

long Get_LimitN_soft_X() {
	return LimitN_soft_X;
}
void Set_LimitN_soft_X(long limit) {
	LimitN_soft_X = limit;
}
long Get_LimitP_soft_X() {
	return LimitP_soft_X;
}
void Set_LimitP_soft_X(long limit) {
	LimitP_soft_X = limit;
}

long Get_LimitN_soft_Z() {
	return LimitN_soft_Z;
}
void Set_LimitN_soft_Z(long limit) {
	LimitN_soft_Z = limit;
}
long Get_LimitP_soft_Z() {
	return LimitP_soft_Z;
}
void Set_LimitP_soft_Z(long limit) {
	LimitP_soft_Z = limit;
}