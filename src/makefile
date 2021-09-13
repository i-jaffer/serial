example : example.o serial.o
	gcc $^ -o $@ -L ../lib/ -lpthread -I ../include/

example.o : example.c
	gcc -c $^ -o $@ -I ../include/

serial.o : serial.c
	gcc -c $^ -o $@ -I ../include/

lib : libserial_arm64.so libserial_arm.so libserial.so


libserial_arm64.so : serial.c
	aarch64-linux-gnu-gcc -fPIC -shared $^ -o $@ -I ../include/

libserial_arm.so : serial.c
	arm-linux-gnueabihf-gcc -std=c99 -fPIC -shared $^ -o $@ -I ../include/

libserial.so : serial.c
	gcc -fPIC -shared $^ -o $@ -I ../include/

clean: 
	rm *.o -f
	rm example -f
	rm *.so -f
