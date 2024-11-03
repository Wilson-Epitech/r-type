set(SERVER_SRC
    main.cpp
)

add_executable(rtype_server ${SERVER_SRC})

if(WIN32)
target_link_libraries(rtype_server PRIVATE rtype_lib ws2_32)
elseif(UNIX)
target_link_libraries(rtype_server PRIVATE rtype_lib)
endif()

add_library(server SHARED
    ./Server.cpp
    ../handleData/HandleData.cpp
    ../../ecs/components/Components.cpp
)

target_include_directories(server PRIVATE
    ../
    ../handleData
    ../../ecs
    ../../ecs/components
    ../../../lib
    ../../../include
)

if(WIN32)
target_link_libraries(server PRIVATE ws2_32)
endif()

add_custom_command(TARGET server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:server> ${CMAKE_SOURCE_DIR}/sharedLibraries
)

if(UNIX)
add_custom_command(TARGET rtype_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:rtype_server> ${CMAKE_SOURCE_DIR}/rtype_server
)
elseif(WIN32)
    set(RAYLIB_DLL_PATH "${CMAKE_SOURCE_DIR}/build/src/network/server/raylib.dll")
    set(GLFW3_DLL_PATH "${CMAKE_SOURCE_DIR}/build/src/network/server/glfw3.dll")

    add_custom_command(TARGET rtype_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${RAYLIB_DLL_PATH} ${CMAKE_SOURCE_DIR}/raylib.dll
    COMMAND ${CMAKE_COMMAND} -E copy
    ${GLFW3_DLL_PATH} ${CMAKE_SOURCE_DIR}/glfw3.dll
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:rtype_server> ${CMAKE_SOURCE_DIR}/rtype_server.exe
    )
endif()