/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-21 11:22:50
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-23 14:22:17
 */
//tcp客户端
var net = require('net')
var client=null;
var netObj={
  close(){
    // 客户端主动断开
    if(client){
      client.end()
      client=null;
      console.error('服务器断开-与视觉连接');
    }
  },
  conn(port=7600){
    return new Promise((resolve,reject)=>{
      client = net.connect(
        {
          port,
        },
        function(error,result){
          console.error('error,result',error,result);
          console.log("net连接到服务器12")
          resolve(client)
      })

      // 如果发生错误会进来这里
      client.on('error',function(error){
        console.log('连接失败-error',error)
        reject(error)
      })

    })
  },
  conn_send(obj){
    console.error('conn_send_obj:',obj);
    return new Promise(async (resolve,reject)=>{
      if(!client){
        try {
          client = await netObj.conn(obj.port);
  
          //客户端收到服务 端执行的事件
          client.on('data',function(data){
            data = data.toString()
            console.log('net客户端：3到服务端响应数据为'+data.toString());
  
            // 等待结果回来再发给express服务器
            resolve(data)
  
            // 客户端主动断开
            client.end()
            client=null;
          })
          
          client.on('end',function(){
            console.log('net断开与服务器的连接3')
          })
  
          //如果连接没有出错，就给视觉服务端传递的数据
          console.error('发给视觉服务端');
          client.write(obj.data);
          
        } catch (error) {
          console.error('连接视觉服务器失败--here：',error);
          client=null;
          reject(error);
        }
        
      }
    })
  },
  async send(obj){
    console.error('发送开始');
    return new Promise(async (resolve,reject)=>{
      try {
        let rel = await netObj.conn_send(obj);
        console.error('发送完');
        resolve(rel);
      } catch (error) {
        console.error('发送出错：',error);
        reject(error);
      }
    })
    
  }
}

// netObj.send();

module.exports = netObj;