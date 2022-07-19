#include "wordnet.h"

#include <limits>
#include <queue>

ShortestCommonAncestor::ShortestCommonAncestor(const Digraph & dg)
    : m_graph(dg)
{
}

ShortestCommonAncestor::ShortestCommonAncestor(Digraph && dg2)
    : m_graph(std::move(dg2))
{
}

std::pair<ID_type, unsigned> ShortestCommonAncestor::get_ancestor(const std::set<ID_type> & subset_a, const std::set<ID_type> & subset_b) const
{
    std::unordered_map<ID_type, std::array<unsigned, 2>> lengths; // <id_node : [len_to_set_a, len_to_set_b]>

    std::queue<std::pair<ID_type, char>> queue; // <id_node, 0/1 (0 - set_a, 1 - set_b)>
    const unsigned infinity = std::numeric_limits<unsigned>::max();
    std::pair<ID_type, unsigned> answer{0, infinity};

    for (ID_type id_a : subset_a) {
        queue.emplace(id_a, 0);
        lengths.emplace(id_a, std::array<unsigned, 2>{0, infinity});
    }
    for (ID_type id_b : subset_b) {
        queue.emplace(id_b, 1);
        if (!lengths.emplace(id_b, std::array<unsigned, 2>{infinity, 0}).second) {
            return {id_b, 0};
        }
    }
    while (!queue.empty()) {
        auto [elem, mode_subset] = queue.front();
        queue.pop();
        unsigned current_length = lengths[elem][mode_subset];

        auto neighbours = m_graph[elem];
        if (neighbours == nullptr) {
            continue;
        }
        std::array<unsigned, 2> arr;
        arr[mode_subset] = current_length + 1;
        arr[1 - mode_subset] = infinity;
        for (const ID_type neighbour : *neighbours) {
            const auto [found, inserted] = lengths.emplace(neighbour, arr);

            if (!inserted) {
                auto & neig_lengths = found->second;
                neig_lengths[mode_subset] = std::min(current_length + 1, neig_lengths[mode_subset]);
                if (neig_lengths[1 - mode_subset] < infinity && neig_lengths[0] + neig_lengths[1] < answer.second) {
                    answer = {neighbour, neig_lengths[0] + neig_lengths[1]};
                }
            }
            else {
                queue.emplace(neighbour, mode_subset);
            }
        }
    }

    return answer;
}

unsigned int ShortestCommonAncestor::ancestor(unsigned int v, unsigned int w) const
{
    return this->get_ancestor({v}, {w}).first;
}

unsigned int ShortestCommonAncestor::length(unsigned int v, unsigned int w) const
{
    return this->get_ancestor({v}, {w}).second;
}

unsigned int ShortestCommonAncestor::ancestor_subset(const std::set<unsigned int> & subset_a, const std::set<unsigned int> & subset_b) const
{
    return this->get_ancestor(subset_a, subset_b).first;
}

unsigned int ShortestCommonAncestor::length_subset(const std::set<unsigned int> & subset_a, const std::set<unsigned int> & subset_b) const
{
    return this->get_ancestor(subset_a, subset_b).second;
}
