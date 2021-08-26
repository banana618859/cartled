/*
 * @Descripttion: unTitle
 * @Author: yizheng.yuan
 * @Date: 2021-07-28 16:30:01
 * @LastEditors: yizheng.yuan
 * @LastEditTime: 2021-07-28 16:30:17
 */

// https://www.jianshu.com/p/dd9463dead8c

const ffi = require('ffi-napi')
var ref = require('ref-napi')
var ArrayType = require('ref-array-napi')
const path = require('path')

// 映射到C语言 int数组类型
var IntArray = ArrayType(ref.types.int)

// 加载 DLL文件,无需写扩展名,将DLL中的函数映射成JS方法
const MyDellDemo = new ffi.Library(path.resolve('MYDLLDEMO'), {
  // 方法名必须与C函数名一致
  add: [
    'int', // 对应 C函数返回类型
    ['int', 'int'] // C函数参数列表
  ],
   // 使用 ffi中内置类型的简写类型
  addPtr: ['void', ['int', 'int', 'int*']],
   // IntArray 是上面通过 ArrayType 构建出来的类型
  initArray: ['void', [IntArray, 'int']]
})

// 调用add 方法
const result = MyDellDemo.add(1, 2)
console.log(`add method result of 1 + 2 is: ` + result)

// 调用addPtr 方法
// 使用Buffer类在C代码和JS代码之间实现了内存共享，让Buffer成为了C语言当中的指针。
// C函数使用指针操作函数外部的内存，所以首先需要 分配一个int类型的内存空间 第一个参数为 C语言数据类型，第二个参数为 默认值
var intBuf = ref.alloc(ref.types.int, 100)
console.log('addPtr 调用前数据>>', ref.deref(intBuf)) //获取指向的内容
MyDellDemo.addPtr(2, 2, intBuf) // 调用函数，传递指针
console.log('addPtr 调用后数据>>', ref.deref(intBuf))

// 调用initArray 方法
// IntArray 是前面使用ref-napi 和 ref-array-napi 库创建的数据类型,数组的长度为 8
// 这里一定要分配内存空间，否则 函数内的指针无法操作内存
let myArray = new IntArray(8)
MyDellDemo.initArray(myArray, 8)
console.log('初始化数组执行结果:')
for (var i = 0; i < myArray.length; i++) {
  console.log(myArray[i])
}