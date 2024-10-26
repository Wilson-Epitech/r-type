set(SERVER_SRC
    main.cpp
)

add_executable(rtype_server ${SERVER_SRC})

target_link_libraries(rtype_server PRIVATE rtype_lib)

add_library(server SHARED
    ../handleData/HandleData.cpp
    ../../ecs/components/Components.cpp
)

target_include_directories(server PRIVATE
    ../handleData
    ../../ecs
    ../../ecs/components
    ../../../lib
    ../../../include
)

add_custom_command(TARGET server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:server> ${CMAKE_SOURCE_DIR}/sharedLibraries
)

add_custom_command(TARGET rtype_server POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:rtype_server> ${CMAKE_SOURCE_DIR}/rtype_server
)