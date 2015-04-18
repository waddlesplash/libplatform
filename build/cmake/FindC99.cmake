######
## C99 detection
### Yes, we don't check for "real" C99 here, but only the features we require
### (declaring variables not at the beginning of blocks, declaration in "for"...)
######

set(C99_CFLAGS)
set(C99_TEST_SRC "int main(int argc,char* argv[]){int a;for(int i=0;i<10;i++)a+=i;int b;}")

include(CheckCSourceCompiles)
check_C_source_compiles("${C99_TEST_SRC}" CC_SUPPORTS_C99)

if(NOT CC_SUPPORTS_C99)
  # Try again with the "C99" Cflag set
  set(C99_CFLAGS "-std=c99")
  set(CMAKE_REQUIRED_FLAGS ${C99_CFLAGS})
  check_C_source_compiles("${C99_TEST_SRC}" CC_SUPPORTS_C99_WITH_FLAGS)
  unset(CMAKE_REQUIRED_FLAGS)

  if(NOT CC_SUPPORTS_C99_WITH_FLAGS)
    message(FATAL_ERROR "${CMAKE_C_COMPILER} does not have C99 support!")
  endif()
endif()
