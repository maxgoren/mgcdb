#include "./include/util.hpp"

vector<string> split(string str, char delim) {
    int l = 0, r = 0;
    vector<string> pieces;
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == delim) {
            if (r-l > 1 || (r-l == 1 && str[l] != delim))
                pieces.push_back(str.substr(l, r-l));
            l = i+1; r = l;
        } else r++;
    }
    //last slice wont end with a delimiter (most likely)
    if (r-l > 1 || (r-l == 1 && str[l] != delim)) {
        pieces.push_back(str.substr(l, r-l));
    }
    return pieces;
}