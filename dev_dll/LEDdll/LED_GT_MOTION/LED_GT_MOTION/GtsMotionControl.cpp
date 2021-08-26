
#define DLL_EXPORT

#include "pch.h"
#include "GtsMotionControl.h"
#include "stdlib.h"


//#include "GtsControl.h"
//#include "GtsControlDlg.h"


#define MC_LIMIT_POSITIVE               0//正极限输入
#define MC_LIMIT_NEGATIVE               1//负极限输入
#define MC_ALARM                        2//轴报警输入
#define MC_HOME                         3//原点输入
#define MC_GPI                          4//通用输入
#define MC_ARRIVE                       5//电机到位信号输入
#define MC_ENABLE						10//(该宏定义为 10)： 驱动器使能。
#define MC_CLEAR						11//(该宏定义为 11)： 报警清除。
#define MC_GPO							12//(该宏定义为 12)： 通用输出。

#define BUZZER							0//蜂鸣器
#define RED_LIGHT						1//红灯
#define YELLOW_LIGHT					2//黄灯
#define GREEN_LIGHT						3//绿灯
#define AXIS_Z_BRAKE					4//Z轴抱闸

short error;

//Z轴回原参数
// 定义home回零搜索距离
#define Z_SEARCH_HOME 200000
// 定义到home捕获位置的偏移量
#define Z_HOME_OFFSET 2000
// 定义index回零搜索距离
#define Z_SEARCH_INDEX -150000
// 定义到index捕获位置的偏移量
#define Z_INDEX_OFFSET -2000

