fsh:
	find **/*.c *.c > sources; gcc -Wall -g @sources -o fsh -lreadline; rm sources

clean: 
	rm -f fsh 

doc:
	doxygen Doxyfile
