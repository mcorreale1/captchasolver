#ifndef MINIMIZE_H
#define MINIMIZE_H


mat computeCost(vector<vector <int>> X, vector<ImageData> y, const mat& theta){
    mat J;
    int m;
    m = y.n_rows;
    J = arma::arma::sum( (pow(((X * theta) - y), 2)) / (2 * m) );
    return J;
}

#endif
