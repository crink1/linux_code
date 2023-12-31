mycode:code.c
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f mycode
