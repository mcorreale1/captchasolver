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

        arma::mat sigmoid(arma::mat z){
            return 1.0 / (1.0 + arma::expmat(-z));
        }

        float sigmoid(float z) {
            return 1.0 / (1.0 + exp(-z));
        }


        void runOneVsAll(double alpha, int num_iters, vector<uchar> &image) {
            arma::mat img(1, VECTOR_SIZE);
            for(int i = 0; i < VECTOR_SIZE; ++i) {
                img.at(0,i) = (double) image[i];
            }

            for(int c = 0; c < 10; ++c) {
                cout << "ON CLASS " << c << endl;
                for(int itr = 0; itr < num_iters; ++itr) {
                    arma::mat J = computeCost((c * 100), c);
                    for(int r = (c * 100) + 1; r < ((c + 1) * 100); ++r) {
                        J = computeCost(r, c);
                        gradientDescent(J, alpha, r, c );
                    }
                }
            }


            for(int i = 0; i < 10; ++i) {
                cout << "Class " << i << " Percent: " << sigmoid(img * theta.row(i).t()) << endl;
            }
        }
         
        arma::mat sig(int i) {
            arma::mat z = 1.0 / ( 1 + arma::exp((X * theta.row(i).t()) * -1.0));
            return z;
        }

        //Use when testing
        arma::mat computeCost(arma::mat &image, int c) {
            arma::mat J;

            J = arma::sum(-c * arma::log(sigmoid(image * theta.row(c).t())) - (1.0 - c * arma::log(1.0 - sigmoid(image * theta.row(c).t()))));
            J = arma::abs(J);

            return J;
        }

        //Use when training
        arma::mat computeCost(int i, int c) {
            arma::mat J;

            int Y = (c == (int) y.at(i, 0));
            J = arma::sum((float) -Y * arma::log(sigmoid(X.row(i) * theta.row(c).t())) - (1.0 - (float)Y * arma::log(1.0 - sigmoid(X.row(i) * theta.row(c).t())))) / ((float)m/10.0);
            J = arma::abs(J);

            return J;
        }

        void gradientDescent(arma::mat J, double alpha, int i, int c) {
            int Y = (c == (int) y.at(i, 0));
            arma::mat delta;
            //delta = X.row(r).t() * ((X.row(r) * theta.row(c).t()) - y.row(r)) / m;
            //delta =   (X * theta.row(c).t() - y) / m;
            delta = arma::sum((sigmoid(X.row(i) * theta.row(c).t()) - Y) * X.row(i));
            theta.row(c) = (theta.row(c) - (alpha * delta)) + J.at(0,0);
            for(int k = 0; k < VECTOR_SIZE; ++k) {
                theta.at(c, k) = sigmoid(theta.at(c, k));
            }
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
