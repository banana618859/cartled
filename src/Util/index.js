/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-08-04 10:23:34
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-25 11:43:37
 */
export default{
  // 毫数转格式 【yyyy-MM-dd】
  format(time, format = 'yyyy-MM-dd HH:mm:ss') {
    const t = new Date(time);
    const tf = function (i, notZero) { return (i < 10 && !notZero ? '0' : '') + i; };
    return format.replace(/yyyy|MM|dd|HH|mm|ss|hs/g, function (a) {
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
        case 'hs':
          return tf(t.getMilliseconds(), true);
            // break;
      };
    });
  },
  num2Binary(num, numLen){
    var rel=[]
    for(let i=0;i<numLen;i++){
        rel.push(0)
    }
    if(num > 0){
      var loopNum=-1
      for(let i=0;loopNum<0;i++){
          if(num < Math.pow(2,i)){
            loopNum=i-1
            break;
          }
      }

      // 从大取到小
      for(let i=loopNum; i>-1;i--){
          let nowOne =  Math.pow(2,i);
          // console.log('i',i,nowOne)
          let lastIndex = rel.length-(i+1);
          if(num < nowOne){
              rel.splice(lastIndex,1,0)
          }else{
              num = num - nowOne;
              rel.splice(lastIndex,1,1)
          }
      }
    }
    rel = rel.reverse()
    return JSON.parse(JSON.stringify(rel));
  }
}