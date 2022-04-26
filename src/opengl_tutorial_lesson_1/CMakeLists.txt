# Получить имя папки 
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(${CURRENT_FOLDER_NAME} 
    main.cpp)

set(LIBS 
    GLEW::GLEW
    glfw
    ${OPENGL_LIBRARIES})
    
target_link_libraries(${CURRENT_FOLDER_NAME} ${LIBS})