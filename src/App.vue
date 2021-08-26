<!--
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-19 10:11:04
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-07-23 18:00:59
-->
<template>
  <div id="app" class="wrapper">
    <!-- <img src="./assets/logo.png">-->
    <router-view/> 
  </div>
</template>

<script>
export default {
  name: 'App'
}
</script>

<style>
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
}
</style>
