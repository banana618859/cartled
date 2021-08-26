/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-19 17:43:31
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-07-19 17:56:33
 */

const { spawn } = require('child_process');
const dbDataPath = 'D:/Mongodb/data'
console.error('启动数据库------------');
 // mongodb 线程
 var mongodArgs = ['--dbpath', dbDataPath, '--port', 27100];

 const mongo_pr = spawn('mongod', mongodArgs);
 mongo_pr.stdout.on('data', (data) => {
   console.log(`mongo-stdout: ${data}`,this);
 });

 mongo_pr.stderr.on('data', (data) => {
   console.error(`mongo-stderr: ${data}`);
 });

 mongo_pr.on('close', (code) => {
   console.log(`mongo-child process exited with code ${code}`);
 });