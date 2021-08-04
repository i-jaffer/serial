example : example.o serial.o
	gcc $^ -o $@ -L ../lib/ -lpthread -lqueue -I ../include/

example.o : example.c
	gcc -c $^ -o $@ -I ../include/

serial.o : serial.c
	gcc -c $^ -o $@ -I ../include/

libserial.so : serial.c
	gcc -fPIC -shared $^ -o $@ -I ../include/

clean: 
	rm *.o -f
	rm example -f
