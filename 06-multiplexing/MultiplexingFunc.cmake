
function(glob_sources variable directory)
    file(GLOB_RECURSE ${variable} ${directory}/*.cpp)
    set(${variable} ${${variable}} PARENT_SCOPE)
endfunction()

