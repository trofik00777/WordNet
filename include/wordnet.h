#pragma once

#include <iosfwd>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

using ID_type = unsigned;

class Digraph
{
private:
    std::unordered_map<ID_type, std::unordered_set<ID_type>> m_graph;

public:
    explicit Digraph(std::istream & hypernyms);

    const std::unordered_set<ID_type> * operator[](ID_type index) const;
};

class ShortestCommonAncestor
{
private:
    const Digraph m_graph;

    /// algorithm like dijkstra
    /// return (ID_of_sca, length_subsetA->sca->subsetB)
    std::pair<ID_type, unsigned> get_ancestor(const std::set<ID_type> & subset_a, const std::set<ID_type> & subset_b) const;

public:
    explicit ShortestCommonAncestor(const Digraph & dg);
    explicit ShortestCommonAncestor(Digraph && dg2);

    // calculates length of shortest common ancestor path from node with id 'v' to node with id 'w'
    unsigned length(unsigned v, unsigned w) const;

    // returns node id of shortest common ancestor of nodes v and w
    unsigned ancestor(unsigned v, unsigned w) const;

    // calculates length of shortest common ancestor path from node subset 'subset_a' to node subset 'subset_b'
    unsigned length_subset(const std::set<unsigned> & subset_a, const std::set<unsigned> & subset_b) const;

    // returns node id of shortest common ancestor of node subset 'subset_a' and node subset 'subset_b'
    unsigned ancestor_subset(const std::set<unsigned> & subset_a, const std::set<unsigned> & subset_b) const;
};

class WordNet
{
    using WordIDs = std::unordered_map<std::string, std::set<ID_type>>;
    using GlossesIDs = std::unordered_map<ID_type, std::string>;

public:
    WordNet(std::istream & synsets, std::istream & hypernyms);

    /**
     * Simple proxy class used to enumerate nouns.
     *
     * Usage example:
     *
     * WordNet wordnet{...};
     * ...
     * for (const std::string & noun : wordnet.nouns()) {
     *     // ...
     * }
     */
    class Nouns
    {
    private:
        const WordNet & m_wordNet;

    public:
        class iterator
        {
        private:
            using base_iterator = WordIDs::const_iterator;

            base_iterator m_iter;

        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::string;
            using difference_type = typename base_iterator::difference_type;
            using pointer = const std::string *;
            using reference = const std::string &;

            iterator() = default;
            iterator(const base_iterator & iter)
                : m_iter(iter)
            {
            }

            iterator & operator++()
            {
                ++m_iter;
                return *this;
            }
            iterator operator++(int)
            {
                auto tmp = *this;
                ++m_iter;
                return tmp;
            }
            reference operator*() const
            {
                return m_iter->first;
            }
            pointer operator->() const
            {
                return &(m_iter->first);
            }
            bool operator==(const iterator & other) const
            {
                return m_iter == other.m_iter;
            }
            bool operator!=(const iterator & other) const
            {
                return m_iter != other.m_iter;
            }
        };

        Nouns(const WordNet & wordNet)
            : m_wordNet(wordNet)
        {
        }

        iterator begin() const
        {
            return iterator(m_wordNet.m_words.begin());
        }
        iterator end() const
        {
            return iterator(m_wordNet.m_words.end());
        }
    };

    // lists all nouns stored in WordNet
    Nouns nouns() const;

    // returns 'true' iff 'word' is stored in WordNet
    bool is_noun(const std::string & word) const;

    // returns gloss of "shortest common ancestor" of noun1 and noun2
    std::string sca(const std::string & noun1, const std::string & noun2) const;

    // calculates distance between noun1 and noun2
    unsigned distance(const std::string & noun1, const std::string & noun2) const;

private:
    WordIDs m_words;
    GlossesIDs m_glosses;
    ShortestCommonAncestor m_sca;
};

class Outcast
{
private:
    WordNet & wordNet;

public:
    explicit Outcast(WordNet & wordnet);

    // returns outcast word
    std::string outcast(const std::set<std::string> & nouns);
};
