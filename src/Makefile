

TARGET = ../sbox
objects =  allowed_syscall.o lang_base.o main.o \
		   lang_c.o lang_cpp.o lang_java.o
ARGS = -DDEBUG
#ARGS = 

$(TARGET) : $(objects)
	g++ -o $(TARGET) $(objects) $(ARGS)

lang_base.o : lang_base.cpp lang_base.h allowed_syscall.h
	g++ -c lang_base.cpp $(ARGS)

lang_java.o : lang_java.cpp lang_java.h
	g++ -c lang_java.cpp $(ARGS)

lang_c.o : lang_c.cpp lang_c.h
	g++ -c lang_c.cpp $(ARGS)

lang_cpp.o : lang_cpp.cpp lang_cpp.h
	g++ -c lang_cpp.cpp $(ARGS)

main.o : main.cpp
	g++ -c main.cpp $(ARGS)

allowed_syscall.o : allowed_syscall.cpp allowed_syscall.h
	g++ -c allowed_syscall.cpp $(ARGS)

.PHONY: clean
clean:
	rm -f $(objects) $(TARGET)
