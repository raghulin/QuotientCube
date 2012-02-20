all:

	gcc -shared -o libccube.so ./src/*.cpp \
		-I./include \
		-Wno-deprecated
		
	gcc -shared -o ccube_udf.so ./src/*.cpp UDF/cube_udf.cpp \
		-I./include \
		-I/usr/local/src/mysql-5.1.49/include/ \
		-Wno-deprecated

clean:
	rm -rf *.so
	rm -rf *.o

