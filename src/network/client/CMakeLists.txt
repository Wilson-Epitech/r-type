set(CLIENT_SRC
    main.cpp
)

add_executable(rtype_client ${CLIENT_SRC})

target_link_libraries(rtype_client PRIVATE rtype_lib ws2_32)

add_library(client SHARED
    ./Client.cpp
    ../handleData/HandleData.cpp
    ../../ecs/components/Components.cpp
)

target_include_directories(client PRIVATE
    ../
    ../handleData
    ../../ecs
    ../../ecs/components
    ../../../lib
    ../../../include
)

target_link_libraries(client PRIVATE ws2_32)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(rtype_client
        PRIVATE "-framework OpenGL"
        PRIVATE "-framework Cocoa"
        PRIVATE "-framework IOKit"
        PRIVATE "-framework CoreVideo"
        PRIVATE "-framework GLUT"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(rtype_client
        PRIVATE GL
        PRIVATE m
        PRIVATE pthread
        PRIVATE dl
        PRIVATE rt
        PRIVATE X11
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(rtype_client
        PRIVATE opengl32
        PRIVATE gdi32
        PRIVATE winmm
        PRIVATE user32
    )
else()
    message(FATAL_ERROR "Unsupported OS")
endif()

add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:client> ${CMAKE_SOURCE_DIR}/sharedLibraries
)

if(UNIX)
add_custom_command(TARGET rtype_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:rtype_client> ${CMAKE_SOURCE_DIR}/rtype_client
)
elseif(WIN32)
    add_custom_command(TARGET rtype_client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:rtype_client> ${CMAKE_SOURCE_DIR}/rtype_client.exe
)
endif()