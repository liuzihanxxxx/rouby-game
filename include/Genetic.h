#ifndef ROUBY_GAME_GENETIC_H
#define ROUBY_GAME_GENETIC_H

#include <stdexcept>
#include <forward_list>
#include <initializer_list>
#include <list>
#include <array>
#include <vector>
#include <random>
#include <map>

namespace ga {
    typedef long long int factor;
    typedef long double fitness;

    struct Chromosome {
    private:
        factor *gene = nullptr;
        size_t size = 0;

        void getbig_small() {
            for (factor i = 0; i < size; i++) {
                if (big < gene[i]) {
                    big = gene[i];
                }
                if (small > gene[i]) {
                    small = gene[i];
                }
            }
        }

        factor big = 0, small = 0;
    public:
        class bad_chromosome: public std::exception{
        public:
            const char* what() const noexcept{
                return "fatal error: In your recent use this chromosome, it was null!";
            }
        };

        Chromosome() = default;

        Chromosome(const Chromosome &chr) {
            if (chr.gene != nullptr) {
                this->size = chr.size;
                this->gene = new factor[this->size];
                *(this->gene) = *(chr.gene);
                this->big = chr.big;
                this->small = chr.small;
            }
        }

        Chromosome(size_t length){
            size = length;
            gene = new factor[length];
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

        enum {
            BIG,
            SMALL
        };

        factor &operator[](size_t _Where) {
            if(this->gene == nullptr){
                throw bad_chromosome{};
            }
            //If you don't have the following instructions, Just give a note
#pragma clang diagnostic push
#pragma ide diagnostic ignored "NullDereference"
            return gene[_Where];
#pragma clang diagnostic pop
        }

        Chromosome &operator=(const Chromosome &chr) {
            if (chr.gene != nullptr) {
                this->size = chr.size;
                this->gene = new factor[this->size];
                *(this->gene) = *(chr.gene);
                this->big = chr.big;
                this->small = chr.small;
            }
        }

        template<class Arr>
        Chromosome &operator=(Arr a) {
            static_assert(std::is_member_function_pointer<decltype(&Arr::size)>::value && \
        std::is_member_function_pointer<decltype(&Arr::operator[])>::value,
                          "Constructure function Chromosome: Constructure want a array style parameter");

            for (size_t i = 0; i < a.size(); i++) {
                gene[i] = a[i];
            }
            getbig_small();
        }

        Chromosome &operator=(std::list<factor> list) {
            size_t i = 0;
            size = list.size();
            gene = new factor[size];
            for (typename std::list<factor>::iterator it = list.begin(); it != list.end(); ++it) {
                gene[i] = *it;
                i++;
            }
            getbig_small();
        }

        Chromosome &operator=(std::forward_list<factor> list) {
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
        virtual void set(factor value, size_t _Where) {
            this->operator[](_Where) = value;
        }
        virtual factor get(decltype(BIG) _Which) {
            switch (_Which) {
                case BIG:
                    return this->big;
                case SMALL:
                    return this->small;
            }
        }

        //iterator
        virtual factor *begin(){ return gene; }
        virtual factor *end(){ return gene+size; }



        ~Chromosome() {
            this->clear();
        }
    };

    class fitnessFunction {
    protected:
        virtual fitness ffunction(Chromosome) = 0;

    public:
        fitness operator()(Chromosome ch) {
            return ffunction(ch);
        }
    };

#define fitnessFunc(clsname, function_body) class clsname: public fitnessFunction\
{                                                                                \
    protected:                                                                   \
    virtual fitness ffunction(Chromosome ch)noexcept function_body               \
}                                                                                \
clsname fit_#clsname

    /**
        about marco fitnessFunc\n
        you should use like this:\n
            fitnessFunc(foo, {...});\n
            fit_foo(); //use
      */

    class Genetic {
    private:
        std::vector<Chromosome> chroms;
        struct selection;
    public:
        Genetic() = delete;

        Genetic(const Genetic &from) {
            this->chroms = from.chroms;
        }

        template<class T>
        requires requires(T t){
            { chroms = t };
        }
        Genetic(T from) {
            chroms = from;
        }

        //Note: to programming more Constructor to support more array-type!

        selection *selection;

        ///////////////////////////////////
        std::vector<Chromosome> &getChroms() {
            return chroms;
        }
    };

    namespace genetic_algo {
        struct selection {
        public:
        class bad_chromosome_fitness: public std::exception{
        public:
            const char* what() const noexcept{
                return "fatal error: fitness is amount 0!";
            }
        };

            template<class T>
            requires requires(T t, Chromosome ch){
                { t.ffunction(ch) }->std::same_as<fitness>;
            }
            [[nodiscard]] static std::vector<Chromosome> championship(T ffunc,
                                                                      std::vector<Chromosome> pop,
                                                                      long long int max_length = -1,
                                                                      short how_much = 7) {
                if (max_length < 0) {
                    max_length = pop.size();
                    if(max_length < 0 || max_length > pop.size()){
                        throw std::out_of_range{"function genetic_algo::selection::championship: "
                                                "population's size cannot less than 0!"};
                    }
                }
                while (how_much >= pop.size() - 1) {
                    how_much--;
                }

                std::vector<std::vector<long long int>> indexs(max_length);
                for(size_t i = 0 ;i < indexs.size(); i++){
                    indexs[i] = std::vector<long long int>(how_much);
                }
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib(0, pop.size() - 1);
                for (size_t i = 0; i < max_length; i++) {
                    for (short j = 0; j < how_much; j++) {
                        size_t index = distrib(gen);
                        indexs[i][j] = index;
                    }
                }

                std::vector<Chromosome> ret;

                for (long long int i = 0; i < max_length; i++) {
                    std::vector<fitness> fits(how_much);
                    for (short j = 0; j < how_much; j++) {
                        fits[j] = ffunc.ffunction(pop[indexs[i][j]]);
                    }
                    size_t where = std::distance(fits.begin(),std::max_element(fits.begin(), fits.end()));
                    ret.push_back(pop[indexs[i][where]]);
                }
                return ret;
            }

            template<class T>
            requires requires(T t, Chromosome ch){
                { t.ffunction(ch) }->std::same_as<fitness>;
            }
            [[nodiscard]] static std::vector<Chromosome> roulette(T ffunc,
                                                                  std::vector<Chromosome> pop,
                                                                  long long int max_length = -1){
                {
                    bool is_0 = true;
                    for(size_t i = 0; i < pop.size(); i++){
                        if (ffunc.ffunction(pop[i])>0){
                            is_0 = false;
                        }
                    }
                    if(is_0){
                        throw bad_chromosome_fitness{};
                    }
                }

                std::vector<Chromosome> ret;
                std::map<Chromosome, double> weights;
                fitness total;
                if(max_length < 0 || max_length > pop.size()){
                    max_length = pop.size();
                }

                for(Chromosome& ch : pop){
                    total += ffunc.ffunction(ch);
                }
                for(Chromosome& ch : pop){
                    weights[ch] = ffunc.ffunction(ch)/total;
                }

                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(0.0f, 1.0f);

                for(size_t i = 0;i < max_length; ++i){
                    long double rand = dis(gen);
                    long double sum = 0;

                    for(size_t j = 0; j < pop.size(); ++j){
                        sum+=weights[pop[j]];
                        if(sum > rand){
                            ret.push_back(pop[j]);
                            break;
                        }
                    }
                }
                return ret;
            }

            template<class T>
            requires requires(T t, Chromosome ch){
                { t.ffunction(ch) }->std::same_as<fitness>;
            }
            [[nodiscard]] static std::vector<Chromosome> elitism(T ffunc,
                                                                 std::vector<Chromosome> pop,
                                                                 long long int max_length = -1){
                if(max_length < 0 || max_length > pop.size()){
                    max_length = pop.size()-1;
                }
                std::vector<std::pair<fitness, size_t>> fits;
                for(size_t i = 0; i < pop.size(); i++){
                    fits.push_back({ffunc.ffunction(pop[i]), i});
                }
                std::sort<std::vector<std::pair<fitness, size_t>>,
                          bool(*)(std::pair<fitness, size_t>, std::pair<fitness, size_t>)>(fits.begin(), fits.end(),
                                  [](std::pair<fitness, size_t>a, std::pair<fitness, size_t>b){
                                        return a.first < b.first;
                                    });

                std::vector<Chromosome> ret;
                for(size_t i = 0; i < max_length; i++){
                    ret.push_back(pop[fits[i].first]);
                }

                return ret;
            }

            [[nodiscard]] static std::vector<Chromosome>random(std::vector<Chromosome> pop,
                                                               long long int max_length = -1){
                if(max_length < 0 || max_length > pop.size()){
                    max_length = pop.size()-1;
                }
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(0, pop.size()-1);
                std::vector<Chromosome> ret;
                for(size_t i = 0; i < max_length; i++){
                    ret.push_back(pop[dis(gen)]);
                }

                return ret;
            }
        };
        struct crossover{
            static void single_point(Chromosome a, Chromosome b, long long int where){
                factor tmp = a[where];
                a[where] = b[where];
                b[where] = tmp;
            }

            template<class T>
            requires requires(T t){
                {t[0]};
                {t.size()}->std::same_as<size_t>;
            }
            static void multi_point(Chromosome a, Chromosome b, T where){
                for(size_t i  = 0; i < where.size(); i++){
                    single_point(a, b, where[i]);
                }
            }
        };
    }
}

using namespace ga::genetic_algo;

#define getChromes

#endif //ROUBY_GAME_GENETIC_H
