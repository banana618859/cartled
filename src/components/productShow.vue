<!--
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-26 17:09:46
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-25 18:44:04
-->
<template>
  <div 
    style="width:100%; height:100%"
    class="flexBox-c">
    <p style="height:50px; padding: 15px;">
      <el-button type="primary" size="mini" :disabled="!runType" @click="$emit('reset')">复位</el-button>
      <el-button type="success" size="mini" :disabled="!runType" @click="$emit('start',true)">启动</el-button>
      <el-button type="warning" size="mini" :disabled="!runType"  @click="$emit('start',false)">暂停</el-button>
      <span class="tipsText">
        <span v-if="runType" class="color_green">自动运行中</span>
        <span v-else>调试中</span>
      </span>
    </p>
    <hr/>
    <div class="flex1 over_auto">
      <ul class="proUl">
        <li v-for="(item, index) in productData" :key="item.name+index" class="flexBox-r">
          <div class="text-c leftTitle">
            {{ item.name }}
          </div>
          <div class="flex1 mg-l10 faBtn">
            <el-button type="success" @click="$emit('showImg',one)" size="mini" v-for="(one, index) in item.allTest" :key="index">
              {{ one.name }}
            </el-button>
          </div>
        </li>
      </ul>
    </div>
    <p v-if="DiArr.length" style="height:30px; line-height:30px; padding:0 5px;">
      <span>
        <i class="small_yuan mg-r5" :class="[DiArr[14].value ? 'bg_green': '']"></i>
        <span v-if="DiArr[14].value">小车到位</span>
        <span v-else class="myRed">小车未到位</span>
        </span>
      <span class="mg-l20">
        <i class="small_yuan mg-r5" :class="[DiArr[5].value ? 'bg_green' : '']"></i>
        左夹手{{DiArr[4].value? '已松开': (DiArr[5].value ? '已夹紧': '')}}
        </span>
      <span class="mg-l20">
        <i class="small_yuan mg-r5" :class="[DiArr[7].value ? 'bg_green':'']"></i>
        右夹手{{DiArr[6].value? '已松开': (DiArr[7].value ? '已夹紧': '')}}</span>
    </p>
  </div>
</template>

<script>
export default {
  props:{
    isStart:{
      type: Boolean,
      default: true
    },
    runType:{
      type: Boolean,
      default: true
    },
    DiArr:{
      type: Array,
      default: ()=>[]
    },
    productData:{
      type: Array,
      default: ()=>[]
    }
  },

  methods:{}
}
</script>

<style scoped>
  .proUl li{margin-top: 10px; text-align: left;}
  .leftTitle{width: 60px; height: 40px; line-height: 40px;}
  .el-button+.el-button{margin-left: 0;}
  .faBtn .el-button{margin-bottom: 10px; margin-left: 10px;}
  
  .tipsText{
    margin-left: 10px;
    display: inline-block;
    line-height: 26px;
    border-bottom: 2px solid #ccc;
    text-align: center;
    padding: 0 5px;
  }
</style>