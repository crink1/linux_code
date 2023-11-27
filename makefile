myprocess:code.c
	g++ -o $@ $^
.PHONY:clean
clean:
	rm -f myprocess
