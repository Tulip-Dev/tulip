INCLUDE_DIRECTORIES(src)

IF(LINUX)
  SET(LIB_SRCS
    src/client/linux/crash_generation/crash_generation_client.cc
    src/client/linux/handler/exception_handler.cc
    src/client/linux/minidump_writer/linux_dumper.cc
    src/client/linux/minidump_writer/minidump_writer.cc
    src/client/minidump_file_writer.cc
    src/common/convert_UTF.c
    src/common/md5.c
    src/common/string_conversion.cc
    src/common/linux/file_id.cc
    src/common/linux/guid_creator.cc)
ELSE(WIN32)

ENDIF(LINUX)

ADD_LIBRARY(breakpad_client STATIC ${LIB_SRCS})
