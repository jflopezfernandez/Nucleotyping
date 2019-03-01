
#include "Nucleotyping/Nucleotyping.hpp"


int main(int argc, char *argv[])
{
    // TODO: Handle no input case.
    if (argc == 1) { return EXIT_FAILURE; }

    FILE* inputFile = fopen(argv[1], "r");

    /** Get file size.
     * 
     */
    fseek(inputFile, 0L, SEEK_END);
    const auto size = ftell(inputFile);
    rewind(inputFile);

    unsigned long FirstLine = 0;
    unsigned long NewLines = 0;

    unsigned long Adenine  = 0;
    unsigned long Cytosine = 0;
    unsigned long Guanine  = 0;
    unsigned long Thymine  = 0;

    char c = fgetc(inputFile);

    // The first line .fa* gene sequence files have metadata only, so before we
    // get started in earnest, chomp out the first line by reading character by
    // character until we reach the new line.
    if (c == '>') {
        do {
            ++FirstLine;
            c = fgetc(inputFile);
        } while (c != '\n');
        ++FirstLine;
    }

    std::cout << "File Size: " << size - FirstLine << std::endl;

    // Spit out the last new line.
    c = fgetc(inputFile);

    while (c = fgetc(inputFile)) {
        if (c == EOF) { break; }

        // printf("%c", c);

        if (c == 'A') {
            ++Adenine;
        } else if (c == 'C') {
            ++Cytosine;
        } else if (c == 'G') {
            ++Guanine;
        } else if (c == 'T') {
            ++Thymine;
        } else if (c == '\n') {
            ++NewLines;
        } else {
            printf("%c", c);
        }
    }

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
