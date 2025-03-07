# 最近要落地 室内商场的三维展示 场景 ，于是在github上一顿找，没有找到好的。最符合的要求就是：

## indoor3D
### 基于原生js的以及比较低版本的three.js
https://github.com/wolfwind521/indoor3D

### 它有配套的编辑器:基于qt5的c++
https://github.com/wolfwind521/IndoorMapEditor

## 它们也有各种衍生：

### Angular版本的 也是早期node版本的
https://github.com/tangerren/Indoor-Map
https://github.com/tangerren/Indoor-Map-Draw
未开发完成，也挺久的了

### 在展示的部分 增加了 shapefile格式
https://github.com/tommy2gis/indoor3D
同样也是原生的，也挺久的了

## 采坑记录
本方案是基于 wolfwind521/indoor3D 的一套

### 更新代码 [本项目master分支]
代码直接拉取后 发现了问题，因为是很早之前的代码了，使用的是qt5.x的版本
我用的是maco15.1.1 arm的m1 ，原则上从codex和sdk都不太兼容
所以使用最新的qt6.x，然后codex也是最新的，当然项目indoor3D函数也根据最新的qt6.x方式更改了，即目前看到的最新版本
打包编译运行都可以了。 但是app很脆弱很容易崩亏。

### 匹配环境[本项目qt5.x分支]
所以想是不是更改代码或者本身项目升级就会出现问题，因为c++的程序嘛，对底层内存的控制不同cpu运算字符了，可能会有区别。
就用x86的笔记本，虚拟了macos14.x版本的系统，安装了qt5.x的版本 codex 10的sdk
就是本项目qt5.x分支部分代码。
同样打包编译运行都可以了。 但是app还是很脆弱很容易崩亏。

### 崩溃思考
不管使用哪种方式，打开app后，新建项目随便新增编辑就会崩溃
或者载入现有json数据文件，出来看不到商铺，编辑也会崩溃
### 待优化
如果有人熟悉qt的UI部分，看了报错信息，还是挺难调试的
### 操作使用
如果有人知道操作方法，也请issues告知大家

# 重构 three.js geojson
本人是tvt.js的作者:https://github.com/hawk86104/three-vue-tres
所以想在插件商场落地这个场景，看了数据结构后，还是想用通用的geojson格式的配置文件，通过tvt的底子来展示三维场景
编辑器部分将使用通用的geojson编辑器
已完成 详见 https://www.icegl.cn/tvtstore/networkLinkTopology.html
