# Получить имя папки 
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Создание названия переменной для исходников 
set(CURRENT_SRC ${CURRENT_FOLDER_NAME}_SRC)

# Сохраняем назания всех исходников в одной переменной
file(GLOB CURRENT_SRC "*.cpp" "*.h")

# Сохраняем назания всех шейдеров в одной переменной
file(GLOB CURRENT_SHADERS "*.vert" "*.frag")

# Копируем шейдера к бинарникам, тк шейдера компилируются во время выполнения
file(COPY ${CURRENT_SHADERS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${CURRENT_FOLDER_NAME} ${CURRENT_SRC})

set(LIBS 
    GLEW::GLEW
    glfw
    ${OPENGL_LIBRARIES})
    
target_link_libraries(${CURRENT_FOLDER_NAME} ${LIBS})