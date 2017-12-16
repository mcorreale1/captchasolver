#ifndef MINIMIZE_H
#define MINIMIZE_H

class Minimize {
    public:
        int lambda = 3;
        arma::mat X;
        arma::mat y;
        arma::mat theta;
        int m;
        Minimize(vector<ImageData> &d, vector<vector<float> > &t) {
            m = d.size();
            X.resize(m, VECTOR_SIZE);
            y.resize(m, 1);
            theta.resize(10, VECTOR_SIZE);
            for(int i = 0; i < d.size(); ++i) {
                y.at(i, 0) = (int) d[i].c;
                for(int k = 0; k < d[i].pixels.size(); ++k) {
                    X.at(i,k) = d[i].pixels[k];
                }
            }
            for(int i = 0; i < 10; ++i) {
                for(int k = 0; k < VECTOR_SIZE; ++k) {
                    theta.at(i, k) = t[i][k];
                }
            }

        }

        double sigmoid(double z){
            return 1.0 / (1.0 + exp(-z));
        }
         
        arma::rowvec sigmoid(arma::rowvec z){
            for(int i=0; i<z.size(); i++){
                z(i) = sigmoid(z(i));
            }
            return z;
        }
        /*
        arma::colvec sigmoid(arma::colvec z){
            arma::rowvec temp = z.t();
            return (sigmoid(temp)).t();
        }
        */

        arma::mat sig(int i) {
            arma::mat z = 1.0 / ( 1 + arma::exp((X * theta.row(i).t()) * -1.0));
            return z;
        }

        arma::mat computeCost(int i, int c) {
            arma::mat J;
            J = arma::sum(-y.row(i) * arma::log(sigmoid(X.row(i) * theta.row(i).t())) - (1.0 - y.row(i) * arma::log(1.0 - sigmoid(X.row(i) * theta.row(i).t())))) / m;
            //J = J + (lambda / (2.0 * m)) * arma::sum(pow(theta.row(i).t(), 2));
            //J = (arma::sum( ((pow(((X * theta.row(0).t()) - y), 2)) / (2 * m) )));
            //cout << J << endl;
            //J = J + (lambda / (2 * m) * sigmoid(arma::sum(pow(theta.row(0).t(), 2))));
            return J;
        }

        void gradientDescent(double alpha, int num_iters, int r) {
            arma::mat delta;
            for(int i = 0; i < num_iters; ++i) {
                //delta = (X.row(r).t() * (sigmoid(X.row(r) *(theta.row(r)).t()) - y.row(r))) / m;
                delta = X.row(r).t() * (sigmoid(X.row(r) * theta.row(r).t()) - y.row(r)) / m + ((float)lambda / (float)m) * arma::sum(pow(theta.row(r).t(), 2));
                theta.row(r) = (arma::trans((theta.row(r).t() - alpha * delta)));
            }
            //theta.row(0) = theta.row(0) + (lambda / m * theta.row(0));
            //cout <<  theta.row(0) << endl;
        }
};

/*
void gradientDesnt( vector<ImageData> &data, double alpha, int iter, vector<float> &theta){
    arma::delta;
    int m = data.size();

    for(int i = 0; i < iter; ++i){
        delta = arma::trans(X)*
    }
}
*/

/*arma::mat computeCost(vector<ImageData> &data, const arma::mat &theta){
    arma::mat J;
    int m = 1000;
    m = data.size();
    arma::mat X;
    arma::mat y;

    X.resize(m, VECTOR_SIZE);
    y.resize(m, 1);
    for(int i = 0; i < data.size(); ++i) {
        y.at(i, 0) = (int) data[i].c;
        for(int k = 0; k < data[i].pixels.size(); ++k) {
            X.at(i,k) = data[i].pixels[k];
        }
    }
    J = arma::sum( (pow(((X * theta) - y), 2)) / (2 * m) ) * lambda / (2 * m) * arma:::sum(pow(theta, 2));
    return J;
}*/


#endif
