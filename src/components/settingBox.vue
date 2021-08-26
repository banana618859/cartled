<!--
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-26 17:09:46
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-25 19:24:04
-->
<template>
  <div class="swrap text-l posr flexBox-c">
    <p class="title_30 mg-b5">参数配置</p>
    <el-form ref="form" :model="currentProject" class="pd-r10" size="mini" label-width="100px">
      <el-form-item label="X轴DUT位置">
        <el-input v-if="currentProject" type="number" :disabled="true" v-model.number="currentProject.X_POS_dut1" placeholder="e输入数值"></el-input>
      </el-form-item>
      <el-form-item label="Z轴DUT位置">
        <el-input v-if="currentProject" type="number" :disabled="true" v-model.number="currentProject.Z_POS_dut1" placeholder="e输入数值"></el-input>
      </el-form-item>
      <el-form-item label="Z轴间隔">
        <el-input v-if="currentProject" type="number" :disabled="true" v-model.number="currentProject.Z_POS_gap" placeholder="e输入数值"></el-input>
      </el-form-item>
    </el-form>
    <p class="title_30 mg-b5">
      <el-button 
        :type="DiArr[7] ? 'success': 'primary'" 
        @click="$emit('rightBite',DiArr[7])" class="fr" size="mini">
        右夹手{{DiArr[7]? '松开': '夹紧'}}
      </el-button>
      <el-button 
        :type="DiArr[5] ? 'success': 'primary'" 
         @click="$emit('leftBite', DiArr[5])" class="fr mg-r5" size="mini">
        左夹手{{DiArr[5]? '松开': '夹紧'}}
      </el-button>
      手动操作</p>
    <div class="flex1 over_auto">
      <el-form ref="formA" :model="formA" class="pd-r10" size="mini" label-width="100px">
        <el-form-item :label="'DO_1-'+DoArr.length">
          <div class="over_hide">
            <ul class="border_ddd pd-l5 wrapUl">
              <li 
                v-for="(item,index) in DoArr"
                :key="item.name+index"
                class="hand"
                @click="changeDo(item.value, index)">
                <el-tooltip class="item" effect="dark" :content="item.name" placement="top">
                  <div>
                    <div class="oneP">
                      <i class="small_yuan_14" :class="[item.value ? 'bg_green' : '']"></i>
                    </div>
                    <div class="oneP">{{index}}</div>
                  </div>
                </el-tooltip>
              </li>
            </ul>
          </div>
        </el-form-item>
        <el-form-item :label="'DI_1-'+DiArr.length">
          <div class="over_hide">
            <ul class="border_ddd pd-l5 wrapUl">
              <li v-for="(item,index) in DiArr"
                :key="'do'+item+index">
                <el-tooltip class="item" effect="dark" :content="item.name" placement="top">
                  <div>
                    <div class="oneP">
                      <i class="small_yuan_14" :class="[item.value ? 'bg_green' : '']"></i>
                    </div>
                    <div class="oneP">{{index}}</div>
                  </div>
                </el-tooltip>
              </li>
            </ul>
          </div>
        </el-form-item>
        
        <el-form-item label="轴">
          <div style="background-color: #eee;">
            <el-radio-group v-model="formA.currentAxis">
              <el-radio label="X" class="mg-l5">X轴</el-radio>
              <el-radio label="Z">Z轴</el-radio>
            </el-radio-group>
            <el-button type="success" class="fr" size="mini">初始化</el-button>
          </div>
        </el-form-item>
          <el-form-item label="操作">
          <div class="inline-btn">
            <el-button type="primary" size="mini" @click="$emit('setFun','goBack',formA.currentAxis)">回原</el-button>
            <el-button type="primary" size="mini" @click="$emit('setFun','move',formA.currentAxis,formA.targetLocation)">移动</el-button>
            <el-button type="primary" v-loading="visualWait" size="mini" @click="$emit('setFun','triggerVisual')">触发视觉</el-button>
          </div>
        </el-form-item>
        <el-form-item label="手动目标位置">
          <el-input  type="number" v-model.number="formA.targetLocation" placeholder="e输入数值">
            <template slot="append">mm</template>
          </el-input>
        </el-form-item>
        <el-form-item label="移动速度">
          <el-input v-if="currentProject" type="number" v-model.number="currentProject.MAN_Vel" placeholder="e输入数值"></el-input>
        </el-form-item>
        <el-form-item label="移动加速度">
          <el-input v-if="currentProject" type="number" v-model.number="currentProject.MAN_Acc" placeholder="e输入数值"></el-input>
        </el-form-item>
      </el-form>
      <div style="height:150px" class="flexBox-r mg-t15">
        <div style="width:28%">
          <ul class="ul20">
            <li>
              &nbsp;<br/>
              &nbsp;<br/>
              X<br/>
              Z
            </li>
            <li>
              正限 
              <i class="small_yuan mg-r5" :class="[frontArr[0] ? '': 'bg_green']"></i>
              <i class="small_yuan mg-r5" :class="[frontArr[1] ? '': 'bg_green']"></i>
            </li>
            <li>
              原点
              <i class="small_yuan mg-r5" :class="[originArr[0] ? '': 'bg_green']"></i>
              <i class="small_yuan mg-r5" :class="[originArr[1] ? '': 'bg_green']"></i>
            </li>
            <li>
              负限
              <i class="small_yuan mg-r5" :class="[endArr[0] ? '': 'bg_green']"></i>
              <i class="small_yuan mg-r5" :class="[endArr[1] ? '': 'bg_green']"></i>
            </li>
          </ul>
        </div>
        <div class="flex1">
          <div style="height:50px;">
            <div class="img_cover mg_auto topImg w40h100 posr hand"
              @mousedown="$emit('directionMove','z',false)"
              @mouseup  ="$emit('directionStop','z',true)">
              <div class="topText">z上升</div>
            </div>
          </div>
          <div style="height:40px" class="flexBox-r">
            <div style="width:50%" class="pd-r20">
              <div class="img_cover topImg2 fr rotate-180 w50h100 posr hand"
              @mousedown="$emit('directionMove','x',true)"
              @mouseup  ="$emit('directionStop','x',true)">
                <div class="leftText">x前进</div>
              </div>
            </div>
            
            <div style="width:50%" class="pd-l20">
              <div class="img_cover topImg2 w50h100 posr hand"
              @mousedown="$emit('directionMove','x',false)"
              @mouseup  ="$emit('directionStop','x',false)">
                <div class="rightText">x后退</div>
              </div>
            </div>
          </div>
          <div style="height:50px;" >
            <div class="img_cover mg_auto topImg rotate180 w40h100 posr hand"
              @mousedown="$emit('directionMove','z',true)"
              @mouseup  ="$emit('directionStop','z',false)">
              <div class="bottomText">z下降</div>
            </div>
          </div>
        </div>
      </div>
    </div>

    <div class="bottomLine flexBox-r">
      <div class="flex1">X轴：{{xPos}}</div>
      <div class="flex1">Z轴：{{zPos}}</div>
    </div>
  </div>
