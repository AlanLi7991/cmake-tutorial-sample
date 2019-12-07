cmake_minimum_required(VERSION 3.14)

project(HelloWorld)

message("-------------------------------------------")
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
message("load_message_func : " ${MESSAGE_TWO} "\n")

# 声明Func，并采用作用域提升
function(load_message_parent variable)
    set(${variable} "message_three")
    set(${variable} ${${variable}} PARENT_SCOPE)
endfunction()

# 使用了变量提升的Func
load_message_parent(MESSAGE_THREE)
message("load_message_parent : " ${MESSAGE_THREE} "\n")
message("-------------------------------------------")

add_executable(main main.cpp)
