set(SERVER_SRC
    Server.cpp
    main.cpp
)

add_executable(rtype_server ${SERVER_SRC})

target_link_libraries(rtype_server PRIVATE rtype_lib raylib asio glfw)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(rtype_server
        PRIVATE "-framework OpenGL"
        PRIVATE "-framework Cocoa"
        PRIVATE "-framework IOKit"
        PRIVATE "-framework CoreVideo"
        PRIVATE "-framework GLUT"
    )
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(rtype_server
        PRIVATE GL m pthread dl rt X11
    )
else()
    message(FATAL_ERROR "Unsupported OS")
endif()
