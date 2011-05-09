@cd ..
@del /F /Q doc\html
@doxygen.exe ogdf-doxygen.cfg
copy doc\images\ftv2*.png doc\html\
@cd doc
