/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-19 10:11:04
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-25 18:55:46
 */
// The Vue build version to load with the `import` command
// (runtime-only or standalone) has been set in webpack.base.conf with an alias.
import Vue from 'vue'
import App from './App'
import router from './router'

// element-ui引入
import ElementUI from 'element-ui';
import 'element-ui/lib/theme-chalk/index.css';
Vue.use(ElementUI);

// 后面引入，可以覆盖element-ui的全局样式
import './assets/my-reset.css'
import './assets/pd-mg.css'
import './assets/common.css'

// vue mixin
import mixin from './mixin';
Vue.mixin(mixin)

Vue.config.productionTip = false

// 后台服务器路径
window.baseUrl = 'http://localhost:8100';

// 引入axios
import axios from 'axios'
Vue.prototype.$axios = axios

/* eslint-disable no-new */
new Vue({
  el: '#app',
  router,
  components: { App },
  template: '<App/>'
})
