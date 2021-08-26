/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-30 18:33:47
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-20 16:45:21
 */
export default{
  methods:{
    sleep(delay){
      return new Promise((resolve,reject)=>{
        let tId = setTimeout(()=>{
          clearTimeout(tId);
          resolve()
        },delay)
      })
    },
    myMsg(theText, theType='success') {
      this.$message({
        message: theText,
        type: theType
      });
    },
    getData(obj){
      return new Promise((resolve,reject)=>{
        let url=''
        if(obj.url){
          url = window.baseUrl+obj.url
        }
        this.$axios.get(url)
        .then(function (res) {
          console.log('suc:',res);
          resolve(res)
        })
        .catch(function (error) {
          console.log('error',error);
          reject(error)
        })
      })
    },
    postData(obj){
      let url=''
      if(obj.url){
        url = window.baseUrl+obj.url
      }
      return new Promise((resolve,reject)=>{
        this.$axios.post(url, obj.allData)
        .then(function (res) {
          // console.log('suc:',res);
          resolve(res)
        })
        .catch(function (error) {
          console.log('error',error);
          reject(error)
        })
      })
    },
    answerFun(text='确定删除吗？'){
      return new Promise((resolve,reject)=>{
        this.$confirm(text, '提示', {
          confirmButtonText: '确定',
          cancelButtonText: '取消',
          type: 'warning'
        }).then(() => {
          resolve(true)
        }).catch(() => {
          resolve(false)       
        });
      })
    },
    // 毫数转格式 【yyyy-MM-dd】
    formatDate(time, format = 'yyyy-MM-dd HH:mm:ss') {
      const t = new Date(time);
      const tf = function (i) { return (i < 10 ? '0' : '') + i; };
      return format.replace(/yyyy|MM|dd|HH|mm|ss/g, function (a) {
        switch (a) {
          case 'yyyy':
            return tf(t.getFullYear());
          // break;
          case 'MM':
            return tf(t.getMonth() + 1);
          // break;
          case 'mm':
            return tf(t.getMinutes());
          // break;
          case 'dd':
            return tf(t.getDate());
          // break;
          case 'HH':
            return tf(t.getHours());
          // break;
          case 'ss':
            return tf(t.getSeconds());
          // break;
        };
      });
    }
  }
}