cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(ki-deck)

littlefs_create_partition_image(files device_files FLASH_IN_PROJECT)

