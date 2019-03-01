
// TODO: Use boost::program_options ya dingus.

#include "Nucleotyping/Nucleotyping.hpp"

#if defined(NUCLEO_PREDEF_STRING_DEFINITIONS)
char s1[] = NUCLEO_PREDEF_STRING_NUCLEO_PREDEF_STRING_A001;
char s2[] = NUCLEO_PREDEF_STRING_NUCLEO_PREDEF_STRING_A002;
char s3[] = NUCLEO_PREDEF_STRING_NUCLEO_PREDEF_STRING_A003;
char s4[] = NUCLEO_PREDEF_STRING_NUCLEO_PREDEF_STRING_A004;
char s5[] = NUCLEO_PREDEF_STRING_NUCLEO_PREDEF_STRING_A005;
#else
#endif // NUCLEO_PREDEF_STRING_DEFINITIONS


int main(int argc, char *argv[])
{
    bool verbose = false;
    bool findAllMatches = false;
    bool sequenceSet = false;

    char *subsequence = nullptr;

    // TODO: Handle no input case.
    if (argc == 1) { return EXIT_FAILURE; }

    /** Process command line arguments.
     *  
     *  Options:
     *    --all [-a]  Find all sequence matches.
     * 
     */
    for (auto i = 1; i < argc; i++) {
        if (!strcmp("--all", argv[i]) || !strcmp("-a", argv[i])) {
            findAllMatches = true;
        } else if (!strcmp("--verbose", argv[i]) || !strcmp("-v", argv[i])) {
            verbose = true;
        } else if (!strcmp("--help", argv[i]) || !strcmp("-h", argv[i])) {
            // TODO: Help option handler.
            std::cout << "Help will go here one day." << std::endl;
            return EXIT_SUCCESS;
        } else if (!strcmp("--version", argv[i])) {
            // TODO: Version option handler.
            std::cout << "A version number perhaps some day." << std::endl;
            return EXIT_SUCCESS;
        } else if (!strcmp("--subseq", argv[i]) && (i < (argc - 1))) {
            const auto length = strlen(argv[i+1]);

            subsequence = (char *) malloc(sizeof (char) * length);
            memset(subsequence, 0, length);
            strcpy(subsequence, argv[i + 1]);

            sequenceSet = true;

            if (verbose) {
                std::cout << "Subsequence: " << subsequence << std::endl;
                printf("Size of sequence: %d\n", (int) length);
            }
        }
    }

    FILE* inputFile = nullptr;

    /** TODO: Fix command-line argument parsing.
     *  
     *  Open the input file using the last command-line argument as the name
     *  of the file. This is a temporary measure before I fix the command-line
     *  arguments situation.
     * 
     */
    inputFile = fopen(argv[argc - 1], "r");

    if (!inputFile) {
        std::cerr << "Failed to open file: " << argv[argc - 1] << std::endl;

        return EXIT_FAILURE;
    }

    /** Get file size.
     * 
     */
    fseek(inputFile, 0L, SEEK_END);
    const auto size = ftell(inputFile) + 1;
    rewind(inputFile);

    unsigned long FirstLine = 0;
    unsigned long NewLines = 0;

    unsigned long Adenine  = 0;
    unsigned long Cytosine = 0;
    unsigned long Guanine  = 0;
    unsigned long Thymine  = 0;
    unsigned long Uracil   = 0;

    if (!sequenceSet) {
        subsequence = s5;
    }

    char c = fgetc(inputFile);

    /** The first line .fa* gene sequence files have metadata only, so before we
     *  get started in earnest, chomp out the first line by reading character by
     *  character until we reach the new line.
     * 
     */
    if (c == '>') {
        ++NewLines;

        /** Print first new line with line number, otherwise everything will
         *  start from the wrong line.
         * 
         */
        // TODO: Upgrade to C++.
        // if (verbose) { printf("\n%8d  ", NewLines); }

        do {
            // TODO: Upgrade to C++.
            // if (verbose) { printf("%c", c); }
            c = fgetc(inputFile);
        } while (c != '\n');
        
        ++NewLines;

        // TODO: Upgrade to C++.
        // if (verbose) { printf("\n%8lu  ", NewLines); }
    }

    /** DEBUG: Revert changes for production build.
     *  I'm setting the size variable to something lower so that the program
     *  does not have to run all the way through every time.
     * 
     *  variable: N
     *  original: const auto N = size;
     * 
     */
    const unsigned long N = size;

    unsigned long matches = 0;

    unsigned long seqstart = 0;
    unsigned long seqstop = 0;

    bool found = false;

    std::ostringstream results;

    for (unsigned long i = 0; i < N; ++i) {
        c = fgetc(inputFile);

        if (c == '\n') {
            ++NewLines;
            continue;
        }

        if (c == *subsequence) {
            const auto savedState = ftell(inputFile);
            unsigned long subsize = 1;

            seqstart = savedState;

            // DEBUG: Remove print statement
            if (verbose) { printf("[%lu] %c", seqstart, c); }

            for (unsigned long j = 1; j < strlen(subsequence) - i; ++j) {
                char k = fgetc(inputFile);

                if (k == '\n') {
                    k = fgetc(inputFile);
                }

                if (k == subsequence[j]) {
                    // DEBUG: Remove print statement
                    ++subsize;
                    
                    if (verbose) { printf("%c", k); }

                    if (subsize == strlen(subsequence)) {
                        found = true;
                        seqstop = ftell(inputFile);

                        if (verbose) { printf(" DONE.\n"); }

                        break;
                    }

                    continue;
                }

                // DEBUG: Remove print statement
                if (subsize) {
                    if (verbose) { printf("[%c]\n", k); }
                }

                break;
            }

            if (found) {
                results << "  " << ++matches << ": Line " << NewLines << " (" << seqstart << "-" << seqstop << ")\n";

                if (findAllMatches) {
                    found = false;
                    continue;
                } else {
                    /** Otherwise, break out. */
                    break;
                }
            }

            fseek(inputFile, savedState, SEEK_SET);
        }
    }

    std::cout << "\nResults: " << std::endl;
    std::cout << results.str() << std::endl;

    /** Exit unconditionally.
     * 
     */
    return EXIT_SUCCESS;

    // const auto total = Adenine + Cytosine + Guanine + Thymine;
    const auto total = Adenine + Cytosine + Guanine + Thymine + NewLines;

    std::cout << "Total: " << total << std::endl;
    std::cout << "Adenine: " << Adenine << std::endl;
    std::cout << "Cytosine: " << Cytosine << std::endl;
    std::cout << "Guanine: " << Guanine << std::endl;
    std::cout << "Thymine: " << Thymine << std::endl;

    fclose(inputFile);

    return EXIT_SUCCESS;
}
