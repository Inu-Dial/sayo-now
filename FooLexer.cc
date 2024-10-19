#include <string>
#include <sstream>
#include "FooLexer.hh"


namespace foo{
    static std::mt19937_64 mt(time(0));
    std::string get_rand_str(){
        std::stringstream ss;
        ss<<mt()<<mt();
        return ss.str();
    }
    enum class int_type;
    int_type get_int_type(std::string s){
        return int_type::int32;
    }

}