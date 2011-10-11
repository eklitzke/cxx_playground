CFLAGS=-g

uniq: uniq.cc
	$(CXX) $(CFLAGS) -lboost_program_options -o $@ $<

clean:
	-rm -f uniq
