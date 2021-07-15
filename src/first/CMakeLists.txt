# Получить имя папки 
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# Создание название переменной для исходников 
set(CURRENT_SRC ${CURRENT_FOLDER_NAME}_SRC)

# Сохраняем назания всех исходников в одной переменной
file(GLOB CURRENT_SRC "*.cpp" "*.h")

add_executable(${CURRENT_FOLDER_NAME} ${CURRENT_SRC})
target_link_libraries(${CURRENT_FOLDER_NAME} ${ALL_LIBS})