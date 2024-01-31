d-lib=libtest.so

$(d-lib):add.o sub.o
	gcc -shared -o$@ $^

%.o:%.c
	gcc -fPIC -c $<

.PHONY:clean
clean:
	rm -rf *.o *.a