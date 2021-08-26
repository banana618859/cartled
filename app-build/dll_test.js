/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-19 18:51:24
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-07-19 18:53:15
 */
var ffi = require('ffi');
 
var libm = ffi.Library('libm', {
  'ceil': [ 'double', [ 'double' ] ]
});
libm.ceil(1.5); // 2
 
// You can also access just functions in the current process by passing a null
var current = ffi.Library(null, {
  'atoi': [ 'int', [ 'string' ] ]
});
current.atoi('1234'); // 1234