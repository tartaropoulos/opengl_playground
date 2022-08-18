# Получить имя папки 
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Копируем шейдера к бинарникам, тк шейдера компилируются во время выполнения
file(COPY 
    simpleShader.vert
    simpleShader.frag 
    uvtemplate.bmp
    uvtemplate.DDS
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${CURRENT_FOLDER_NAME} 
    main.cpp
    ../common/shader.cpp
    ../common/shader.h
    ../common/texture.cpp
    ../common/texture.h)

set(LIBS 
    GLEW::GLEW
    glfw
    ${OPENGL_LIBRARIES})
    
target_link_libraries(${CURRENT_FOLDER_NAME} ${LIBS})