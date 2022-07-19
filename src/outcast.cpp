#include "wordnet.h"

#include <vector>

Outcast::Outcast(WordNet & wordnet)
    : wordNet(wordnet)
{
}

std::string Outcast::outcast(const std::set<std::string> & nouns)
{
    bool multi = true;
    unsigned len_to_node = 0;
    const std::string * outcast_node_str;
    std::vector<unsigned> lengths(nouns.size(), 0);
    std::size_t index = 0;
    for (auto first = nouns.begin(), end = nouns.end(); first != end; ++first, ++index) {
        std::size_t index2 = index + 1;
        unsigned prev_len = lengths[index];
        for (auto second = std::next(first); second != end; ++second, ++index2) {
            auto dist = wordNet.distance(*first, *second);
            lengths[index] += dist;
            lengths[index2] += dist;
            if (lengths[index2] > len_to_node) {
                multi = false;
                outcast_node_str = &*second;
                len_to_node = lengths[index2];
            }
            else if (dist != 0 && lengths[index2] == len_to_node) {
                multi = true;
            }
        }
        if (lengths[index] > len_to_node) {
            multi = false;
            outcast_node_str = &*first;
            len_to_node = lengths[index];
        }
        else if (lengths[index] != prev_len && lengths[index] == len_to_node) {
            multi = true;
        }
    }

    if (multi) {
        return "";
    }
    return *outcast_node_str;
}
