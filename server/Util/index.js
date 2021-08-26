/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-08-04 10:23:34
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-16 10:52:11
 */
var Util={
  num2Binary(num, numLen=16){
    var rel=[]
    for(let i=0;i<numLen;i++){
        rel.push(0)
    }
    var loopNum=-1
    for(let i=0;loopNum<0;i++){
        if(num < Math.pow(2,i)){
            loopNum=i-1
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
    rel = rel.reverse()
    return JSON.parse(JSON.stringify(rel));
  }
}

module.exports = Util;