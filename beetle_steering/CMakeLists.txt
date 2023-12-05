set(srcs "src/beetle_steering.c")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "include"
                       REQUIRES ezi_driver
                       )
