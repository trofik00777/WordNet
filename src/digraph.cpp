#include "wordnet.h"

#include <sstream>
#include <string>

Digraph::Digraph(std::istream & hypernyms)
{
    for (std::string line; std::getline(hypernyms, line);) {
        std::stringstream line_stream(line);
        ID_type id_set, id_hyp;
        line_stream >> id_set;
        line_stream.ignore();
        auto & hyps = m_graph[id_set];
        while (line_stream >> id_hyp) {
            line_stream.ignore();

            hyps.insert(id_hyp);
        }
    }
}

const std::unordered_set<ID_type> * Digraph::operator[](const ID_type index) const
{
    auto res = m_graph.find(index);
    if (res != m_graph.end()) {
        return &(res->second);
    }
    return nullptr;
}
