#include "wordnet.h"

#include <sstream>
#include <string>

WordNet::WordNet(std::istream & synsets, std::istream & hypernyms) // NOLINT "error: 'hypernyms' can have 'const' qualifier [const-param]"
    : m_sca(Digraph(hypernyms))
{
    for (std::string line; std::getline(synsets, line);) {
        std::stringstream line_stream(line);
        ID_type id_set;
        line_stream >> id_set;
        line_stream.ignore();

        std::string word;
        std::getline(line_stream, word, ',');
        std::getline(line_stream, m_glosses[id_set]); // (, delim=' ')

        std::stringstream words_stream(word);
        while (std::getline(words_stream, word, ' ')) {
            m_words[std::move(word)].insert(id_set);
        }
    }
}

WordNet::Nouns WordNet::nouns() const
{
    return Nouns(*this);
}

bool WordNet::is_noun(const std::string & word) const
{
    return m_words.find(word) != m_words.end();
}

std::string WordNet::sca(const std::string & noun1, const std::string & noun2) const
{
    return m_glosses.at(m_sca.ancestor_subset(m_words.at(noun1), m_words.at(noun2)));
}

unsigned int WordNet::distance(const std::string & noun1, const std::string & noun2) const
{
    return m_sca.length_subset(m_words.at(noun1), m_words.at(noun2));
}
