#ifndef NGM_HPP
#define NGM_HPP

#include <iostream>
#include <unordered_map>
#include <string>
#include <tuple>
#include <cmath>
#include <vector>
#include <omp.h>

namespace ngram {

#include "./print.hpp"

    class NgramModel {
    private:
        typedef std::tuple<std::string, std::string> ngram_type;
        typedef std::unordered_map<std::string, int> rhs_map_type;
        typedef std::unordered_map<std::string, rhs_map_type> lhs_map_type;
        lhs_map_type ngram_counts;

        void ngram_count_increment(
            const ngram_type& ngram
        ) {
            const std::string& lhs = std::get<0>(ngram);
            const std::string& rhs = std::get<1>(ngram);
            ngram_count_increment(lhs, rhs);
        }
        void ngram_count_increment(
            const std::string& lhs,
            const std::string& rhs
        ) {
            auto& lhs_map = ngram_counts;
            auto lhs_it = lhs_map.find(lhs);
            rhs_map_type rhs_map;
            // rhs_map[""] = 0;
            if (lhs_it != lhs_map.end()) {
                // found lhs
                rhs_map = lhs_it->second;
            } else {
                n_lhs += 1;
            }
            auto rhs_it = rhs_map.find(rhs);
            if (rhs_it != rhs_map.end()) {
                // found rhs
                rhs_map[rhs]++;
            } else {
                // did not find rhs
                rhs_map[rhs] = 1;
                n_rhs += 1;
            }
            rhs_map[""]++;
            lhs_map[lhs] = rhs_map;
        }
    public:
        int n;
        double alpha;
        double unseen_alpha;
        bool normalise_length;

        int n_lhs = 0;
        int n_rhs = 0;

        NgramModel(
            int n = 3,
            double alpha = 1.0,
            double unseen_alpha = 1.0,
            bool normalise_length = true
        ) : n(n), alpha(alpha), unseen_alpha(unseen_alpha), normalise_length(normalise_length) {}

        void print() const {
            std::cout << "n: ";
            print::print(n);
            std::cout << "alpha: ";
            print::print(alpha);
            std::cout << "unseen_alpha: ";
            print::print(unseen_alpha);
            std::cout << "ngram counts:" << std::endl;
            print::__PL_UP__();
            print::print(ngram_counts);
            print::__PL_DOWN__();
        }

        void update(            
            const ngram_type& ngram
        ) {
            ngram_count_increment(ngram);
        }
        void update(            
            const std::string& lhs,
            const std::string& rhs
        ) {
            ngram_count_increment(lhs, rhs);
        }
        void update(
            const std::string& text
        ) {
            const std::size_t n_ngrams = text.size() - n + 1;
            for (int i = 0; i < n_ngrams; i++) {
                // e.g. n = 3, text = "howdy", i = 1:
                // lhs = "ow", rhs = "d"
                const std::string& lhs = text.substr(i, n - 1);
                const std::string& rhs = text.substr(i + n - 1, 1);
                update(lhs, rhs);
            }
        }
        void update(
            const std::vector<std::string> texts
        ) {
            for (const auto& text : texts) {
                update(text);
            }
        }
        
        int ngram_count_get(
            const ngram_type& ngram
        ) const {
            const std::string& lhs = std::get<0>(ngram);
            const std::string& rhs = std::get<1>(ngram);
            return(ngram_count_get(lhs, rhs));
        }
        void ngram_count_get(
            const std::string& lhs,
            const std::string& rhs,
            int& out
        ) const {
            auto lhs_it = ngram_counts.find(lhs);
            if (lhs_it == ngram_counts.end()) {
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
        int ngram_count_get(
            const std::string& lhs,
            const std::string& rhs
        ) const {
            int out = 0;
            ngram_count_get(lhs, rhs, out);
            return(out);
        }

        void lpmf(
            const std::string& lhs,
            const std::string& rhs,
            double& out
        ) const {
            double a = ngram_count_get(lhs, rhs);
            if (a == 0.0) {
                a += unseen_alpha;
            } else {
                a += alpha;
            }
            double b = ngram_count_get(lhs, "");
            b += n_rhs * alpha + unseen_alpha;
            out = log(a) - log(b);
        }
        double lpmf(
            const std::string& lhs,
            const std::string& rhs
        ) const {
            double out = 0.0;
            lpmf(lhs, rhs, out);
            return(out);
        }

        void lpmf(
            const std::string& text,
            double& out
        ) const {
            out = 0.0;
            const std::size_t n_ngrams = text.size() - n + 1;
            for (int i = 0; i < n_ngrams; i++) {
                // e.g. n = 3, text = "howdy", i = 1:
                // lhs = "ow", rhs = "d"
                const std::string& lhs = text.substr(i, n - 1);
                const std::string& rhs = text.substr(i + n - 1, 1);
                out += lpmf(lhs, rhs);
            }
            if (normalise_length) {
                out /= n_ngrams;
            }
        }
        double lpmf(
            const std::string& text
        ) const {
            double out = 0.0;
            lpmf(text, out);
            return(out);
        }

        void lpmf(
            const std::vector<std::string>& texts,
            std::vector<double>& out,
            int n_threads = 1
        ) const {
            omp_set_num_threads(n_threads);
            #pragma omp parallel for
            for (int i = 0; i < out.size(); i++) {
                const std::string& text_i = texts[i];
                double& out_i = out[i];
                lpmf(text_i, out_i);
            }
        }
        std::vector<double> lpmf(
            const std::vector<std::string>& texts,
            int n_threads = 1
        ) const {
            std::vector<double> out(texts.size());
            lpmf(texts, out, n_threads);
            return(out);
        }

    };

}

#endif
