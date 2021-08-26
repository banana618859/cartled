/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-19 20:14:18
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-23 15:39:59
 */
// c语言指针类型转化 指导链接 https://www.jianshu.com/p/dd9463dead8c

// 打印ffi的类型 好像有个object类型的？
/**
  void: <Buffer@0x00007FFAF6202CF8 name: 'void'>,    
  uint8: <Buffer@0x00007FFAF6202D10 name: 'uint8'>,  
  int8: <Buffer@0x00007FFAF6202D28 name: 'int8'>,    
  uint16: <Buffer@0x00007FFAF6202D40 name: 'uint16'>,
  int16: <Buffer@0x00007FFAF6202D58 name: 'int16'>,  
  uint32: <Buffer@0x00007FFAF6202D70 name: 'uint32'>,
  int32: <Buffer@0x00007FFAF6202D88 name: 'int32'>,  
  uint64: <Buffer@0x00007FFAF6202DA0 name: 'uint64'>,
  int64: <Buffer@0x00007FFAF6202DB8 name: 'int64'>,
  uchar: <Buffer@0x00007FFAF6202D10 name: 'uchar'>,
  char: <Buffer@0x00007FFAF6202D28 name: 'char'>,
  ushort: <Buffer@0x00007FFAF6202D40 name: 'ushort'>,
  short: <Buffer@0x00007FFAF6202D58 name: 'short'>,
  uint: <Buffer@0x00007FFAF6202D70 name: 'uint'>,
  int: <Buffer@0x00007FFAF6202D88 name: 'int'>,
  float: <Buffer@0x00007FFAF6202DE8 name: 'float'>,
  double: <Buffer@0x00007FFAF6202E00 name: 'double'>,
  pointer: <Buffer@0x00007FFAF6202DD0 name: 'pointer'>,
  ulonglong: <Buffer@0x00007FFAF6202D70 name: 'ulonglong'>,
  longlong: <Buffer@0x00007FFAF6202D88 name: 'longlong'>
 */

var ffi = require('ffi-napi');
var ref = require('ref-napi')
// console.error('ref',ref.types);
var fs = require('fs');
var path = require('path');
var filepath0 = path.resolve(__dirname, './aa.dll')
var gts = path.resolve(__dirname, './gts.dll')
var filepath = path.join(__dirname, './LED_GT_MOTION.dll')

console.error('filepath',filepath, fs.existsSync(filepath));

var libm = ffi.Library(filepath, {
  // 函数名 返回类型 传入参数类型2个参数
  LTE_init: [ 'int', [] ],  // 初始化
  LTE_close: [ 'void', [] ], // 关闭软件
  LTE_Stop: [ 'void', [] ], // 急停

  // DIO
  LTE_GetDI: ['long', ['short']], // 间隔100ms
  LTE_GetDO: ['long', []], // 间隔1s
  // 设置DO 参数：第几个？，状态关/开？
  LTE_SetDO_bit: ['void', ['short','short', 'short']],

  // 运动
  //gohome:short axis, double vel, double acc = 0.25, double dec = 0.25, long offset = OFFSET_N
  LTE_GoHome: ['int', ['short','double','double','double','long']], // 回原
  // ?移动 轴名称，移动速度，手动目标位置，？，加速度
  // short axis, double vel, long pos, bool P_N_CHECK = true, double acc = 1, double dec = 1
  LTE_PointMove: ['int', ['short','double','long', 'bool','double', 'double']],
  // 方向盘移动 参数：轴名称，移动速度，方向正/反
  // short axis, double vel, bool direction = true, double acc = 0.1, double dec = 0.1
  LTE_Jog: ['void', ['short','double','bool','double','double']],

  // 单轴停 void LTE_Axis_Stop(short axis);
  LTE_Axis_Stop: ['void', ['short']],

  // 夹手按钮 参数：轴名称 3？，方向 true/false？  
  // short axis, bool direction = true, double vel = 2, long distance_limit
  LTE_CLAMP: ['int', ['short','bool','double','long']],

  // 查询轴状态 轮询 不间断查间隔100ms 参数：轴名称: x/z? 
  // long LTE_Axis_GetSts(short axis);
  LTE_Axis_GetSts: ['long', ['short']],
  // 读取当前位置 查X、Z轴 long LTE_ReadEncPos(short axis);
  LTE_ReadEncPos: ['long', ['short']],
  // 急停 void LTE_Stop()
  LTE_Stop: ['void', []],
});

// var libm = ffi.Library(gts, {
//   // 函数名 返回类型 传入参数类型2个参数
//   GT_Open:  ['short', []],
//   GT_Reset: ['short', []]
// });
// console.log(libm.GT_Open())

var dllObj={
  // 初始化 
  init(){
    let rel = libm.LTE_init();
    console.error('rel-init:',rel);
    return rel;
  },
  // 复位 和 回原
  goBack({axis,vel, acc,dec, offset}){
    console.error('goBack-dll:',axis,vel);
    let rel = libm.LTE_GoHome(axis,vel, acc,dec, offset);
    console.error('rel-goBack:',rel);
    return rel;
  },
  // 移动
  move({ axis,vel,pos,check,acc,dec }){
    let rel = libm.LTE_PointMove(axis,vel,pos,check,acc,dec);
    console.error('rel-move:',rel);
    return rel;
  },
  // 关闭
  close(){
    let rel = libm.LTE_close();
    console.error('rel-gtc:',rel);
    return rel;
  }, 
  // 左夹手
  biteFun({axis, status, vel, distance_limit}){
    console.error('bit:',axis, status, vel, distance_limit)
    let rel = libm.LTE_CLAMP(axis, status, vel, distance_limit);
    console.error('rel-clamp:',rel);      
    return rel;
  },
  // 方向盘动
  directionMove({axis,vel=10,direction=true,acc=0.1,dec=0.1}){
    let rel = libm.LTE_Jog(axis,vel,direction,acc,dec);
    console.error('rel-directionMove:',rel);
    return rel;
  },
  // 方向盘停
  directionStop({axis}){
    let rel = libm.LTE_Axis_Stop(axis);
    console.error('rel-directionStop:',rel);
    return rel;
  },
  // ditype：2轴正前限(0)，2轴负后限(1)，2轴原点(3), 16个GPI(4)
  getDi(diType=4){
    let rel = libm.LTE_GetDI(diType);
    console.error('rel-getDi:',rel);
    return rel;
  },
  getDo(){
    let rel = libm.LTE_GetDO();
    console.error('relgetDo:',rel);
    return rel;
  },
  setDo({index, status}){
    console.log('setDo:',index, status)
    let rel = libm.LTE_SetDO_bit(index, status,12);
    console.error('rel-setDo:',rel);
    return rel;
  },
  axis_GetSts({axis}){
    console.log('axis_GetSts:',axis)
    let rel = libm.LTE_Axis_GetSts(axis);
    console.error('rel-axis_GetSts:',rel);
    return rel;
  },
  LTE_Stop(){
    console.log('LTE_Stop:')
    let rel = libm.LTE_Stop();
    console.error('rel-LTE_Stop:',rel);
    return rel;
  },
  getAxisPos({axis}){
    console.log('axis_pos:',axis)
    let rel = libm.LTE_ReadEncPos(axis);
    console.error('rel-pos:',rel);
    return rel;
  },
}
module.exports = dllObj;

// You can also access just functions in the current process by passing a null
// var current = ffi.Library(null, {
//   'atoi': [ 'int', [ 'string' ] ]
// });
// current.atoi('1234'); // 1234