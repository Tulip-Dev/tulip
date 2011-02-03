ADD_CUSTOM_TARGET(common_copy ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

#INSTALL(FILES tulip-default.css tulip-doxygen.css closed.gif opened.gif
#  favicon.ico 1.png 2.png 3.png 4.png 5.png 6.png 7.png 8.png 9.png
#  10.png logo_bg.png logo-icon.png logo.png
#DESTINATION share/tulip/common
#COMPONENT tulip_doc)
