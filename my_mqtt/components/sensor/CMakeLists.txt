#file(GLOB_RECURSE srcs "src/*.c")

#set(EXTRA_COMPONENT_DIRS ${PROJECT_DIR}/extras/components)

idf_component_register(SRCS "src/app_temp.c"
                       INCLUDE_DIRS "include"
                       REQUIRES dht )
