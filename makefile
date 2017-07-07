.PHONY: all compiler clean
stage1exe: driver.c
	gcc -o compiler driver.c	
