uniq: uniq.cpp
	$(CXX) $(CFLAGS) -lboost_program_options -o $@ $<
