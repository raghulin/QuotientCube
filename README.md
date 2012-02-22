About:
============
Quotient Cube tree construction for MySQL tables using C++ and MySQL UDF:

1. Partitions MySQL data into cells of similar dimension values.
2. Constructs the QC tree with pre computed aggregate functions(SUM, COUNT, MIN and MAX) for MySQL base table.
3. Supports MySQL UDF's for range and point queries on the constructed qc tree.

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
	select ccube(D1,D2,D3,M1) from QT;
	select ccube_query("*");

Tests:
============
	CPPUnit tests are included in cppunit folder. Edit the Makefile and change libcppunit.so location to cppunit installation. 
	>make
	>./testcase1

References:
============
	Reference papers for QC Tree are included in include folder.

Sample run:
============
	Following runs are done on un optimized tpch data tables. QC Tree query time include tree deserialization from file for every query.

mysql> select ccube(D1,D2,D3,M1) from TT;
+----------------------+
| ccube(D1,D2,D3,M1)   |
+----------------------+
| -5597912180054818816 |
+----------------------+
1 row in set (10 min 23.09 sec)

mysql> select ccube_query("*");
+---------------------------------------------------------------------------------------------------------------------------------------+
| ccube_query("*")                                                         								|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    
+---------------------------------------------------------------------------------------------------------------------------------------+
| SUM = 257202.000000 Count = 5144347 Min = 0.000000 Max = 0.100000                                                                     |
+---------------------------------------------------------------------------------------------------------------------------------------+
1 row in set (0.03 sec)

mysql> select SUM(M1) from TT;
+-----------------+
| SUM(M1)         |
+-----------------+
| 257202.30999743 |
+-----------------+
1 row in set (0.59 sec)

mysql> select SUM(M1) from TT where D3='TRUCK' and D1='F';
+------------------+
| SUM(M1)          |
+------------------+
| 21401.3300000193 |
+------------------+
1 row in set (1.04 sec)

mysql> select ccube_query("F","*","TRUCK");
+---------------------------------------------------------------------------------------------------------------------------------------+
| ccube_query("F","*","TRUCK")   													|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
+---------------------------------------------------------------------------------------------------------------------------------------+
| SUM = 21401.300000 Count = 427609 Min = 0.000000 Max = 0.100000          				                                |
+---------------------------------------------------------------------------------------------------------------------------------------+
1 row in set (0.02 sec)

mysql> select SUM(M1) from TT where (D3='TRUCK' OR D3='FOB' OR D3='SHIP') and D1='F';
+------------------+
| SUM(M1)          |
+------------------+
| 64262.7499997517 |
+------------------+
1 row in set (1.29 sec)

mysql> select ccube_query("F","*","TRUCK FOB SHIP");
+------------------------------------------------+
| ccube_query("F","*","TRUCK FOB SHIP")                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
+------------------------------------------------+
| SUM = 64262.700000 Count = 1284254 Min = 0.000000 Max = 0.100000
+------------------------------------------------+
1 row in set (0.02 sec)

mysql> select SUM(M1) from TT where (D3='FOB' OR D3='SHIP') and D1='F';
+------------------+
| SUM(M1)          |
+------------------+
| 42861.4199999228 |
+------------------+
1 row in set (1.16 sec)

mysql> select count(*) from TT where (D3='FOB' OR D3='SHIP') and D3='F';
+----------+
| count(*) |
+----------+
|   856645 |
+----------+
1 row in set (1.12 sec)

mysql> select ccube_query("F","*","FOB SHIP");                           
+---------------------------------------------------------------------------------------------------------------------------------------+
| ccube_query("F","*","FOB SHIP")                                                                                        		|                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               
+---------------------------------------------------------------------------------------------------------------------------------------+
| SUM = 42861.400000 Count = 856645 Min = 0.000000 Max = 0.100000                                                                       |
+---------------------------------------------------------------------------------------------------------------------------------------+
1 row in set (0.03 sec)
