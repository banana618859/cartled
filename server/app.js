/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-19 10:39:30
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-25 19:07:52
 */
try {
// 获取启动线程的参数
var allParam = process.argv.slice(2);
var telnetPort = allParam[0]
var tcpPort = allParam[1]
var configFilePath = allParam[2]
console.error('线程参数process',process.argv,allParam,telnetPort,tcpPort,configFilePath);

const fs = require('fs');
const express = require('express')
const app = express()
const port = 8100;
const Util = require("./Util/index")
// console.error('Util',Util,Util.num2Binary(65534));
// 运行状态
var runType=true;

// 启动状态
var isStart=true;

// 间隔计时器
let timerId;

//解析参数，需要引入body-parser
var bodyParser = require('body-parser');
// app.use(express.static('public'));

// 参数解析
app.use(bodyParser.json()); // for parsing application/json
app.use(bodyParser.urlencoded({
    extended: true
})); // for parsing application/x-www-form-urlencoded

// 启动ws
const WebSocket = require('ws');
const wss = new WebSocket.Server({ port: 8000 });
wss.on('connection', function connection(ws) {
  console.error('connection',Date.now());
  ws.on('message', function incoming(message) {
    console.log('received: %s', message);
  });
  let obj={msg: 'conn success!'}
  ws.send(JSON.stringify(obj));
  global.wsObj = ws;
});

// 更新通知函数
function saveLogFun(obj){
  // 通知前端，重新拉取日志数据
  console.log('重新拉取:')
  wsRel.type = 'save';
  wsRel.data=obj
  global.wsObj.send(JSON.stringify(wsRel));
}
function sendToVisual(obj){
  return new Promise(async (resolve,reject)=>{
    setTimeout(()=>{
      reject('访问超时')
    },15000)
    try {
      let rel = await netClient.send(obj);
      resolve(rel)
    } catch (error) {
      reject(error)
    }
  })
}

// 引入dll
const dllObj= require('./dll/ffi.js');

// 引入net 为监听客户软件telnet信息
const netServer = require('./net_server');

// 引入net 为与视觉软件通信
const netClient = require('./net_client');
global.wsObj=null;

// 是否启动
global.isStart=false;

// 当前项目
var currentProject=null;
let wsRel={
  type: 'currentProject',
  data: currentProject
}
let gHttpRel={
  error: 0,
  msg: 'success',
  data: ''
}
let ngHttpRel={
  error: 1,
  msg: 'has error',
  data: ''
}

  //设置跨域访问
  app.all('*', (req, res, next) => {
    res.header("Access-Control-Allow-Origin", "*");
    res.header("Access-Control-Allow-Headers", "X-Requested-With");
    res.header("Access-Control-Allow-Methods","PUT,POST,GET,DELETE,OPTIONS");
    res.header("X-Powered-By",' 3.2.1')
    res.header("Content-Type", "application/json;charset=utf-8");
    next();
  });

  app.post('/findFun', async (req, res) => {
    console.log('findFun:', req.body);
    try {
      let rel = await dbObj.findFun(req.body)
      gHttpRel.data = rel;
      // gHttpRel.count = rel.relObj;
      res.send(JSON.stringify(gHttpRel))
    } catch (error) {
      console.error('error-pf:',error);
      ngHttpRel.data = error;
      res.send(JSON.stringify(ngHttpRel))
    }
  })
  
  app.post('/commonFun', async (req, res) => {
    console.error('commonFun:', req.body);
    try {
      switch(req.body.data.type){
        case 'closeTcpClient':
          console.error('关闭net客户端');
          netClient.close()
          console.error('关闭net服务器');
          await netServer.close()
          console.error('net-客户端和服务器都关闭了');
          res.send(JSON.stringify(gHttpRel))
          break;
        // 打开 
        case 'start_status':
          isStart = req.body.data.status
          gHttpRel.data = isStart;
          res.send(JSON.stringify(gHttpRel))
          break;
        case 'change_runType':
          runType = req.body.data.status
          gHttpRel.data = runType;
          res.send(JSON.stringify(gHttpRel))
          break;
        case 'triggerVisual':
          // 触发视觉
          var data = req.body.data.data;
          console.error('视觉data:',data);
          try {
            // 发给视觉--这里可能连接不上视觉，所以要捕捉错误
            let param = {
              port: tcpPort,
              data: 'TEST'
            }
            // 手动触发
            let rel = await sendToVisual(param)
            console.error('rel',rel)
            saveLogFun({
              name: req.body.data.type,
              result: rel
            })
            gHttpRel.data = rel;
            res.send(JSON.stringify(gHttpRel))
          } catch (error) {
            console.error('err:',error);
            saveLogFun({
              name: 'server_error',
              result: '视觉软件连接错误，请检查！'
            })
            ngHttpRel.data = error;
            res.send(JSON.stringify(ngHttpRel))
          }
          break;
        default:
          gHttpRel.data = 'Method not found!';
          res.send(JSON.stringify(gHttpRel))
        break;
      }
      
    } catch (error) {
      console.error('error-pf:',error);
      ngHttpRel.data = error;
      res.send(JSON.stringify(ngHttpRel))
    }
  })

  app.post('/dllFun', async (req, res) => {
    console.error('dllFun:', req.body);
    try {
      var rel;
      switch(req.body.data.type){
        // 自动初始化
        case 'init':
          rel = dllObj.init();
          gHttpRel.data = rel;
          saveLogFun({
            name: req.body.data.type,
            result: rel
          })
          res.send(JSON.stringify(gHttpRel))
          break;
        // 查轴状态
        case 'getAxisStatus':
          rel = dllObj.axis_GetSts(req.body.data);
          let finalArr= Util.num2Binary(rel.data);
          console.error('finalArr:',finalArr);
          // 1,5,6,8位为1时，触发急停
          if(finalArr[1] && finalArr[5] && finalArr[6] && finalArr[8]){
            console.error('调急停');
            let rel_stop = dllObj.LTE_Stop();
            console.error('急停结果rel_stop',rel_stop);
            gHttpRel.msg = '已调急停';
            gHttpRel.rel_stop = rel_stop;
          }
          gHttpRel.data = rel;
          res.send(JSON.stringify(gHttpRel))
          break;
        // 查轴位置 getAxisPos
        case 'getAxisPos':
          rel = dllObj.getAxisPos(req.body.data);
          gHttpRel.data = rel;
          res.send(JSON.stringify(gHttpRel))
          break;
        // 复位和回原
        case 'goBack':
          rel = await dllObj.goBack(req.body.data);
          gHttpRel.data = rel
          saveLogFun({
            name: req.body.data.type,
            result: rel
          })
          res.send(JSON.stringify(gHttpRel))
          break;
        // 复位和回原
        case 'move':
          rel = dllObj.move(req.body.data);
          gHttpRel.data = rel
          saveLogFun({
            name: req.body.data.type,
            result: rel
          })
          res.send(JSON.stringify(gHttpRel))
          break;
        // 查Di
        case 'getDi':
          // 查轴前限 0
          var frontLimit = dllObj.getDi(0);
          // 查轴后限 1
          var endLimit = dllObj.getDi(1);
          // 查轴原点 3
          var origin = dllObj.getDi(3);
          // 16个GPI 4
          var DiLamp = dllObj.getDi(4);
          gHttpRel.data = {
            frontLimit,
            endLimit,
            origin,
            DiLamp
          };
          res.send(JSON.stringify(gHttpRel))
          break;
        // 查Do
        case 'getDo':
          rel = dllObj.getDo();
          gHttpRel.data = rel;
          res.send(JSON.stringify(gHttpRel))
          break;
        // 设置Do
        case 'setDo':
          rel = dllObj.setDo(req.body.data);
          gHttpRel.data = 'setDo success';
          saveLogFun({
            name: req.body.data.type,
            result: rel
          })
          res.send(JSON.stringify(gHttpRel))
          break;
        // 设置左夹手
        case 'biteFun':
          rel = dllObj.biteFun(req.body.data);
          gHttpRel.data = rel;
          saveLogFun({
            name: req.body.data.type,
            result: rel
          })
          res.send(JSON.stringify(gHttpRel))
          break;
        // 查Do
        case 'getDo':
          rel = dllObj.getDo();
          gHttpRel.data = rel;
          saveLogFun({
            name: req.body.data.type,
            result: rel
          })
          res.send(JSON.stringify(gHttpRel))
          break;
        // 启动
        case 'start':
          global.isStart = true;
          gHttpRel.data = 'Start success!';
          saveLogFun({
            name: req.body.data.type,
            result: rel
          })
          res.send(JSON.stringify(gHttpRel))
          break;
        // 暂停
        case 'stop':
          global.isStart = false;
          gHttpRel.data = 'Stop success!';
          saveLogFun({
            name: req.body.data.type,
            result: rel
          })
          res.send(JSON.stringify(gHttpRel))
          break;
        // 方向盘动
        case 'directionMove':
          console.error('req.body.data',req.body.data)
          rel = await dllObj.directionMove(req.body.data);
          gHttpRel.data = rel;
           saveLogFun({
            name: req.body.data.type,
            result: rel
          })
          res.send(JSON.stringify(gHttpRel))
          break;
        // 方向盘停
        case 'directionStop':
          console.error('directionStop-app:', req.body);
          rel = await dllObj.directionStop(req.body.data);
          gHttpRel.data = rel;
          saveLogFun({
            name: req.body.data.type,
            result: rel
          })
          res.send(JSON.stringify(gHttpRel))
          break;
        default:
          gHttpRel.data = 'Dll not found!';
          res.send(JSON.stringify(gHttpRel))
          break;
      }
      
    } catch (error) {
      console.error('error-pf:',error);
      ngHttpRel.data = error;
      res.send(JSON.stringify(ngHttpRel))
    }
  })

  // 获取所有项目配置
  app.get('/getAllProject', (req, res) => {
    try {
      console.error('configFilePath',configFilePath);
      const allFile = JSON.parse(fs.readFileSync(configFilePath).toString());
      gHttpRel.data = allFile
      res.send(JSON.stringify(gHttpRel))
    } catch (error) {
      console.error('getAllProject:',error);
    }
  })
  app.get('/', (req, res) => {
    res.send('hello world')
  })

  async function cbFun(data){
    console.error('telnet_client_data',data);
    if(runType){
      // 将收到的指令进行合法性检验
      let validateRel = cmdValidateFun(data);

      if(!validateRel){
        
        // 验证不合法，直接回复
        let rel='Incorrect command.'
        // 1、调用net服务器，返回数据给客户
        netServer.sendBack(rel)
        saveLogFun({
          name: 'telnet',
          result: rel
        })
      }else{
        // 验证通过

        // 声明层
        validateRel.levelName = validateRel.cmdArr[2];

        // 判断是否切换层--切换产品
        if (
          currentProject &&
          currentProject.levelName !== validateRel.levelName
        ) {
          console.error( "切换层：",currentProject.levelName, validateRel.levelName);
          // 如果发生切换层，要先平移到对应位置，才能发触发视觉
        }

        currentProject = validateRel;
        // 不管是否换层，都分别平移X轴，和Z轴后，再调视觉
        // 1表示X轴，2表示Z轴
        // 平移X轴
        var mObj={
          axis: 1,
          vel: currentProject.MAN_Vel,
          pos: currentProject.X_POS_dut1,
          check: true,
          acc: currentProject.MAN_Acc,
          dec: currentProject.MAN_Dec
        }
        console.error('移动X轴--这里居然是同步的-正合我意');
        // dllObj.move(mObj)

        // 平移Z轴
        //（4-1）*间隔+初始位置：目标位置参数
        let pos = (currentProject.levelName - 1) 
          * currentProject.Z_POS_gap + currentProject.Z_POS_dut1;
        mObj={
          axis: 2,
          vel: currentProject.MAN_Vel,
          pos,
          check: true,
          acc: currentProject.MAN_Acc,
          dec: currentProject.MAN_Dec
        }
        console.error('移动Z轴--这里居然是同步的-正合我意');
        // dllObj.move(mObj)
        
        // 1、启动参数发给前端
        wsRel.type = 'currentProject';
        wsRel.data=currentProject
        global.wsObj.send(JSON.stringify(wsRel));
        
        try {
          // 2、发给视觉--这里可能连接不上视觉，所以要捕捉错误
          // let rel = await netClient.send()
          let param = {
            port: tcpPort,
            data: 'TEST'
          }
          // 自动触发-客户触发
          console.error('触发视觉');
          let rel = await sendToVisual(param)
          console.log('rel',rel)

          // 3、调用net服务器，返回数据给客户
          netServer.sendBack(rel)
          // 4.不保存数据库了，改为文件存储操作日志
          saveLogFun({
            name: 'triggerVisual',
            result: rel
          })
        } catch (error) {
          // 1、调用net服务器，返回数据给客户
          netServer.sendBack('Vision software not open!')
          // 2、通知前端，保存日志数据
          saveLogFun({
            name: 'server_error',
            result: error
          })
        }
      }

    }else{
      // 调试模式
      let rel = 'Debug mode, no response to telnet access!'
      netServer.sendBack(rel)
    }
  }

  function cmdValidateFun(data){
    // 根据下划线将指令截取
    let arr = data.split('_');
    // 检查第一个项目名称是否存在
    // 读取配置文件内容
    let proArr = JSON.parse(fs.readFileSync(configFilePath).toString());
    // console.error('proArr',arr,proArr);

    let hasOne=false;
    for(let i = 0; i < proArr.length; i++){
      if(proArr[i].projectName === arr[0]){
        hasOne = proArr[i];
        hasOne['cmdArr'] = arr
        break;
      }
    }
    return hasOne;
  }

  app.listen(port, async () => {
    console.error(`${Date.now()}__Example app listening at http://localhost:${port}`);

    // 启动net服务器-为监听telnet指令
    netServer.conn(telnetPort, cbFun);
    
  })
} catch (error) {
  console.error('server_error:',error);
  // 当出现错误时 
  // 126可能是dll找不到，127可能是dll的调用方法、参数不对
}