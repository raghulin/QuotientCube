all:

	g++ -shared -o libccube.so ./src/*.cpp \
		-I./include \
		-Wno-deprecated
		
	g++ -shared -o ccube_udf.so ./src/*.cpp UDF/cube_udf.cpp \
		-I./include \
		-I/usr/local/src/mysql-5.6.10/include/ \
		-Wno-deprecated

clean:
	rm -rf *.so
	rm -rf *.o

