/**
 * @file main.cpp
 * @author EA
 */

#include <string>
#include <unordered_map>
#include <filesystem>
#include <getopt.h>

#include "compressUnordering.hpp"
#include "reorderingHyperedges.hpp"
#include "reorderingVertices.hpp"
#include "reorderingVerticesHyperedges.hpp"
#include "query.hpp"

using namespace std;
using namespace std::filesystem;

// used to convert the default values of the compression to a string
#define STRINGIFY( x) #x
#define STR(x) STRINGIFY(x)

#define TYPE_UNORDER "unorder"
#define TYPE_REORDER_HYPEREDGES "reH"
#define TYPE_REORDER_VERTICES "reV"
#define TYPE_REORDER_VERTICES_HYPEREDGES "reVH"
#define TYPE_INCIDENCE_LIST "inc"


static void print_usage(bool error) {
	static const char* usage_str = \
	"Usage: reordering-cli\n"
	"    -h    show this help\n"
	"\n"
	" * to compress a hypergraph:\n"
    "   reordering-cli -i [input_file] -o [output] -t {" TYPE_UNORDER "|" TYPE_REORDER_HYPEREDGES "|" TYPE_REORDER_VERTICES "|" TYPE_REORDER_VERTICES_HYPEREDGES "} -x [temp_dir]\n"
    " * to read a hypergraph:\n"
    "   reordering-cli -i [input] -t {" TYPE_UNORDER "|" TYPE_REORDER_HYPEREDGES "|" TYPE_REORDER_VERTICES "|" TYPE_REORDER_VERTICES_HYPEREDGES "|" TYPE_INCIDENCE_LIST "} -q [query_file] \n"

	;
	FILE* os = error ? stderr : stdout;
	fprintf(os, "%s", usage_str);
}


#define check_mode(mode_compress, mode_read, compress_expected, option_name) \
do { \
    if(compress_expected) { \
        if(mode_read) { \
            fprintf(stderr, "option '-%s' not allowed when compressing\n", option_name); \
            return -1; \
        } \
        mode_compress = true; \
    } \
    else { \
        if(mode_compress) { \
            fprintf(stderr, "option '-%s' not allowed when reading the compressed graph\n", option_name); \
            return -1; \
        } \
        mode_read = true; \
    } \
} while(0)


int main(int argc, char** argv) {
	if(argc <= 1) {
		print_usage(true);
		return EXIT_FAILURE;
	}

    int opt;
    std::string input_file;
    std::string output_file;
    std::string temp_dir;
    std::string node_query;
    std::string test_file;
    int type = 0;
    bool mode_compress = false;
    bool mode_read = false;
    while ((opt = getopt(argc, argv, "hi:o:t:x:q:")) != -1) {
        switch (opt) {
            case 'i':
                input_file = optarg;
                if (!exists(input_file)) {
                    printf("Invalid input file.");
                    return EXIT_FAILURE;
                }
                break;
            case 'o':
                check_mode(mode_compress, mode_read, true, "o");
                output_file = optarg;
                if (output_file.empty()) {
                    printf("Output file not specified correctly.");
                    return EXIT_FAILURE;
                }
                break;
            case 'x':
                check_mode(mode_compress, mode_read, true, "x");
                temp_dir = optarg;
                if (temp_dir.empty()) {
                    printf("Tempdir not specified correctly.");
                    return EXIT_FAILURE;
                }
                break;
            case 't':
                if (std::string(optarg) == TYPE_UNORDER)
                    type = 0;
                else if (std::string(optarg) == TYPE_REORDER_HYPEREDGES)
                    type = 1;
                else if (std::string(optarg) == TYPE_REORDER_VERTICES)
                    type = 2;
                else if (std::string(optarg) == TYPE_REORDER_VERTICES_HYPEREDGES)
                    type = 3;
                else if (std::string(optarg) == TYPE_INCIDENCE_LIST)  {
                    check_mode(mode_compress, mode_read, false, "t");
                    type = 4;
                }
                else {
                    printf("Invalid type Type.");
                    print_usage(true);
                    return EXIT_FAILURE;
                }
                break;
            case 'q':
                check_mode(mode_compress, mode_read, false, "q");
                test_file = optarg;
                if (!exists(test_file)) {
                    printf("Invalid test file.");
                    return EXIT_FAILURE;
                }
                break;
            case 'h':
            default:
                print_usage(true);
                return EXIT_SUCCESS;
        }
    }

    if (mode_compress) {
        switch (type) {
            case 0:
                solve_compress_unordering(input_file, output_file);
                break;
            case 1:
                solve_reordering_hyperedges(input_file, output_file, true);
                break;
            case 2:
                solve_reordering_vertices(input_file, output_file, true);
                break;
            case 3:
                std::string unc_hyp = temp_dir + "/temp_unc_hyp";
                std::string unc_ver = temp_dir + "/temp_unc_ver";
                solve_reordering_hyperedges(input_file, unc_hyp, false);
                solve_reordering_vertices(input_file, unc_ver, false);
                solve_reordering_vertices_hyperedges(input_file, unc_ver, unc_hyp, output_file);
                break;
        }
    }
    if (mode_read) {
        if (type == 4)
        {
            perform_query(input_file, test_file, 1);
        }
        else
        {
            perform_query(input_file, test_file, 0);
        }
    }
    return EXIT_SUCCESS;
}
