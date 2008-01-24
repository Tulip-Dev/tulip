noinst_LTLIBRARIES = libgpugraph.la

INCLUDES = @GL_INCLUDES@
AM_LDFLAGS = @GL_LDFLAGS@ @LIB_GL@ 
			   

libgpugraph_la_SOURCES = libgpugraph.cpp

fragmentShaderHeaderSource.h: libgpugraph.glsl
	sh glsl_to_cpp.sh $< > $@

libgpugraph.cpp: fragmentShaderHeaderSource.h

clean-generic:
	rm -f fragmentShaderHeaderSource.h
