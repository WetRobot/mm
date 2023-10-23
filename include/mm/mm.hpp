#ifndef MM_HPP
#define MM_HPP

#include <iostream>
#include <unordered_map>
#include <string>
#include <tuple>
#include <cmath>
#include <vector>
#include <omp.h>

namespace mm {

#include "./print.hpp"

    template <typename mm_elem_type>
    class MarkovModel {
    private:
        typedef std::tuple<const mm_elem_type, const mm_elem_type> mm_transition_type;
        typedef std::unordered_map<mm_elem_type, int> rhs_map_type;
        typedef std::unordered_map<mm_elem_type, rhs_map_type> lhs_rhs_counts_type;
        lhs_rhs_counts_type lhs_rhs_counts; // contains e.g. count for 0 -> 1
        rhs_map_type lhs_counts; // constains e.g. count for 0 -> any

        int n_unique_rhs_states = 0; // number of unique rhs states seen

        template <typename KeyType, typename ValueType>
        ValueType get_value_or_default(
            const std::unordered_map<KeyType,
            ValueType>& map,
            const KeyType& key,
            const ValueType& default_value
        ) const {
            auto it = map.find(key);
            if (it != map.end()) {
                return it->second;
            }
            return default_value;
        }

        void transition_count_increment(
            const mm_transition_type& transition,
            const int& n_obs = 1
        ) {
            const mm_elem_type& lhs = std::get<0>(transition);
            const mm_elem_type& rhs = std::get<1>(transition);
            transition_count_increment(lhs, rhs, n_obs);
        }
        void transition_count_increment(
            const mm_elem_type& lhs,
            const mm_elem_type& rhs,
            const int& n_obs = 1
        ) {
            auto lhs_it = lhs_rhs_counts.find(lhs);
            if (lhs_it != lhs_rhs_counts.end()) {
                // this lhs has been seen before
                lhs_counts[lhs] += n_obs;  // Increment lhs count
                auto rhs_it = lhs_it->second.find(rhs);
                if (rhs_it != lhs_it->second.end()) {
                    // found rhs
                    rhs_it->second++;  // Increment rhs count
                } else {
                    // did not find rhs
                    lhs_it->second[rhs] = n_obs;
                    n_unique_rhs_states += n_obs;
                }
            } else {
                // this lhs has NOT been seen before
                lhs_counts[lhs] = n_obs;
                lhs_rhs_counts[lhs][rhs] = n_obs;  // Initialize rhs count
                n_unique_rhs_states += n_obs;
            }
        }

        int n;
        double alpha;
        double unseen_alpha;
        bool normalise_length;

    public:
        MarkovModel(
            double alpha = 1.0,
            double unseen_alpha = 1.0,
            bool normalise_length = false
        ) : alpha(alpha), unseen_alpha(unseen_alpha), normalise_length(normalise_length) {}

        MarkovModel(
            std::vector<mm_elem_type> lhs_space,
            std::vector<mm_elem_type> rhs_space,
            double alpha = 1.0,
            double unseen_alpha = 1.0,
            bool normalise_length = false
        ) : alpha(alpha), unseen_alpha(unseen_alpha), normalise_length(normalise_length) {
            std::size_t n_transitions = lhs_space.size();
            for (std::size_t i = 0; i < n_transitions; i++) {
                transition_count_increment(
                    lhs_space[i],
                    rhs_space[i],
                    0
                );
            }
        }

        MarkovModel(
            std::vector<mm_transition_type> transition_space,
            double alpha = 1.0,
            double unseen_alpha = 1.0,
            bool normalise_length = false
        ) : alpha(alpha), unseen_alpha(unseen_alpha), normalise_length(normalise_length) {
            std::size_t n_transitions = transition_space.size();
            for (std::size_t i = 0; i < n_transitions; i++) {
                transition_count_increment(
                    transition_space[i],
                    0
                );
            }
        }

        void print() const {
            std::cout << "n: ";
            print::print(n);
            std::cout << "alpha: ";
            print::print(alpha);
            std::cout << "unseen_alpha: ";
            print::print(unseen_alpha);
            std::cout << "transition counts:" << std::endl;
            print::__PL_UP__();
            print::print(lhs_rhs_counts);
            print::__PL_DOWN__();
        }

        void update(            
            const mm_transition_type& transition
        ) {
            transition_count_increment(transition);
        }
        void update(            
            const mm_elem_type& lhs,
            const mm_elem_type& rhs
        ) {
            transition_count_increment(lhs, rhs);
        }
        
        void transition_count_get(
            const mm_elem_type& lhs,
            const mm_elem_type& rhs,
            int& out
        ) const {
            auto lhs_it = lhs_rhs_counts.find(lhs);
            if (lhs_it == lhs_rhs_counts.end()) {
                out = 0;
                return;
            }
            rhs_map_type rhs_map = lhs_it->second;
            auto rhs_it = rhs_map.find(rhs);
            if (rhs_it == rhs_map.end()) {
                out = 0;
                return;
            }
            out = rhs_it->second;
        }
        int transition_count_get(
            const mm_elem_type& lhs,
            const mm_elem_type& rhs
        ) const {
            int out = 0;
            transition_count_get(lhs, rhs, out);
            return(out);
        }

        void lpmf(
            const mm_elem_type& lhs,
            const mm_elem_type& rhs,
            double& out
        ) const {
            double a = transition_count_get(lhs, rhs);
            if (a == 0.0) {
                a += unseen_alpha;
            } else {
                a += alpha;
            }
            double b = 0.0;
            b += get_value_or_default(lhs_counts, lhs, 0);
            b += n_unique_rhs_states * alpha + unseen_alpha;
            out = log(a) - log(b);
        }
        double lpmf(
            const mm_elem_type& lhs,
            const mm_elem_type& rhs
        ) const {
            double out = 0.0;
            lpmf(lhs, rhs, out);
            return(out);
        }
        
        void lpmf(
            const mm_transition_type& transition,
            double& out
        ) const {
            lpmf(std::get<0>(transition), std::get<1>(transition), out);
        }
        void lpmf(
            const mm_transition_type& transition
        ) const {
            return lpmf(std::get<0>(transition), std::get<1>(transition));
        }

    };

}

#endif
