CXX	?= g++
LD	?= ld

CXXXFLAGS  = -Wall -Wextra -Wpedantic -g -I./ 

all: extractTypes

extractTypes: examples/extractTypes.cxx
	$(CXX) $(CXXXFLAGS) $< $(LDFLAGS) -o $@

.PHONY : clean

clean:
	@rm -f extractTypes
