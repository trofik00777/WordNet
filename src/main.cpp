#include <fstream>
#include <iostream>
#include "wordnet.h"

int main()
{
    std::ifstream synsets{"etc/synsets.txt"}, hypernyms{"etc/hypernyms.txt"};

    std::cout << synsets.is_open() << "\n";

    WordNet wordnet(synsets, hypernyms);
    Outcast outcast(wordnet);
    std::cout << outcast.outcast({"horse", "zebra", "cat", "bear", "table"}) << std::endl; // table

    return 0;
}
