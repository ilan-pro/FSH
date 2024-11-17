fsh:
	find **/*.c *.c > sources; gcc -Wall -g @sources -o fsh -lreadline; rm source

clean: 
	rm -f fsh 

doc:
	doxygen Doxyfile
