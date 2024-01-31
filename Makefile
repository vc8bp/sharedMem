COMPILER = g++
LDLIBS += -lboost_system -lboost_thread -lboost_filesystem
LDFLAGS += -std=c++17 -lstdc++ 

read:reader.o
	clear
	@echo "---------OUTPUT----------"
	@./reader

wri:writer.o
	clear
	@echo "---------OUTPUT----------"
	@./writer

reader.o: reader.cpp 
	@echo "Compiling reader.cpp"
	${COMPILER} $< -o $@ $ ${LDLIBS} ${LDFLAGS}
	chmod +x $@



writer.o: writer.cpp 
	echo "Compiling writer.cpp"
	${COMPILER} $< -o $@ $ ${LDLIBS} ${LDFLAGS}
	chmod +x $@

clean: 
	rm *.o

