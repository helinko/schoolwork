
#ifndef SATUNNAISUUTTA_HH
#define SATUNNAISUUTTA_HH

#include <exception>
#include <fstream>
#include <string>
#include <cstdint>

// virhepoikkeus
class RandError : public std::exception {
public:
    RandError(std::string message) : msg_(message) {};
    ~RandError() throw() {};
    const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

//
// Tuotetaan satunnaislukuja linuxin /dev/urandom:n avulla
// uudessa C++:ssa on <random>-palveluita, mutta tämä koodi ei käytä niitä
// koska mallikoodi näyttää /dev/urandom-tiedoston suojauksen yhtäaikaiselta luvulta
//
class Random {
public:
    Random();
    virtual ~Random();

    // 32 bittinen satunnaisluku (koko lukualue käytössä)
    uint32_t integer();
    // satunnaisluku, joka on pienmpi kuin *param* ( väli [0,X-1] )
    uint32_t integer( uint32_t topPlusOne );
    // satunnaisluku väliltä [lower, upper]
    uint32_t integer( uint32_t lower, uint32_t upper );

private:
    std::ifstream file_;
};

#endif
