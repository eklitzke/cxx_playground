/* a simple, non-comprehensive uniq(1) command */

#include <iostream>
#include <fstream>
#include <string>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

/* get EXIT_SUCCESS, EXIT_FAILURE */
#include <stdlib.h>

/* Display a value; this is either just the unique key, or the unique key and
 * the count
 */
static inline void
display_val(std::string key, unsigned int count, bool show_count) {
  if (show_count) {
	std::cout << count << " ";
  }
  std::cout << key << std::endl;
}

/* Uniquify a file */
static int
uniq_file(std::string fname, bool show_count) {
  std::ifstream infile(fname.c_str(), std::ios::in);
  if (!infile) {
	std::cerr << "no such file \"" << fname << "\"" << std::endl;
	return EXIT_FAILURE;
  }
  unsigned int count = 0;
  std::string prev = std::string("");
  for (std::string line; std::getline(infile, line); ) {
	if (prev != line && prev != "") {
	  display_val(prev, count, show_count);
	  count = 1;
	} else {
	  count++;
	}
	prev = line;
  }
  display_val(prev, count, show_count);
  return EXIT_SUCCESS;
}

int main(int argc, char **argv) {
  po::options_description generic("Generic options");
  generic.add_options()
	("help,h", "print help message")
	("count,c", "count input")
	;

  po::options_description hidden("Hidden options");
  hidden.add_options()
	("input-file", po::value< std::vector<std::string> >(), "input file")
	;

  po::positional_options_description p;
  p.add("input-file", -1);

  po::options_description cmdline_options;
  cmdline_options.add(generic).add(hidden);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
			options(cmdline_options).positional(p).run(), vm);

  if (vm.count("help")) {
	std::cout << generic << std::endl;
	return EXIT_FAILURE;
  }

  bool show_count = (bool) vm.count("count");

  std::vector<std::string> fnames = vm["input-file"].as< std::vector<std::string> >();
  std::vector<std::string>::iterator it;
  for (it = fnames.begin(); it < fnames.end(); it++) {
	int ret = uniq_file(*it, show_count);
	if (ret != EXIT_SUCCESS) {
	  return ret;
	}
  }
  return EXIT_SUCCESS;
}
