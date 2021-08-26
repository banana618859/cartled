/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-21 11:37:54
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-24 11:08:16
 */
//tcp服务端
var net = require('net')
var connObj =null;
var server = null;
var nets={
  close()
  {
    // return new Promise((resolve,reject)=>{
    //   server.close(()=>{
    //     console.error('tcpServer-close:',arguments);
    //     resolve()
    //   })
    // })
    server.unref()
  },
  conn(port, callback){
    console.error('启动net服务器');
    server=net.createServer(function(connection){
      //客户端关闭连接执行的事件
      connObj = connection;
      connection.on('end',function(){
        console.log('客户端关闭连接')
      })
      let str=''
      connection.on('data',function(data){
        data = data.toString()
        console.log('服务端：2收到客户端发送数据为'+data)
        
        // connection.write('\r\nback:'+data.toString());
        // 如果收到回车'\r\n'即发送
        if(data==='\r\n'){
          callback(str)
          str = ''
        }else{
          str +=data;
        }
      })
      //给客户端响应的数据
      // connection.write('response hello'+Date.now())
    })
    
    server.listen(port,function(){
      console.error(`net_server监听telnet:端口${port}`)
    })
  },
  sendBack(data){
    //给客户端响应的数据
    connObj.write(data)
  }
}
// nets.conn()

module.exports = nets;