</template>

<script>
export default {
  props:{
    currentProject:{
      type: Object,
      default: ()=>{}
    },
    xPos:{
      type: Number,
      default: 0
    },
    zPos:{
      type: Number,
      default: 0
    },
    DoArr:{
      type: Array,
      default: ()=>[]
    },
    DiArr:{
      type: Array,
      default: ()=>[]
    },
    frontArr:{
      type: Array,
      default: ()=>[]
    },
    endArr:{
      type: Array,
      default: ()=>[]
    },
    originArr:{
      type: Array,
      default: ()=>[]
    },
    visualWait:{
      type: Boolean,
      default: false
    }
  },
  data(){
    return{
      form:{
        xDut: '',
        zDut: '',
        zGap: '',        
      },
      formA:{
        currentAxis: 'Z',
        targetLocation: '',
        moveSpeed: '',
        MoveAcceleration: ''
      }
    }
  },
  mounted(){
    console.error('DoArr',this.DoArr);
  },
  methods:{
    submit(item){
      console.error('submit',item);
    },
    async changeDo(item,index){
      let rel = await this.answerFun('确定修改吗？');
      if(!rel){return}
      console.error('changeDo:',item,index);
      let status=0;
      if(!item){
        status=1
      }
      this.$emit('setDo',index, status)
    }
  }
}
</script>

<style scoped>
  .swrap{
    width:100%; height:100%;
  }
  .el-form-item--mini.el-form-item, .el-form-item--small.el-form-item{
    margin-bottom: 5px;
  }
  .ul20{display: flex; height: 100%; justify-content: space-between; padding: 10px; align-items: center;}
  .ul20 li{
    writing-mode: tb-lr;
    width: 20px;
  }
  .w40h100{width: 40px; height:100%;}
  .w50h100{width: 50px; height:100%;}
  .topText{position: absolute; 
    right: -35px;
    top: 20px;
  }
  .rightText{position: absolute; 
    right: 9px;
    top: 36px;
  }
  .bottomText{position: absolute; 
   right: -35px;
    bottom: 20px;
    transform: rotate(180deg);
  }
  .leftText{position: absolute; 
    right: 9px;
    top: 36px;
    transform: rotate(180deg);
  }
  .bottomLine{
    width: 100%;
    border-top:1px solid #ddd;
    height: 30px;
    line-height: 30px;
    padding: 0 10px;
  }
  .inline-btn .el-button{margin: 0; margin-bottom: 10px; margin-right: 10px;}

  .wrapUl{display: flex; flex-wrap: wrap; overflow: hidden; padding-bottom:5px;}
  .wrapUl li{margin-right: 12px; margin-bottom:8px; display: flex; flex-direction: column; align-items: center; justify-content: center;}
  .oneP{height: 13px; font-size: 12px; text-align: center;}
</style>