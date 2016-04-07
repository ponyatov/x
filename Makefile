log.log: ./exe.exe src.src ./test.exe
	./exe.exe < src.src > log.log && tail $(TAIL) log.log
	./test.exe
C = cpp.cpp ypp.tab.cpp lex.yy.c
H = hpp.hpp ypp.tab.hpp
CXXFLAGS = -std=gnu++11
./exe.exe: $(C) $(H) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $(C)
./test.exe: test.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<	
ypp.tab.cpp: ypp.ypp
	bison $<
lex.yy.c: lpp.lpp
	flex $<	