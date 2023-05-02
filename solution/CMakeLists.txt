file(GLOB_RECURSE sources CONFIGURE_DEPENDS
    src/*.c
    include/*.h
)

add_executable(image-transformer ${sources} src/main.c include/bmp.h include/image.h include/image_edit.h src/image.c src/image_edit.c src/bmp.c include/starter_master.h src/starter_master.c)
target_include_directories(image-transformer PRIVATE src include)
