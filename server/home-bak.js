/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-08-02 18:54:22
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-08-02 18:54:23
 */


      // 查询dll 线程
      // let dllPath = 'D:/pratdemo/nw_demo/vue_nw_node/sike-0726/process/child.js'
      // let dllPath = './process/child.js'
      // console.error('dllPath',dllPath);
      // // const dll_pr = spawn('node', [dllPath]);
      // const dll_pr = ch_pr.fork(dllPath);
      // console.error('dll_pr',dll_pr);
      // // 通信
      // let count=0
      // dll_pr.on('message', (m) => {
      //   console.log('父进程收到消息12', m);
      //   count++;
      //   if(count>5){
      //     console.error('dll_pr.pid',dll_pr.pid);
      //     // ch_pr.fork('kill', [dll_pr.pid]);
      //     // process.exit();
      //   }
      // });
      // // 使子进程输出: 子进程收到消息 { hello: 'world' }
      // dll_pr.send({ hello: 'world' });

      
      
      // mongodb 线程
      // var mongodArgs = ['--dbpath', dbDataPath, '--port', 27100];

      // const mongo_pr = spawn(mongodbAppPath, mongodArgs);
      // mongo_pr.stdout.on('data', (data) => {
      //   console.log(`mongo-stdout: ${data}`,this);
      // });

      // mongo_pr.stderr.on('data', (data) => {
      //   console.error(`mongo-stderr: ${data}`);
      // });

      // mongo_pr.on('close', (code) => {
      //   console.log(`mongo-child process exited with code ${code}`);
      // });