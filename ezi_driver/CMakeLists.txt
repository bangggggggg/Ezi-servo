set(srcs "src/Comm.c" "src/CommInterface.c")

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS "include"
                       REQUIRES driver

                    )
