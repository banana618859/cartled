<!--
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-19 10:11:04
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-22 15:53:04
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
          <i class="el-icon-setting setIcon hand" @click="showSetting=true"></i>
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
      <div
        class="middle over_auto img_cover posr flexBox-r vCenter"
        :style="{ backgroundImage: `url(${currentImg})` }"
      >
        <div v-if="currentTest && !currentImg">
          <p><img :src="tempImg" alt="图片不存在提示" /></p>
          <p class="mg-t10">图片不存在</p>
        </div>
        <div v-show="!currentTest" class="vCenter mImg">图片显示区</div>
      </div>
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
    <div class="footer text-l flexBox-c posr" :style="{ height: footH + 'px' }">
      <div class="topLine" id="topLine"></div>
      <div
        style="
          background-color: #eee;
          padding: 0 5px;
          height: 40px;
          line-height: 40px;
        "
      >
        <span class="font_16">操作&通信记录</span>
        <span style="color:red;">(此处日志改成文件存储)</span>
        <!-- <div class="fr mg-t5">
          <el-date-picker
            v-model="dateValue"
            type="datetimerange"
            :picker-options="pickerOptions"
            range-separator="至"
            start-placeholder="开始日期"
            end-placeholder="结束日期"
            value-format="timestamp"
            size="mini"
            class="fl mg-r20"
            style="width: 340px"
            @change="getLatestLog"
          >
          </el-date-picker>
          <el-pagination
            class="fl"
            @size-change="handleSizeChange"
            @current-change="handleCurrentChange"
            :current-page="currentPage"
            :page-sizes="[10, 20, 50, 100]"
            :page-size="pageSize"
            layout="total, sizes, prev, pager, next"
            :total="total"
          >
          </el-pagination>
          
        </div> -->
      </div>
      <div class="flex1 over_auto" style="padding: 0 10px">
        <ul>
          <li v-for="(item,index) in tableData" :key="item+index">
            {{item}}
          </li>
        </ul>
      </div>
    </div>

    <el-dialog
      title="系统配置"
      class="myDialog"
      :visible.sync="showSetting"
      :show-close="false"
      :close-on-click-modal="false"
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
          <el-form-item label="项目文件路径" prop="projectPath">
            <el-input v-model="setObj.projectPath">
              <el-button @click="selectPathPro" slot="append" icon="el-icon-folder-opened">
              </el-button>
            </el-input>
          </el-form-item>
        </el-form>
      </div>
      <span slot="footer" class="dialog-footer">
        <el-button size="mini" type="primary" @click="sureSet('ruleForm')">确 定</el-button>
      </span>
    </el-dialog>

    <!-- input文件框，加multiple属性后可以上传多文件 -->
    <form id="importFunForm" style="height: 0">
      <input
        class="el-upload__input"
        type="file"
        nwdirectory
        id="importFunIpt"
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
      DoArr: [1, 0, 0, 0, 1],
      DiArr: [1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0],
      frontArr: [1, 0, 0, 0, 1],
      endArr: [1, 0, 0, 0, 1],
      originArr: [1, 0, 0, 0, 1],
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
      currentPage: 1,
      pageSize: 10,
      allProject: [],
      pickerOptions: {
        shortcuts: [
          {
            text: "最近一周",
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 7);
              picker.$emit("pick", [start, end]);
            },
          },
          {
            text: "最近一个月",
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 30);
              picker.$emit("pick", [start, end]);
            },
          },
          {
            text: "最近三个月",
            onClick(picker) {
              const end = new Date();
              const start = new Date();
              start.setTime(start.getTime() - 3600 * 1000 * 24 * 90);
              picker.$emit("pick", [start, end]);
            },
          },
        ],
      },
      dateValue: [],
    };
  },
  mounted() {
    let that = this;
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
    this.configPath = path.join(startPath,'sysConfig.json')
    console.error("startPath", lastOne, startPath);
    if(!fs.existsSync(this.configPath)){
      this.showSetting = true;
      return
    }

    this.initFun()

    // 底部拖拽
    setTimeout(() => {
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
    }, 200);
  },
  methods: {
    initFun(){
      // 回填配置内容
      this.getConf(this.configPath)

      let cbFun = () => {
        setTimeout(()=>{
          this.getAllProject();
        },2000)
        console.error("回调回来--");
        // 初始化
        this.init();
      };
      // 开发ui界面时，可以先不用后台
      this.startServer(cbFun);
      this.startWs();
      // console.error('axios',this.$axios);

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
          that.updateDi(rel.data.data.DiLamp);

          // 更新前限
          that.updateFront(rel.data.data.frontLimit);
          // 更新后限
          that.updateEnd(rel.data.data.endLimit);
          // 更新原点
          that.updateOrigin(rel.data.data.origin);

          // 查询轴状态
          that.getAxisStatus();

          // 一次完成，马上开始下一次
          await that.sleep(10000);
          if(!that.isHurryStop){
            loopRun_getDi();
          }
        } catch (error) {
          // 如果调用失败，就不开始了
          console.error("loopRun-err:", error);
        }
      }
      // loopRun_getDi();

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

          that.updateDo(rel.data.data);
          // 一次完成，马上开始下一次
          await that.sleep(12000);
          if(!that.isHurryStop){
            loopRun_getDo();
          }
          
        } catch (error) {
          // 如果调用失败，就不开始了
          console.error("loopRun-err:", error);
        }
      }
      // loopRun_getDo();
    },
    getConf(filePath){
      this.setObj = JSON.parse(fs.readFileSync(filePath).toString())
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
      document.getElementById('importFunForm') &&
        document.getElementById('importFunForm').reset();
      // 再打开
      document.getElementById('importFunIpt').click();
    },
    async selectPathChange() {
      const theSelectPath = event.target.value;
      console.log('selectPathChange:', theSelectPath);
      if(this.isLogPath){
        this.setObj.logPath= theSelectPath;
      }else{
        this.setObj.projectPath= theSelectPath;
      }
      
    },
    sureSet(formName) {
      console.error('set:',this.setObj);
      this.$refs[formName].validate((valid) => {
        if (valid) {
          this.showSetting = false;
          // 将配置内容保存到当前目录的文件下
          fs.writeFileSync(this.configPath, JSON.stringify(this.setObj,null,2))
          this.myMsg('保存成功！')
          // 配置完成后，重启项目
          this.initFun();
        } else {
          console.log('error submit!!');
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
        case "sendVisual":
          this.sendVisual();
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
    async sendVisual() {
      console.error("sendVisual被触发");
      let rel = await this.postData({
        url: "/commonFun",
        allData: {
          data: {
            type: "sendVisual",
            data: "TEST",
          },
        },
      });
      console.error("sendVisual-rel", rel);
    },
    // 回原
    async goBack(axis) {
      this.showLoad = true;
      // axis,vel, acc,dec, offset
      let rel = await this.postData({
        url: "/dllFun",
        allData: {
          data: {
            type: "goBack",
            axis: axis === "X" ? 1 : 2,
            vel: this.currentProject.HOME_Vel,
            acc: this.currentProject.HOME_Acc,
            dec: this.currentProject.HOME_Dec,
            offset: this.currentProject.HOME_offset,
          },
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
      // （4-1）*间隔+初始位置：目标位置参数
      if (this.runType) {
        pos =
          (4 - 1) * this.currentProject.Z_POS_gap +
          this.currentProject.Z_POS_dut1;
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
    updateDo(num = 15) {
      let rel = Util.num2Binary(num, 5);
      console.error("updateDo-rel", rel);
      this.DoArr = rel;
    },
    updateDi(num = 65534) {
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
      this.DiArr = rel;this.DiArr = rel;
    },
    updateFront(num = 0) {
      // console.error('front',num);
      let rel = Util.num2Binary(num, 16);
      // console.error('front-rel1',rel);
      this.frontArr = rel;
    },
    updateEnd(num = 0) {
      // console.error('End',num);
      let rel = Util.num2Binary(num, 16);
      // console.error('End-rel1',rel);
      this.endArr = rel;
    },
    updateOrigin(num = 0) {
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
          break;
        }
      }
    },
    async getAllProject() {
      console.error('获取所有项目数据：getAllProject');
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
      console.error("oneTest:", oneTest, process.env.NODE_ENV);
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
    handleSizeChange(val) {
      console.log(`每页 ${val} 条`);
      this.pageSize = val;
      this.getLatestLog();
    },
    handleCurrentChange(val) {
      console.log(`当前页: ${val}`);
      this.currentPage = val;
      this.getLatestLog();
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
    },
    dealMsg({ type, data }) {
      switch (type) {
        case "currentProject":
          console.error("cur_pro:", data);
          // 判断是否切换层--切换产品
          if (
            this.currentProject &&
            this.currentProject.cmdArr[2] !== data.cmdArr[2]
          ) {
            console.error(
              "切换层：",
              this.currentProject.cmdArr[2],
              data.cmdArr[2]
            );
          }
          this.currentProject = data;
          this.projectName = data.projectName;
          this.break;
        case "save":
          if (data) {
            // 处理本次测试结果
            console.error("save_data:", data);
            // 写入到文件中
            let logFilePath = this.getLogFilePath()
            let now = Util.format(Date.now())
            let str = JSON.stringify(data)
            let row = `${now}: ${str}`
            // 回显到页面上
            this.tableData.push(row)
            row = row+'\r\n'
            fs.appendFileSync(logFilePath, row)

            // 在页面显示
            this.dealResult(data.name,data.result);
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
    async getLatestLog() {
      let filter = {};
      console.error("this.dateValue", this.dateValue);
      if (this.dateValue.length == 2) {
        filter.date = {
          $gte: this.dateValue[0],
          $lte: this.dateValue[1],
        };
      }
      let rel = await this.postData({
        url: "/findFun",
        allData: {
          filter,
          condition: {
            currentPage: this.currentPage,
            pageSize: this.pageSize,
            sort: { date: -1 },
          },
        },
      });
      console.error("getLog-rel", rel);
      this.tableData = rel.data.data.data;
      this.total = rel.data.data.count;
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
      const ls = spawn("node", [serverPath,this.startPath,'one']);
      ls.stdout.on("data", (data) => {
        // console.log(`node-stdout: ${data}`);
        that.serverIsExist = true;
        if (!timer) {
          timer = setTimeout(() => {
            callback();
            clearTimeout(timer);
          }, 200);
        }
      });

      ls.stderr.on("data", (data) => {
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

      ls.on("close", (code) => {
        console.log(`node-child process exited with code ${code}`);
        that.serverIsExist = false;
      });
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
  height: 2px;
  background-color: #ddd;
  cursor: ns-resize;
}
.setIcon{
  font-size:28px; position:relative; top:5px; right:5px;
}
</style>
