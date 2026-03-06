cmake_minimum_required(VERSION 3.15)

project(bme280_driver)

add_library(${PROJECT_NAME} STATIC
	"src/bme280_driver.c"
)

# добавляет include в пути для поиска заголовочных файлов. За счет этого не надо писать полный абсолютный или относительный путь.
target_include_directories(${PROJECT_NAME} PUBLIC
	"include"
)