<!--
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-19 10:11:04
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-25 19:25:03
-->
<template>
  <div
    style="height: 100%"
    class="flexBox-c"
  >
    <div id="header" class="header border_ddd posr text-l">
      <h2 class="center_box">中控系统</h2>
      <div class="fr">
        <el-tooltip class="item" effect="dark" content="系统配置" placement="left">
          <i class="el-icon-setting setIcon hand" @click="getConf"></i>
        </el-tooltip>
        
        <el-button-group>
          <el-button @click="init" type="primary">初始化</el-button>
          <el-button
            @click="changeRunType(true)"
            :type="runType ? 'success' : ''"
            >自动运行</el-button
          >
          <el-button
            @click="changeRunType(false)"
            :type="runType ? '' : 'success'"
            >调试</el-button>
        </el-button-group>
      </div>

      <el-tooltip
        class="item"
        effect="dark"
        :disabled="!runType"
        content="自动运行模式不能选择项目"
        placement="right"
      >
        <el-select
          v-model="projectName"
          :disabled="runType"
          placeholder="请选择项目"
          @change="projectChange(projectName)"
        >
          <template
            v-if="allProject && allProject.length && allProject[0].projectName"
          >
            <el-option
              v-for="(item, index) in allProject"
              :key="item.projectName + index"
              :label="item.projectName"
              :value="item.projectName"
            >
            </el-option>
          </template>
        </el-select>
      </el-tooltip>
    </div>
    <div
      class="main border_ddd flex1 flexBox-r over_auto"
      :style="{ height: mainH + 'px' }"
    >
      <div class="leftBox">
        <product-show
          :productData="productData"
          :isStart="isStart"
          @showImg="showImg"
          @start="startFun"
          @reset="reset"
          :runType="runType"
          :DiArr="DiArr"
        ></product-show>
      </div>

      <!-- 中间部分 -->
      <div class="middle text-l flexBox-c posr">
        <div
          style="
            background-color: #eee;
            padding: 0 5px;
            height: 40px;
            line-height: 40px;
          ">
          <span class="font_16">操作&通信记录</span>
        </div>
        <div class="flex1 over_auto_y" style="padding: 0 10px">
          <ul class="recodeUl">
            <li v-for="(item,index) in tableData" :key="item+index">
              <i v-if="item.includes('\\r\\n')" @click="lookRecord(item)" class="el-icon-view lookIcon"></i>
              {{item}}
            </li>
          </ul>
        </div>
      </div>
      <!-- <div
        class="middle over_auto img_cover posr flexBox-r vCenter"
        :style="{ backgroundImage: `url(${currentImg})` }"
      >
        <div v-if="currentTest && !currentImg">
          <p><img :src="tempImg" alt="图片不存在提示" /></p>
          <p class="mg-t10">图片不存在</p>
        </div>
        <div v-show="!currentTest" class="vCenter mImg">图片显示区</div>
      </div> -->

      <div class="rightBox posr">
        <div
          v-if="runType || !currentProject"
          style="
            width: 100%;
            height: 100%;
            background-color: #000;
            opacity: 0.15;
            cursor: not-allowed;
            position: absolute;
            top: 0;
            left: 0;
            z-index: 1;
          "
        ></div>
        <setting-box
          v-loading="showLoad"
          :element-loading-text="loadText"
          :currentProject="currentProject"
          :DoArr="DoArr"
          :DiArr="DiArr"
          :frontArr="frontArr"
          :endArr="endArr"
          :originArr="originArr"
          :xPos="xPos"
          :zPos="zPos"
          :visualWait="visualWait"
          @setFun="setFun"
          @rightBite="rightBite"
          @leftBite="leftBite"
          @directionMove="directionMove"
          @directionStop="directionStop"
          @setDo="setDo"
        ></setting-box>
      </div>
    </div>

    <!-- 底部信息展示 -->
    <div
      class="footer over_auto img_cover posr flexBox-r vCenter"
      :style="{ height: footH + 'px', backgroundImage: `url(${currentImg})` }">
      <div class="topLine" id="topLine"></div>
      <div v-if="currentTest && !currentImg">
        <p><img :src="tempImg" alt="图片不存在提示" /></p>
        <p class="mg-t10">图片不存在</p>
      </div>
      <div v-show="!currentTest" class="vCenter mImg">图片显示区</div>
    </div>
    

    <el-dialog
      title="系统配置"
      class="myDialog"
      :visible.sync="showSetting"
      :show-close="false"
      :close-on-click-modal="false"
      :before-close="handleClose"
      width="50%">
      <div class="smallBox">
        <el-form 
          :model="setObj" 
          :rules="rules" 
          size="mini"
          ref="ruleForm" label-width="140px" class="demo-ruleForm">
          <el-form-item label="telnet服务器端口" prop="telnetPort">
            <el-input v-model.number="setObj.telnetPort" placeholder="8600"></el-input>
          </el-form-item>
          <el-form-item label="tcp服务器端口" prop="tcpPort">
            <el-input v-model.number="setObj.tcpPort" placeholder="7600"></el-input>
          </el-form-item>
          <el-form-item label="操作记录保存路径" prop="logPath">
            <el-input v-model="setObj.logPath">
              <el-button @click="selectPath" slot="append" icon="el-icon-folder-opened">
              </el-button>
            </el-input>
          </el-form-item>
          <el-form-item label="项目配置文件" prop="projectPath">
            <el-input v-model="setObj.projectPath">
              <el-button @click="selectPathPro" slot="append" icon="el-icon-folder-opened">
              </el-button>
            </el-input>
          </el-form-item>
        </el-form>
      </div>
      <span slot="footer" class="dialog-footer">
        <el-button v-if="canCancel" size="mini" @click="showSetting = false">取 消</el-button>
        <el-button size="mini" type="primary" @click="sureSet('ruleForm')">保 存</el-button>
      </span>
    </el-dialog>

    <el-dialog
      title="详情"
      class="myDialog notTip"
      :visible.sync="showRecord"
      :show-close="true"
      top="20px"
      width="80%">
      <div class="smallBox" style="height: 85vh; overflow: auto;">
        <el-form 
          v-if="recordObj"
          :model="recordObj"
          size="mini"
          ref="ruleForm" label-width="140px" class="demo-ruleForm">
          <el-form-item label="日期">
            {{recordObj.date}}
          </el-form-item>
          <el-form-item label="名称">
            {{recordObj.name}}
          </el-form-item>
          <el-form-item label="结果">
            <ul>
              <li v-for="(item,index) in recordObj.result" :key="item+index">
                {{item}}
              </li>
            </ul>
          </el-form-item>
        </el-form>
      </div>
    </el-dialog>

    <!-- input文件框，加multiple属性后可以上传多文件 nwdirectory表示选择路径-->
    <form id="importFunForm" style="height: 0">
      <input
        class="el-upload__input"
        type="file"
        nwdirectory
        id="importFunIpt"
        @change="selectPathChange"
      />
    </form>
    <form id="fileForm" style="height: 0">
      <input
        class="el-upload__input"
        type="file"
        id="fileFormIpt"
        @change="selectPathChange"
      />
    </form>
  </div>
