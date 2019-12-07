title: CMake后置-如何读懂复杂的文档
date: 2019-12-09 00:00:00
categories: Tutorial
tags: [CMake]

---

## 如何读懂复杂的文档

CMake的官方文档一直都以晦涩著称，学习曲线比较陡峭，仅有指令的手册没有完善的入门指导

这里补充一些入门提示，帮助新手理解

```
file(GLOB <variable>
     [LIST_DIRECTORIES true|false] [RELATIVE <path>] [CONFIGURE_DEPENDS]
     [<globbing-expressions>...])

```

### 符号的含义

| 符号    | 含义    |  示例 |
| :------------ | :------------ | :------------ |
| Plain Text    | 命令本身，不能修改和自定义 | file、GLOB |
| Brackets () | 函数的参数 | 和所有类C语言一样 |
| Angle Brackets <> | 必选参数 | \<variable> |
| Square Brackets [] | 可选参数 | [CONFIGURE_DEPENDS] |
| Oring \| | 多选一的可选项 | true\|false | 
| Variable | 用户自定义变量 | variable |
| Ellipsis ... | 变长参数 | [\<>...] |


这些符号规则和Shell的说明文档十分类似，如果有Linux相关经验的人应该不难理解

### 解析举例

根据符号分析，我们可以拆分解析指令

1. file是指令本身
2. 第一个固定参数 GLOB 代表Action
3. 第二个参数代表传入一个变量用于储存结果
4. 第三、四个参数一起作为可选输入参数，参数名 LIST_DIRECTORIES 参数值 true 或 false
5. 第五、六个参数一起作为可选输入参数，但是如果第四个存在，则第五个必须存在，参数名 RELATIVE 参数值为一个路径
6. 第七个参数参数为可选参数，本身不附带输入，为标记位，参数名 CONFIGURE_DEPENDS
7. 其八个参数为可选参数，该参数为变长参数，且必须使用 globbing-expressions 

```SH
# file : 固定命令
# GLOB : 固定一个参数
# CPP_FILES : 第二个参数
# 3 - 7 个参数均被省略
# ..../*.cpp : 第八个参数

file(GLOB CPP_FILES ${CMAKE_CURRENT_LIST_DIR}/source/*.cpp)

```