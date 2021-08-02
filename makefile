example : example.o serial.o
	gcc $^ -o $@

example.o : example.c
	gcc -c $^ -o $@

serial.o : serial.c
	gcc -c $^ -o $@

libserial.so : serial.c
	gcc -fPIC -shared $^ -o $@

clean: 
	rm *.o -f
	rm example -f