//X轴回原参数
// 定义home回零搜索距离
#define X_SEARCH_HOME 200000
// 定义到home捕获位置的偏移量
#define X_HOME_OFFSET 2000
// 定义index回零搜索距离
#define X_SEARCH_INDEX -15000
// 定义到index捕获位置的偏移量
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
	// 如果指令执行返回值为非0，说明指令执行错误，向屏幕输出错误结果
	if (error != 0)
	{
		//MessageBox(command);//
		//CString strCommand;
		std::wcout << std::endl << "error>>>>>>>" << (LPCTSTR)command << std::endl;
		//strCommand.Format("%s:指令执行错误！(Error:%d)",command,error);
		//AfxMessageBox(strCommand,0);//合局的，MFC一般用AfxMessageBox

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
	LTE_SetDO_bit(AXIS_Z_BRAKE, OutOff);//Z轴抱闸
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
	error = GT_Open();// 打开运动控制器
	if (error == 0)
	{
		error = GT_Reset();// 复位运动控制器
		Commandhandler(_T("GT_Reset"), error);
		//CString cfg_path = FIND_CFG_FILE(_T(".."));
		//Commandhandler(cfg_path, 1);
		CString cfg_path = _T("C:\\cartledcfg\\cartLed.cfg");
		int len = WideCharToMultiByte(CP_ACP, 0, cfg_path, cfg_path.GetLength(), NULL, 0, NULL, NULL);
		char * pFileName = new char[len + 1];           
		WideCharToMultiByte(CP_ACP, 0, cfg_path, cfg_path.GetLength() + 1, pFileName, len + 1, NULL, NULL);
		pFileName[len] = '\0'; 
		error = GT_LoadConfig(pFileName); // 下载配置文件???使用运动控制器管理软件 MCT2008 生成此配置文件。
		if (error != 0) {
			Commandhandler(_T("GT_LoadConfig fail"), error);
			return 1;
		}
		error = GT_ClrSts(1, 4); // 清除状态
		Commandhandler(_T("GT_ClrSts"), error);
		LTE_SetDO_bit(0, OutOn, MC_CLEAR);//报警清除
		LTE_SetDO_bit(1, OutOn, MC_CLEAR);
		Sleep(300);
		LTE_SetDO_bit(0, OutOff, MC_CLEAR);
		LTE_SetDO_bit(1, OutOff, MC_CLEAR);
		Sleep(100);
		GT_SetStopDec(XAxis, 4, 3000);
		GT_SetStopDec(ZAxis, 4, 3000);
		LTE_SetDO_bit(AXIS_Z_BRAKE, OutOn);//松开Z轴抱闸
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
		Commandhandler(_T("运动控制卡打开GT_Open"), error);
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

//停止所有轴
void LTE_Stop()
{
	error = GT_Stop(1 << 8 | 15, 1 << 8 | 15);//急停止坐标系中的所有轴
	Commandhandler(_T("GT_Stop_急停"), error);
}

//平滑停止所有轴
void LTE_SmoothStop()
{

	error = GT_Stop(1 << 8 | 15, 0);
	Commandhandler(_T("GT_Stop_平滑停止"), error);
}

//JOG运动
void LTE_Jog(short axis, double vel, bool direction, double acc, double dec)
{
	double vel_direction = 0;
	vel_direction = (true == direction) ? vel : vel * (-1);
	TJogPrm jog;
	error = GT_ClrSts(axis, 1);
	Commandhandler(_T("GT_ClrSts"), error);
	error = GT_PrfJog(axis);//轴切换为Jog模式
	Commandhandler(_T("GT_PrfJog"), error);
	error = GT_GetJogPrm(axis, &jog);// 读取Jog运动参数
	jog.acc = acc;
	jog.dec = dec;
	jog.smooth = 0.5;
	error = GT_SetJogPrm(axis, &jog);// 设置Jog运动参数
	Commandhandler(_T("GT_SetJogPrm"), error);
	error = GT_SetVel(axis, vel_direction);// 设置轴的目标速度，正值为正方向
	Commandhandler(_T("GT_SetVel"), error);
	error = GT_Update(1 << (axis - 1));// 启动轴的JOG运动
	Commandhandler(_T("GT_Update"), error);
}

//移动到负限位附近的一个偏移位置
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
			error = GT_ClrSts(Axis, 1);				//消除负限位报警
			Sleep(1000);								//最低不能小于700，否则查询sts有问题
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

//回原
int LTE_GoHome(short axis, double vel, double acc, double  dec, long offset) {
	// 定义返回值变量
	short sRtn;
	// 移动到正限附近
	sRtn = LTE_GoLimitN(axis, vel, offset);
	if (sRtn != 0) {
		return 1;
	}
	// 清零位置
	LTE_ZeroPos(axis);
	// 捕获状态
	short capture;
	// 点位运动参数结构体
	TTrapPrm trapPrm;

	// 轴状态
	long status;
	// 捕获位置
	long pos;
	// 分别是规划位置，编码器位置，轴的规划位置，轴的编码器位置
	double prfPos, encPos, axisPrfPos, axisEncPos;
	// 开启轴的home捕获功能
	sRtn = GT_SetCaptureMode(axis, CAPTURE_HOME);
	Commandhandler(_T("GT_SetCaptureMode"), sRtn);
	// 设置轴为点位运动模式
	sRtn = GT_PrfTrap(axis);
	Commandhandler(_T("GT_PrfTrap"), sRtn);
	// 读取点位运动参数
	sRtn = GT_GetTrapPrm(axis, &trapPrm);
	Commandhandler(_T("GT_GetTrapPrm"), sRtn);
	trapPrm.acc = acc;
	trapPrm.dec = dec;
	// 设置点位运动参数
	sRtn = GT_SetTrapPrm(axis, &trapPrm);
	Commandhandler(_T("GT_SetTrapPrm"), sRtn);
	// 设置目标速度
	sRtn = GT_SetVel(axis, vel);
	Commandhandler(_T("GT_SetVel"), sRtn);
	// 设置目标位置
	sRtn = GT_SetPos(axis, Z_SEARCH_HOME);
	Commandhandler(_T("GT_SetPos"), sRtn);
	// 启动运动，等待home信号触发
	sRtn = GT_Update(1 << (axis - 1));
	Commandhandler(_T("GT_Update"), sRtn);
	//Sleep(2000);
	printf("\nWaiting for home signal...\n");
	// 如果home捕获没有触发，就一直循环
	do
	{
		// 读取轴状态
		sRtn = GT_GetSts(axis, &status);
		// 读取捕获状态
		sRtn = GT_GetCaptureStatus(axis, &capture, &pos);
		// 读取规划位置
		sRtn = GT_GetPrfPos(axis, &prfPos);
		// 读取编码器位置
		sRtn = GT_GetEncPos(axis, &encPos);
		printf("capture=%d prfPos=%-10.1lf encPos=%-10.1lf\r", capture, prfPos, encPos);
		// 电机已经停止，说明整个搜索过程中 home 信号一直没有触发
		if (0 == (status & 0x400))
			//if (10 > abs(Z_SEARCH_HOME - prfPos))
		{
			printf("\nno home found");
			return 1;
		}
		// 如果home信号已经触发，则退出循环，捕获位置已经在pos变量中保存
	} while ((1 != capture) && (false == bMotionStop));
	if (true == bMotionStop) {
		LTE_Stop();
		return 2;
	}
	printf("\ncapture pos = %ld\n", pos);
	// 设定目标位置为捕获位置+偏移量
	sRtn = GT_SetPos(axis, pos + Z_HOME_OFFSET);
	Commandhandler(_T("GT_SetPos"), sRtn);
	// 启动运动
	sRtn = GT_Update(1 << (axis - 1));
	Commandhandler(_T("GT_Update"), sRtn);
	do
	{
		// 读取轴状态
		sRtn = GT_GetSts(axis, &status);
		// 读取规划位置
		sRtn = GT_GetPrfPos(axis, &prfPos);
		// 读取编码器位置
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
	// home捕获完毕
	printf("\nHome finish\n");
	Sleep(200);
	// 启动index捕获
	sRtn = GT_SetCaptureMode(axis, CAPTURE_INDEX);
	Commandhandler(_T("GT_SetCaptureMode"), sRtn);
	// 设置当前位置+index 搜索距离为目标位置
	long index_pos = (long)prfPos + Z_SEARCH_INDEX;
	sRtn = GT_SetPos(axis, index_pos);
	Commandhandler(_T("GT_SetPos"), sRtn);
	// 启动运动
	sRtn = GT_Update(1 << (axis - 1));
	Commandhandler(_T("GT_Update"), sRtn);
	// 等待index捕获信号触发
	printf("\nWaiting for index signal...\n");
	do
	{
		// 读取轴状态
		sRtn = GT_GetSts(axis, &status);
		Commandhandler(_T("GT_GetSts"), sRtn);
		// 读取捕获状态
		sRtn = GT_GetCaptureStatus(axis, &capture, &pos);
		Commandhandler(_T("GT_GetCaptureStatus"), sRtn);
		// 读取规划位置
		sRtn = GT_GetPrfPos(axis, &prfPos);
		Commandhandler(_T("GT_GetPrfPos"), sRtn);
		// 读取编码器位置
		sRtn = GT_GetEncPos(axis, &encPos);
		Commandhandler(_T("GT_GetEncPos"), sRtn);
		printf("capture=%d prfPos=%-10.1lf encPos=%-10.1lf\r", capture, prfPos, encPos);
		// 电机已经停止，说明整个搜索过程中 index 信号一直没有触发
		if (0 == (status & 0x400))
			//if (10 > abs(index_pos - prfPos))
		{
			printf("\nno index found\n");
			//getch();
			return 1;
		}
		// 如果index信号已经触发，则退出循环，捕获位置已经在pos变量中保存
	} while ((0 == capture) && (false == bMotionStop));
	if (true == bMotionStop) {
		LTE_Stop();
		return 2;
	}
	printf("\ncapture pos = %ld\n", pos);
	// 设置捕获位置+index偏移量为目标位置
	sRtn = GT_SetPos(axis, pos + Z_INDEX_OFFSET);
	Commandhandler(_T("GT_SetPos"), sRtn);
	// 启动运动
	sRtn = GT_Update(1 << (axis - 1));
	Commandhandler(_T("GT_Update"), sRtn);
	do
	{
		// 读取轴状态
		sRtn = GT_GetSts(axis, &status);
		Commandhandler(_T("GT_GetSts"), sRtn);
		// 读取规划位置
		sRtn = GT_GetPrfPos(axis, &prfPos);
		Commandhandler(_T("GT_GetPrfPos"), sRtn);
		// 读取编码器位置
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
	// home+index捕获完毕
	printf("\nHome+Index finish\n");
	printf("\nPress any key to set pos as 0...\n");
	//gets_s();
	Sleep(200);
	// 更新原点位置
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


//设置点位运动
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
	// 切换到点位运动模式
	error = GT_PrfTrap(axis);
	Commandhandler(_T("GT_PrfTrap"), error);
	// 读取点位模式运动参数
	error = GT_GetTrapPrm(axis, &trapPrm);
	Commandhandler(_T("GT_GetTrapPrm"), error);
	trapPrm.acc = acc;
	trapPrm.dec = dec;
	trapPrm.smoothTime = 1;
	// 设置点位模式运动参数
	error = GT_SetTrapPrm(axis, &trapPrm);
	Commandhandler(_T("GT_SetTrapPrm"), error);
	// 设置点位模式目标速度
	error = GT_SetVel(axis, vel);
	Commandhandler(_T("GT_SetVel"), error);
	error = GT_SetPos(axis, pos);
	Commandhandler(_T("GT_SetPos"), error);
	error = GT_Update(1 << (axis - 1));
	Commandhandler(_T("GT_Update"), error);
	for (;;)
	{
		// 读取AXIS轴的状态
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
	error = GT_ClrSts(axis, 1); // 清楚状态
	Commandhandler(_T("GT_CltSts"), error);
	error = GT_GetSts(axis, &lAxisStatus);
	Commandhandler(_T("GT_GetSts"), error);
	return lAxisStatus;
}

//编码器当前位置
long LTE_ReadPrfPos(short axis)
{
	double pos;
	error = GT_GetPrfPos(axis, &pos);
	Commandhandler(_T("GT_GetPrfPos"), error);
	return (long)pos;
}

//编码器当前位置
long LTE_ReadEncPos(short axis)
{
	double enc_pos;
	error = GT_GetEncPos(axis, &enc_pos);
	Commandhandler(_T("GT_GetEncPos"), error);
	return (long)enc_pos;
}

//读取DI
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

//读取DI all
long LTE_GetDI(short diType)
{
	long lGpiValue = 0;
	error = GT_GetDi(diType, &lGpiValue);
	return lGpiValue;
}

//设置DO
void LTE_SetDO_bit(short io, short status, short diType)
{
	error = GT_SetDoBit(diType, io + 1, status);
	Commandhandler(_T(" GT_SetDoBit"), error);
}
//获取DO
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
	error = GT_ClrSts(axis); // 清除状态
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

//direction:0：正转，回原；1：反转，夹紧；
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
		// 读取AXIS轴的状态
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
	Commandhandler(_T("夹手到位"), error);
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