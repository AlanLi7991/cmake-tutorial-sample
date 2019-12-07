title: CMake05-函数和宏的对比
date: 2019-12-05 00:00:00
categories: Tutorial
tags: [CMake]

---

## CMake的函数和宏

CMake本身整理逻辑可以通过 宏（macro） 和 函数（function），声明十分类似

## 官方文档说明

在宏的[官方文档中有提到两者不同的问题][link01]，提出了两点主要的差别

1. 宏是类似C语言的预处理，大部分是字符串替换，不会有函数内的变量
2. 遇到return()宏不会停止，函数会直接返回

```
The macro command is very similar to the function() command. ......

In a function, ARGN, ARGC, ARGV and ARGV0, ARGV1, … are true variables in the usual CMake sense. In a macro, they are not, they are string replacements much like the C preprocessor would do with a macro. .....

Another ..... .... return() in a macro body does not just terminate execution of the macro; ......

```

## 更重要的区别Scope不同

例如我们想输入一个变量获得结果，有两种方式，但是经过对比可以发现 load_message_func 并不能正确输出

原因是因为func拥有单独的scope, variable 和外部的并不通用，C语言里一般会采用传入指针来解决

```SH
# 声明Macro
macro(load_message_macro variable)
	set(${variable} "message_one")
endmacro()

# 声明Func
function(load_message_func variable)
	set(${variable} "message_two")
endfunction()

# 使用Macro
load_message_macro(MESSAGE_ONE)
message("load_message_macro : " ${MESSAGE_ONE})

# 使用Func
load_message_func(MESSAGE_TWO)
message("load_message_func : " ${MESSAGE_TWO})

```

CMake里的函数是没有返回值的，也就是说需要有方法做到类似JS的作用域提升

```SH
# 声明Func，并采用作用域提升
function(load_message_parent variable)
    set(${variable} "message_three")
	# 变量提升
	# ${variable} 被展开成 MESSAGE_THREE
	# ${${variable}} 被展开成 message_three 即 MESSAGE_THREE 的值
	# PARENT_SCOPE 指父作用域，是set方法的参数
    set(${variable} ${${variable}} PARENT_SCOPE)
endfunction()

# 使用了变量提升的Func
load_message_parent(MESSAGE_THREE)
message("load_message_parent : " ${MESSAGE_THREE} "\n")
```




[link01]:https://cmake.org/cmake/help/v3.15/command/macro.html