/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-19 14:44:22
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-07-29 15:39:29
 */
const path = require('path');
const fs = require('fs');
const NwBuilder = require('nw-builder');
const copydir = require('node-copydir');
const ch_pr = require('child_process');

let appName = 'app';
let platform = 'win64';
const buildPath = 'build';
const buildPublicPath = 'app-build';

// 先打包开发代码
// node build/build.js
// node 线程
// 自动执行函数
(async ()=>{
  try {
    await build_dev_code();
    console.error('开发代码打包完成--');
    Run();
  } catch (error) {
    console.error('开发代码打包出错--', error);
  }
  
})()

// 打包开发代码
function build_dev_code(){
  let devPath= './build/build.js'
  return new Promise((resolve,reject)=>{
    const ls = ch_pr.spawn('node', [devPath]);
    ls.stdout.on('data', (data) => {
      console.log(`node-stdout: ${data}`);
    });

    ls.stderr.on('data', (data) => {
      console.error(`node-stderr: ${data}`);
    });

    ls.on('close', (code) => {
      console.log(`node-child process exited with code ${code}`);
      resolve()
    });
  })
}

async function Run(){
  // 拷贝 开发完成后打包的文件
  await copyDist();
  console.error('资源拷贝完毕-------');
  build(callbackFun)
}

async function copyDist() {
  await copydir('./dist', buildPublicPath)
}

function build(cb){
  const jsonPath = './package.json'
  const pkJson = JSON.parse(fs.readFileSync(jsonPath).toString())
  console.error('pkjson',pkJson);


  // 注意nw-builder打包一定要.ico后缀的图片才行的，否则不生效
  let iconPath = '';
  if(pkJson){
    appName = pkJson.name;
    platform = pkJson.build.platform;
    iconPath = pkJson.window.buildIcon;
  }
  console.error('iconPath:',iconPath);

  var nw = new NwBuilder({
    appName,
    files: [
      `./${buildPublicPath}/package.json`,
      `./${buildPublicPath}/index.html`,
      `./${buildPublicPath}/icon.png`,
      `./${buildPublicPath}/static/**`
    ], // 包含文件
    winIco : iconPath, // windows系统的icon
    macIcns: iconPath, // mac系统的icon
    platforms: [platform], // 打包的平台
    version: '0.37.0', // 使用 NW.js 的版本
    flavor: "sdk",
    buildDir: `./${buildPublicPath}/${buildPath}`,
    cacheDir: `./${buildPublicPath}/cache`
  });

  nw.on('log', console.log); // 日志打印函数

  // 开始构建
  nw.build().then(function(){
    console.log('done!');
    // 执行回调
    cb();
  }).catch(function(err){
    console.log(err);
  });
}

const callbackFun = async () =>{
  console.error('拷贝服务器');
  const appPath = path.join(buildPublicPath, `/${buildPath}/${appName}/${platform}`);
  const serverSourcePath = './server'
  const serverDestPath = path.join(appPath, 'server');

  await copydir(serverSourcePath, serverDestPath);
  console.error('打包完成！');
}


