GTEST_LDFLAGS := -lgtest

TEST_SRC     := $(wildcard *.test.cpp)
TEST_OBJS    := $(patsubst %.cpp,%.o,$(TEST_SRC))

LIB_SRC      := $(filter-out %.test.cpp, $(wildcard *.cpp))
LIB_OBJS     := $(patsubst %.cpp,%.o,$(LIB_SRC))
LIB_HEADERS  := $(wildcard *.h)

.o: .cpp $(LIB_HEADERS)
	$(CXX) -c $@ $<

.phony: all check

all: fun_tests

check: fun_tests
	./fun_tests

clean:
	rm -f $(TEST_OBJS)
	rm -f fun_tests*

fun_tests: $(TEST_OBJS) $(LIB_OBJS)
	$(CXX) -o $@ $(TEST_OBJS) $(LIB_OBJS) $(GTEST_LDFLAGS)
