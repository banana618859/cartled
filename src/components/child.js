/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-27 18:28:23
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-07-27 18:41:35
 */
console.log('子进程运行文件--');
// child.js
process.on('message', (m) => {
  console.log('子进程收到消息', m);
});

// 使父进程输出: 父进程收到消息 { foo: 'bar', baz: null }
process.send({ foo: 'bar', baz: NaN });