CXX	?= g++
LD	?= ld

CXXXFLAGS  = -Wall -Wextra -Wpedantic -g -I./ 
LDFLAGS = -lbfd

all: stamped traceStamped

stamped: examples/Stamped.cxx
	$(CXX) $(CXXXFLAGS) $< $(LDFLAGS) -o $@

traceStamped: examples/TraceStamped.cxx
	$(CXX) $(CXXXFLAGS) $< $(LDFLAGS) -o $@

.PHONY : clean

clean:
	@rm -f stamped traceStamped