idf_component_register(SRCS "main.c" "utils/delay.c"
                    INCLUDE_DIRS ".")

littlefs_create_partition_image(files ../device_files FLASH_IN_PROJECT)

