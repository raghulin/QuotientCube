How to build:
============
	>make 
	NOTE: edit Makefile and change include directory for MySQL

Install run:
============
	cp cube_udf.so /usr/lib/mysql/plugin

	#DROP FUNCTION ccube_query;
	CREATE FUNCTION ccube_query RETURNS STRING SONAME 'ccube_udf.so';
	#DROP FUNCTION ccube;
	CREATE AGGREGATE FUNCTION ccube RETURNS INTEGER SONAME 'ccube_udf.so';
	select ccube(M1,M2,M3,D) from QT;
	select ccube_query("*");

Tests:
============
	CPPUnit tests are included in cppunit folder.

References:
============
	Reference papers for QC Tree are included in include folder.


