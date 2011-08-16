INCLUDE_DIRECTORIES(src)

DISABLE_COMPILER_WARNINGS()

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
	ADD_LIBRARY(breakpad_client STATIC ${LIB_SRCS})
ELSEIF(MSVC)
	SET(LIB_SRCS
    src/client/windows/crash_generation/crash_generation_client.cc
    src/client/windows/handler/exception_handler.cc
    src/common/convert_UTF.c
    src/common/md5.c
    src/common/string_conversion.cc
    src/common/windows/guid_string.cc
	src/common/windows/string_utils.cc)
	ADD_DEFINITIONS(-DUNICODE)
	ADD_LIBRARY(breakpad_client STATIC ${LIB_SRCS})
ENDIF()


