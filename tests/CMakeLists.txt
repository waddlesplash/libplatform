######
## testutils
######
set(TESTUTILS_SRCS testutils.c testutils.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

######
## libplatform tests
######
add_executable(platform_test ${TESTUTILS_SRCS}
  platform_test/test_types.c
  platform_test/test_clock.c
  platform_test/test_glwindow.c
  platform_test/main.c
  )
target_link_libraries(platform_test platform ${OPENGL_gl_LIBRARY})
