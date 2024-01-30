#ifndef ROUBY_GAME_GENETIC_H
#define ROUBY_GAME_GENETIC_H

#include <stdexcept>
#include <forward_list>
#include <list>
#include <array>
#include <vector>

namespace ga {
    typedef long long int factor;

    class Chromosome {
    private:
        factor *gene = nullptr;
        size_t size = 0;

        void getbig_small(){
            for (factor i = 0;i < size; i++){
                if(big < gene[i]){
                    big = gene[i];
                }
                if(small > gene[i]){
                    small = gene[i];
                }
            }
        }
    public:
        factor big = 0, small = 0;

        Chromosome() = default;

        Chromosome(const Chromosome &chr) {
            if(chr.gene != nullptr){
                this->size = chr.size;
                this->gene = new factor[this->size];
                *(this->gene) = *(chr.gene);
                this->big = chr.big;
                this->small = chr.small;
            }
        }

        template<class Arr>
        Chromosome(Arr a) {
            static_assert(std::is_member_function_pointer<decltype(&Arr::size)>::value && \
        std::is_member_function_pointer<decltype(&Arr::operator[])>::value,
                          "Constructure function Chromosome: Constructure want a array style parameter");

            for (size_t i = 0; i < a.size(); i++) {
                gene[i] = a[i];
            }
            getbig_small();
        }

        Chromosome(std::list<factor> list) {
            size_t i = 0;
            size = list.size();
            gene = new factor[size];
            for (typename std::list<factor>::iterator it = list.begin(); it != list.end(); ++it) {
                gene[i] = *it;
                i++;
            }
            getbig_small();
        }

        Chromosome(std::forward_list<factor> list) {
            size_t i = 0;
            this->size = 0;
            for (typename std::forward_list<factor>::iterator it = list.begin(); it != list.end(); ++it, ++size);
            gene = new factor[size];
            for (typename std::forward_list<factor>::iterator it = list.begin(); it != list.end(); ++it) {
                gene[i] = *it;
                i++;
            }
            getbig_small();
        }

        // Note: to programming more Constructor to support more array-type!

        factor &operator[](size_t _Where) {
            return gene[_Where];
        }

        Chromosome &operator=(const Chromosome &chr){
            if(chr.gene != nullptr){
                this->size = chr.size;
                this->gene = new factor[this->size];
                *(this->gene) = *(chr.gene);
                this->big = chr.big;
                this->small = chr.small;
            }
        }

        template<class Arr>
        Chromosome &operator=(Arr a){
            static_assert(std::is_member_function_pointer<decltype(&Arr::size)>::value && \
        std::is_member_function_pointer<decltype(&Arr::operator[])>::value,
                          "Constructure function Chromosome: Constructure want a array style parameter");

            for (size_t i = 0; i < a.size(); i++) {
                gene[i] = a[i];
            }
            getbig_small();
        }

        Chromosome &operator=(std::list<factor> list){
            size_t i = 0;
            size = list.size();
            gene = new factor[size];
            for (typename std::list<factor>::iterator it = list.begin(); it != list.end(); ++it) {
                gene[i] = *it;
                i++;
            }
            getbig_small();
        }

        Chromosome &operator=(std::forward_list<factor> list){
            size_t i = 0;
            this->size = 0;
            for (typename std::forward_list<factor>::iterator it = list.begin(); it != list.end(); ++it, ++size);
            gene = new factor[size];
            for (typename std::forward_list<factor>::iterator it = list.begin(); it != list.end(); ++it) {
                gene[i] = *it;
                i++;
            }
            getbig_small();
        }

        void clear() {
            delete[]gene;
            gene = nullptr;
            size = 0;
        }

        void set(factor value, size_t _Where) {
            this->operator[](_Where) = value;
        }
    };

    template<const size_t chr_num>
    class Genetic {
    private:
        std::vector<Chromosome> chrs;
    public:
        Genetic() = delete;

        Genetic(Chromosome(&population)[chr_num]) {
            this->chrs = population;
        }

        Genetic(std::vector<Chromosome> population) {
            if (population.size() > chr_num || population.size() < chr_num)
                throw std::out_of_range{"Constructure function Genetic: "
                                        "number of population should amount \"chr_num\""};

            chrs = population;
        }

        Genetic(std::array<Chromosome, chr_num> population) {
            chrs = population;
        }

        //Note: to programming more Constructor to support more array-type!


    };

}
#endif //ROUBY_GAME_GENETIC_H