</template>

<script>
const fs = nw.require("fs");
const path = nw.require("path");
import productShow from "./productShow";
import settingBox from "./settingBox";
import Util from "../Util/index";
export default {
  name: "HelloWorld",
  components: {
    productShow,
    settingBox,
  },
  data() {
    return {
      server_process_pid: 0,
      showRecord: false,
      recordObj: {
        date: '',
        name: '',
        result: ''
      },
      visualWait: false,
      canCancel: false,
      startPath: '',
      configPath: '',
      isLogPath: true,
      setObj: {
        telnetPort: 8600,
        tcpPort: 7600,
        logPath: '',
        projectPath: ''
      },
      rules: {
        telnetPort: [
          { required: true, type: 'number', message: '请输入数字', trigger: 'blur' },
        ],
        tcpPort: [
          { required: true, type: 'number', message: '请输入数字', trigger: 'blur' },
        ],
        logPath: [
          { required: true,  message: '请输入路径', trigger: 'change' },
        ],
        projectPath: [
          { required: true,  message: '请输入路径', trigger: 'change' },
        ],
      },
      showSetting: false,
      isHurryStop: false, // 急停标志
      showLoad: false,
      loadText: "正在运动中...",
      xPos: 0,
      zPos: 0,
      isStart: false,
      DoArr: [],
      DiArr: [],
      frontArr: [0, 0, 0, 0, 0],
      endArr: [0 ,0, 0, 0, 0],
      originArr: [0, 0, 0, 0, 0],
      mainH: 500,
      footH: 250,
      total: 0,
      tableData: [],
      projectData: null,
      msg: "Welcome to Your Vue.js App",
      currentTest: null,
      tempImg: require("../assets/imgError.png"),
      currentImg: "",
      serverIsExist: false,
      projectName: "",
      currentProject: null,
      runType: true,
      productData: [],
      allProject: [],
      dateValue: [],
    };
  },
  async mounted() {
    // 更新一次
    // this.updateDo()
    // this.updateDi()

    let rel = await this.validateConf();
    if(!rel){
      console.error('配置文件不完整！');
      return;
    }

    // 初始化，包括启动后台
    this.initFun()

    // 底部拖拽 
    this.dragFun()
  },
  methods: {
    dragFun(){
      let that = this;
      let winH = document.documentElement.clientHeight;
      this.mainH = winH - this.footH;
      let topLine = document.getElementById("topLine");
      console.log("topLine", topLine);
      topLine.onmousedown = () => {
        console.error("event", event);
        // let firstX = event.clientX;
        let firstY = event.clientY;

        // 移动 document范围内的移动鼠标
        document.onmousemove = () => {
          // console.error('move:',event);
          let changeY = firstY - event.clientY;
          // console.error('changeY:',changeY);
          that.footH += changeY;
          that.mainH = winH - that.footH;
          // 更新起点
          firstY = event.clientY;
        };
        // 弹起
        document.onmouseup = () => {
          console.error("鼠标弹起");
          document.onmousemove = null;
        };
      };
    },
    lookRecord(item){
      let huaIndex = item.indexOf('{')
      console.error('huaIndex',huaIndex);
      let date = item.slice(0,huaIndex-2)
      let obj = item.slice(huaIndex)
      obj = JSON.parse(obj)
      let arr = obj.result.split('\r\n');
      this.recordObj.date = date;
      this.recordObj.name = obj.name;
      this.recordObj.result = arr;
      this.showRecord = true;
    },
    validateConf(){
      let notError = true; 
      console.error('process.env.NODE_ENV',process.env.NODE_ENV);
      console.error("process", nw.process.execPath);
      let thePath = nw.process.execPath;
      let lastOne;
      if(thePath.includes('\\node_modules')){
        lastOne = thePath.lastIndexOf("\\node_modules");
      }else{
        lastOne = thePath.lastIndexOf("\\");
      }
      let startPath = thePath.slice(0, lastOne);
      this.startPath = startPath;
      this.configPath = path.join(startPath,'sysConfig.json')
      console.error("startPath", lastOne, startPath);
      if(!fs.existsSync(this.configPath)){
        this.showSetting = true;
        this.canCancel = false;
        notError = false;
      }else{
        // 如果文件不完整，也要重新配置
        let obj = JSON.parse(fs.readFileSync(this.configPath).toString())
        this.setObj = obj;
        let flag=false;
        for(let key in obj){
          if(!obj[key]){
            flag = true;
            break;
          }
        }
        if(flag){
          this.showSetting = true;
          this.canCancel = false;
          notError = false;
        }else{
          this.canCancel = true;
          notError = true;
        }
        return notError;
      }
    },
    handleClose(){
      console.error('close1');
      this.sureSet('ruleForm')
    },
    closeTcpClient(){
      return new Promise(async (resolve,reject)=>{
        try {
          let rel = await this.postData({
            url: "/commonFun",
            allData: {
              data: {
                type: "closeTcpClient"
              },
            },
          });
          resolve(rel)
        } catch (error) {
          reject(error)
        }
      })
    },
    async initFun(){
      let that = this;

      // 如果后台线程存在，就断开后台线程，再重启
      console.error('server_process_pid:',this.server_process_pid);
      if(this.server_process_pid){
        // 1.通知后台关闭tcp客户端-- h
        // try {
        //   let tcpRel = await this.closeTcpClient()
        //   console.error('tcpRel',tcpRel);
        // } catch (error) {
        //   console.error('tcp客户端关闭异常',error);
        //   that.isHurryStop = true;
        //   // 不再往下跑
        //   return;
        // }

        // 2.客户端断开ws连接
        if(window.ws){
          window.ws.close()
        }

        // 3.后台线程存在，先断开
        var kill;
        console.error('后台线程存在，先断开');
        if(process.env.NODE_ENV === "development"){
          console.error('开发环境');
          kill = nw.require('tree-kill');
        }else{
          let thePath = path.join(this.startPath,'server', 'node_modules', 'tree-kill')
          kill = nw.require(thePath);
          console.error('生产环境:', thePath);
        }
        
        kill(this.server_process_pid);
        await this.sleep(3000)
      }
      
      let cbFun = () => {
        console.error("回调回来--");
        // 初始化
        this.init();
      };
      
      // 4.重启
      this.startServer(cbFun);
      this.startWs();
      that.isHurryStop = false;
      

      // 轮询函数
      async function loopRun_getDi() {
        try {
          let rel = await that.postData({
            url: "/dllFun",
            allData: {
              data: {
                type: "getDi",
              },
            },
          });
          console.error("loopRun_getDi-rel", rel);
          // 更新Di16个灯
          let one = 0;
          if(rel.data.data.DiLamp > one){
            one = rel.data.data.DiLamp
          }
          that.updateDi(one);

          // 更新前限 数字->数组 [x轴，z轴]
          that.updateFront(rel.data.data.frontLimit);
          // 更新后限 数字->数组 [x轴，z轴]
          that.updateEnd(rel.data.data.endLimit);
          // 更新原点 数字->数组 [x轴，z轴]
          that.updateOrigin(rel.data.data.origin);

          // 查询轴状态 --- 这个导致急停，诱发其他错误，要细查
          // that.getAxisStatus();

          // 一次完成，马上开始下一次
          await that.sleep(2000);
          if(!that.isHurryStop){
            loopRun_getDi();
          }
        } catch (error) {
          // 如果调用失败，就不开始了
          console.error("loopRun-err:", error);
        }
      }
      loopRun_getDi();

      async function loopRun_getDo() {
        try {
          let rel = await that.postData({
            url: "/dllFun",
            allData: {
              data: {
                type: "getDo",
              },
            },
          });
          console.error("loopRun_getDo-rel", rel.data.data);

          //  轴位置查询
          let rel_x = await that.postData({
            url: "/dllFun",
            allData: {
              data: {
                type: "getAxisPos",
                axis: 1,
              },
            },
          });
          console.error("loopRun_x", rel_x.data.data);
          that.xPos = rel_x.data.data;

          let rel_z = await that.postData({
            url: "/dllFun",
            allData: {
              data: {
                type: "getAxisPos",
                axis: 2,
              },
            },
          });
          console.error("loopRun_z", rel_z.data.data);
          that.zPos = rel_z.data.data;

          let one = 0;
          if(rel.data.data > one){
            one = rel.data.data
          }
          that.updateDo(one);
          // 一次完成，马上开始下一次
          await that.sleep(1000);
          if(!that.isHurryStop){
            loopRun_getDo();
          }
          
        } catch (error) {
          // 如果调用失败，就不开始了
          console.error("loopRun-err:", error);
        }
      }
      loopRun_getDo();
    },
    getConf(){
      this.showSetting = true;
      this.validateConf()
    },
    selectPath(){
      this.isLogPath= true
      // 清空上次选中
      document.getElementById('importFunForm') &&
        document.getElementById('importFunForm').reset();
      // 再打开
      document.getElementById('importFunIpt').click();
    },
    selectPathPro(){
      this.isLogPath= false
      // 清空上次选中
      document.getElementById('fileForm') &&
        document.getElementById('fileForm').reset();
      // 再打开
      document.getElementById('fileFormIpt').click();
    },
    async selectPathChange() {
      const theSelectPath = event.target.value;
      console.log('selectPathChange:', theSelectPath);
      
      if(this.isLogPath){
        this.setObj.logPath= theSelectPath;
      }else{
        let rel = this.validateConfigFile(theSelectPath)
        if(rel.error){
          this.$alert(`文件：${theSelectPath}不正确，\r\n${rel.msg}`,'错误',{
            type:'error'
          })
          return
        }
        this.setObj.projectPath= theSelectPath;
      }
    },
    validateConfigFile(filePath){
      // 根据路径，校验文件内容，是否符合要求
      let lastOne = filePath.lastIndexOf('.');
      let ext = filePath.slice(lastOne)
      if(ext !== '.json'){
        return{
          error: 1,
          msg: '配置文件必须为.json后缀，如projectConfig.json'
        }
      }
      let rel = JSON.parse(fs.readFileSync(filePath).toString());
      
      if(!Array.isArray(rel)){
        return{
          error: 1,
          msg: '配置文件内容必须为数组，如[{projectName:"abc"}]'
        }
      }else if(rel.length<1){
        return{
          error: 1,
          msg: '配置文件内容不能为空，建议[{projectName:"abc"}]'
        }
      }else{
        for(let i = 0; i < rel.length; i++){
          if(!rel[i]['projectName']){
            return{
              error: 1,
              msg: `第[${i+1}]个配置项格式不正确，至少包含{projectName:"abc"}`
            }
          }
        }
        return{
          error: 0,
          msg: 'success'
        }
      }
    },
    sureSet(formName) {
      console.error('set:',this.setObj);
      this.$refs[formName].validate((valid) => {
        if (valid) {
          this.showSetting = false;
          this.canCancel=true;
          // 将配置内容保存到当前目录的文件下
          fs.writeFileSync(this.configPath, JSON.stringify(this.setObj,null,2))
          this.myMsg('保存成功！')
          // 配置完成后，重启项目
          this.initFun();
        } else {
          console.log('error submit!!');
          this.canCancel=false;
          return false;
        }
      });
    },
    async init() {
      console.error('init');
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data: {
            type: "init",
          },
        },
      });
      // 只需要设置即可，轮询会更新结果
      console.error("初始化：", rel);
      
      // 初始化完成后，改变急停状态
      this.isHurryStop = false;
    },
    async getAxisStatus() {
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data: {
            type: "getAxisStatus",
          },
        },
      });
      // 根据查询回来的结果，判断是否要调急停
      console.error("getAxisStatus", rel);
      // 一旦调了急停，需要重新初始化才能继续使用
      if(rel.data.rel_stop){
        this.$alert('一旦调了急停，需要重新点击初始化才能继续使用');
        // 急停之后，停止轮询（等初始化之后再开始）
        this.isHurryStop = true;
      }
    },
    
    async setDo(index, status) {
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data: {
            type: "setDo",
            index,
            status,
          },
        },
      });
      // 只需要设置即可，轮询会更新结果
      console.error("setDo-rel:", rel);
    },
    // 左夹手切换
    async leftBite(bitStatus) {
      let  status = true;
      if(bitStatus){
        status = false;
      }
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data: {
            type: "biteFun",
            axis: 3,
            status: status,
            vel: this.currentProject.CLAMP_L_Vel,
            distance_limit: this.currentProject.CLAMP_L_limit
          },
        },
      });
      // 只需要设置即可，轮询会更新结果
      console.error("leftBite", rel);
    },
    // 右夹手切换
    async rightBite(bitStatus) {
      let  status = true;
      if(bitStatus){
        status = false;
      }
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data: {
            type: "biteFun",
            axis: 4,
            status: status,
            vel: this.currentProject.CLAMP_R_Vel,
            distance_limit: this.currentProject.CLAMP_R_limit
          },
        },
      });
      // 只需要设置即可，轮询会更新结果
      console.error("leftBite", rel);
    },
    async directionMove(axis, direction) {
      // axis,vel,direction,acc,dec
      console.error("directionMove", axis, direction);
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data: {
            type: "directionMove",
            axis: axis === "X" ? 1 : 2,
            vel: this.currentProject.MAN_Vel,
            direction,
            acc: this.currentProject.MAN_Acc,
            dec: this.currentProject.MAN_Dec,
          },
        },
      });
      console.error("directionMove-rel", rel);
    },
    async directionStop(axis, direction) {
      console.error("directionStop", axis, direction);
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data: {
            type: "directionStop",
            axis: axis === "X" ? 1 : 2,
          },
        },
      });
      console.error("directionStop-rel", rel);
    },
    setFun(type, axis, pos) {
      console.error("type", type);
      switch (type) {
        case "triggerVisual":
          this.triggerVisual();
          break;
        case "goBack":
          this.goBack(axis);
          break;
        case "move":
          this.move(axis, pos);
          break;
      }
    },
    // 触发视觉
    async triggerVisual() {
      this.visualWait = true;
      console.error("triggerVisual被触发");
      let rel = await this.postData({
        url: "/commonFun",
        allData: {
          data: {
            type: "triggerVisual",
            data: "TEST",
          },
        },
      });
      console.error("triggerVisual-rel", rel);
      this.visualWait = false;
    },
    // 回原
    async goBack(axis) {
      this.showLoad = true;
      // axis,vel, acc,dec, offset
      let data = {
        type: "goBack",
        axis: axis === "X" ? 1 : 2,
        vel: this.currentProject.HOME_Vel,
        acc: this.currentProject.HOME_Acc,
        dec: this.currentProject.HOME_Dec,
        offset: this.currentProject.HOME_offset,
      }
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data,
        },
      });
      console.error("goBack-rel", rel);
      this.showLoad = false;
      // setTimeout(()=>{
      //   this.showLoad = false;
      // },15000)
    },
    // 移动:定点运动
    // axis,vel,pos,check,acc,dec
    async move(axis, pos) {
      if (this.runType) {
        // 后台调
        // // （4-1）*间隔+初始位置：目标位置参数
        // pos =
        //   (4 - 1) * this.currentProject.Z_POS_gap +
        //   this.currentProject.Z_POS_dut1;
      }
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data: {
            type: "move",
            axis: axis === "X" ? 1 : 2,
            vel: this.currentProject.MAN_Vel,
            pos,
            check: true,
            acc: this.currentProject.MAN_Acc,
            dec: this.currentProject.MAN_Dec,
          },
        },
      });
      console.error("move-rel", rel);
    },
    // 点击复位
    async reset() {
      // 复位为双轴，即用X和Z分别调一次回原函数goBack
      console.error("点击复位");
      // 通过后台调用dll复位
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data: {
            type: "reset",
          },
        },
      });
      console.error("rel-reset", rel);
      this.myMsg("复位成功:" + rel.data.data);
    },
    // 点击启动
    async startFun(status) {
      console.error("点击启动/暂停", status);
      this.isStart = status;
      // 同步后台
      let rel = await this.postData({
        url: "/commonFun",
        allData: {
          data: {
            type: "start_status",
            status,
          },
        },
      });
      console.error("getLog-rel", rel);
    },
    updateDo(num = 0) {
      let rel = Util.num2Binary(num, 5);
      console.error("updateDo-rel", rel);
      // 5个Do分别代表 
      let nameArr=['蜂鸣器','红灯','黄灯','绿灯','刹车']
      let newArr=[]

      for(let i = 0; i < rel.length; i++){
        newArr.push({
          name: nameArr[i],
          value: rel[i]
        })
      }
      this.DoArr = newArr;
    },
    updateDi(num = 32) {
      // console.error('16个灯：',num);
      let rel = Util.num2Binary(num, 16);
      console.error('updateDi--arr:',rel);
      // 第4、5位决定左夹手
      if(rel[4] && rel[5]){
        this.myMsg('左夹手全为1');
      }
      // 第6、7位决定右夹手
      if(rel[6] && rel[7]){
        this.myMsg('右夹手全为1');
      }
      // 16个Di分别代表 
      let nameArr = [
        '急停信号','复位信号','小车锁定信号','小车解锁信号','左电机原位信号','左电机到位信号',
        '右电机原位信号','右电机到位信号','左侧上门开关信号','左侧下门开关信号',
        '电柜上门开关信号','电柜下门开关信号','右侧上门开关信号','右侧下门开关信号','小车到位开关信号','空'
      ]
      let newArr=[]
      for(let i = 0; i < rel.length; i++){
        newArr.push({
          name: nameArr[i],
          value: rel[i]
        })
      }
      this.DiArr = newArr;
    },
    updateFront(num = 0) {
      if(num<0){
        num=0
      }
      // console.error('front',num);
      let rel = Util.num2Binary(num, 16);
      console.error('front-rel1',rel);
      this.frontArr = rel;
    },
    updateEnd(num = 0) {
      if(num<0){
        num=0
      }
      // console.error('End',num);
      let rel = Util.num2Binary(num, 16);
      // console.error('End-rel1',rel);
      this.endArr = rel;
    },
    updateOrigin(num = 0) {
      if(num<0){
        num=0
      }
      // console.error('Origin',num);
      let rel = Util.num2Binary(num, 16);
      // console.error('Origin-rel1',rel);
      this.originArr = rel;
    },
    formatterFun(row, column, cellVal, index) {
      return this.formatDate(cellVal);
    },
    projectChange(name) {
      console.error("pro_name:", name);
      let arr = this.allProject;
      for (let i = 0; i < arr.length; i++) {
        if (arr[i].projectName === name) {
          this.currentProject = arr[i];
          let data={
            name: '手动切换项目',
            result: name
          }
          this.writeLogAndShow(data)
          break;
        }
      }
    },
    async getAllProject() {
      console.error('前端--获取所有项目数据：getAllProject');
      try {
        let rel = await this.getData({
          url: "/getAllProject",
        });
        console.error("getAllProject-rel", rel);
        this.allProject = rel.data.data;
        return rel;
      } catch (error) {
        console.error("getData-error:", error);
        return error;
      }
    },
    showImg(oneTest) {
      this.currentTest = oneTest;
      this.myMsg('展示图片')
      console.error("showImg:", oneTest, process.env.NODE_ENV);
      let lastOne = oneTest.allDataArr.length - 1;

      let tempImg =
        "D:/pratdemo/nw_demo/vue_nw_node/sike-0726/src/assets/logo.png";

      // 判断图片是否存在 fs.existsSync(oneTest.allDataArr[lastOne])
      console.error("fs.existsSync(tempImg)", fs.existsSync(tempImg));
      if (fs.existsSync(tempImg)) {
        if (process.env.NODE_ENV === "development") {
          tempImg = "http://localhost:7000/icon.png";
        }
        this.currentImg = tempImg;
      } else {
        console.error("图片不存在：", oneTest.allDataArr[lastOne]);
        this.currentImg = "";
        // this.currentImg = nw.require(tempImg)
      }
    },
    async changeRunType(status) {
      if(!status){
        this.getAllProject()
      }
      this.runType = status;
      // 告诉后台，当前的运行模式
      console.error("changeRunType被触发");
      let rel = await this.postData({
        url: "/commonFun",
        allData: {
          data: {
            type: "change_runType",
            status,
          },
        },
      });
      console.error("run-rel", rel);
      let data={
        name: status ? '自动运行模式' : '调试模式',
        result: 0
      }
      this.writeLogAndShow(data)
    },
    startWs() {
      let that = this;
      // 打开一个 web socket  这里端口号和上面监听的需一致
      var ws = new WebSocket("ws://localhost:8000/");

      // Web Socket 已连接上，使用 send() 方法发送数据
      ws.onopen = function () {
        console.error("onopen", Date.now());
        // 这里用一个延时器模拟事件
        ws.send("客户端消息");
      };
      // 这里接受服务器端发过来的消息
      ws.onmessage = function (e) {
        console.error("onmessage", e.data);
        try {
          if (e.data) {
            let data = JSON.parse(e.data);
            that.dealMsg(data);
          } else {
            console.error("记录不存在", e);
          }
        } catch (error) {
          console.error("JSON.parse-报错：", error);
        }
      };
      ws.onerror = function (e) {
        console.error("onerror", e);
      };
      ws.onclose = function (e) {
        console.error("onclose", e);
      };
      console.error("ws", ws);
      window.ws = ws;
    },
    dealMsg({ type, data }) {
      switch (type) {
        case "currentProject":
          console.error("cur_pro:", data);
          this.currentProject = data;
          this.projectName = data.projectName;
          break;
        case "save":
          if (data) {
            // 处理本次测试结果
            console.error("save_data:", data);
            this.writeLogAndShow(data)

            if(data.name === 'server_error'){
              console.error("err_da:", data);
              this.$alert(data.result, "错误提示",{
                type: 'error'
              });
            }
          }
          break;
      }
    },
    writeLogAndShow(data){
      // 写入到文件中
      let logFilePath = this.getLogFilePath()
      let now = Util.format(Date.now())
      let str = JSON.stringify(data)
      let row = `${now}: ${str}`
      // 回显到页面上
      this.tableData.unshift(row)
      row = row+'\r\n'
      fs.appendFileSync(logFilePath, row)
      // 在页面显示
      this.dealResult(data.name,data.result);
    },
    getLogFilePath(){
      let today = Util.format(Date.now(), 'yyyy-MM-dd')
      let logFile = path.join(this.setObj.logPath, `${today}.log`);
      if(!fs.existsSync(logFile)){
        fs.writeFileSync(logFile,'')
      }
      return logFile;
    },
    dealResult(name,rel) {
      console.error("typeof data: ",name, typeof rel);
      if(typeof rel === "string" && rel.includes("\r\n")){
        let allDataArr = rel.split("\r\n");
        console.error("allDataArr", allDataArr);
        if (!allDataArr[allDataArr.length - 1]) {
          allDataArr.pop();
        }
        // 检查产品列表中，是否有当前产品的数据
        // 当前产品名称：第几个产品 this.currentProject.cmdArr[2]
        let proName = "1";
        if (
          this.runType &&
          this.currentProject &&
          this.currentProject.cmdArr[2]
        ) {
          proName = this.currentProject.cmdArr[2];
        }
        let hasOne = false;
        for (let i = 0; i < this.productData.length; i++) {
          if (this.productData[i].name === proName) {
            hasOne = this.productData[i];
            break;
          }
        }
        if (!hasOne) {
          // 如果尚不存在数据，马上创建
          let onePro = {
            name: proName,
            allTest: [],
          };
          let oneTest = {
            name: proName + "-1",
            allDataArr,
          };
          onePro.allTest.push(oneTest);
          this.productData.push(onePro);
        } else {
          // 存在产品
          let oneTest = {
            name: proName + "-" + (hasOne.allTest.length + 1),
            allDataArr,
          };
          hasOne.allTest.push(oneTest);
        }
      }
    },
    startServer(callback) {
      console.error("run server", nw);
      const that = this;
      // 如果不存在，再创建
      if (that.serverIsExist) {
        console.error("存在了212，不重复创建");
        return;
      }
      // 通过子线程跑后台
      const ch_pr = nw.require("child_process");
      const spawn = ch_pr.spawn;
      let serverPath = path.join(this.startPath, "server", "app.js");

      // node 线程
      let timer;
      // 创建子线程，并传递参数，参数有： [后台路径，telnet服务器端口，tcp服务器端口]
      const server_process = spawn("node", [serverPath, this.setObj.telnetPort, this.setObj.tcpPort, this.setObj.projectPath]);
      server_process.stdout.on("data", (data) => {
        // console.log(`node-stdout: ${data}`);
        that.serverIsExist = true;
        if (!timer) {
          timer = setTimeout(() => {
            callback();
            clearTimeout(timer);
          }, 500);
        }
      });

      server_process.stderr.on("data", (data) => {
        console.error(`node-stderr: ${data}`);
        if (typeof data === "object" && data.data) {
          data = data.data.toString();
        }
        if (data.includes("server_error")) {
          console.error("弹出错误提示");
          that.$alert(data, "错误提示", {
            customClass: "alertBox",
          });
        }
      });

      server_process.on("close", (code) => {
        console.log(`node-child process exited with code ${code}`);
        that.serverIsExist = false;
      });
      console.error('server_process',server_process);
      this.server_process_pid = server_process.pid;
    },
  },
};
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
.header {
  height: 55px;
  padding: 5px;
  background-color: #eee;
}
.over_auto_y{
  overflow: hidden;
  overflow-y: auto;
}
.recodeUl li{
  width: 100%;
  padding: 5px 10px;
  word-break: break-all;
}
.recodeUl li:nth-child(2n){
  background-color: #f3f3f3;
}

.leftBox {
  width: 30%;
  height: 100%;
  border: 1px solid #ddd;
}
.middle {
  flex: 1;
  height: 100%;
}
.rightBox {
  width: 385px;
  height: 100%;
  border: 1px solid #ddd;
}
.mImg {
  width: 100%;
  height: 100%;
  background-color: #f2f2f2;
  font-size: 30px;
}
.topLine {
  position: absolute;
  left: 0;
  top: -1px;
  width: 100%;
  height: 3px;
  background-color: #ddd;
  cursor: ns-resize;
}
.setIcon{
  font-size:28px; position:relative; top:5px; right:5px;
}
.lookIcon{
  cursor: pointer;
  color: #409eff;
  font-size: 18px;
}
</style>
