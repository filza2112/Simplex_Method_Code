#include <bits/stdc++.h>
using namespace std;

void print(vector<vector<double>> &A, int cons, int var) {
    cout << "Cb\t    X\t    Xb\t    ";
    for(int i=0; i<2 * cons + var; i++){
        cout<<"X"<<i+1<<"\t    ";
    }
    cout<<"Ratio"<<endl;
    for (int i = 0; i < cons; i++) {
        for (int j = 0; j < (2 * cons + var + 4); j++) {
            if(A[i][j]>1000 || A[i][j]<=-1000){
                cout<<setprecision(2)<<(A[i][j]/10100)<<"M\t    ";
            }
            else if(j==1){
                cout<<"X"<<A[i][j]<<"\t    ";
            }
            else{
            cout <<setprecision(2)<<A[i][j] << "\t    ";
            }
        }
        cout << endl;
    }
}

void printnext(vector<vector<double>> &B, int cons, int var) {
    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            cout << "Zj\t\t\t    ";
        } else if (i == 1) {
            cout << "Cj\t\t\t    ";
        } else {
            cout << "Zj-Cj\t\t\t    ";
        }
        for (int j = 0; j < (2 * cons + var); j++) {
            if(B[i][j]>1000 || B[i][j]<=-1000){
                cout<<setprecision(2)<<(B[i][j]/10100)<<"M\t    ";
            }
            else{
            cout <<setprecision(2)<<B[i][j] << "\t    ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

bool sortcol(const vector<double> &v1, const vector<double> &v2) {
    return v1[1] < v2[1];
}
bool unbound =false;
int smg(vector<vector<double>> &A, vector<vector<double>> &B, int cons, int var, int &count, int size) {
    cout << "Iteration " << count + 1<<":-" << endl;

    double a = DBL_MAX, temp;

    for (int i = 0; i <size; i++) {
        double sum = 0;
        for (int j = 0; j < cons; j++) {
            sum += A[j][0] * A[j][3 + i];
        }
        B[0][i] = sum;
    }

    for (int i = 0; i <size; i++) {
        B[2][i] = B[0][i] - B[1][i];
        if (B[2][i] < a) {
            a = B[2][i];
            temp = i;
        }
    }

    if (a >= 0) {
        print(A, cons, var);
        printnext(B, cons, var);
        return 0;
    }

    double minElement = DBL_MAX;
    int temp2;
    bool flag = true;

    for (int i = 0; i < cons; i++) {
        double ratio = A[i][2] / A[i][3 + temp];
        if (ratio >= 0 && ratio < minElement) {
            A[i][2 * cons + var + 3] = ratio;
            minElement = ratio;
            temp2 = i;
            flag = false;
        }
    }

    if (flag) {
        cout << "IT'S UNBOUNDED" << endl;
        unbound=true;
        return 0;
    }
    int value = A[temp2][1] - 1;

    for (int i = 0; i < cons; i++) {
        A[i][value + 3] = 0;
    }
    for (int i = 0; i < 3; i++) {
        B[i][value] = 0;
    }

    double element = A[temp2][3 + temp];

    for (int i = 0; i <=size; i++) {
        A[temp2][i + 2] = A[temp2][i + 2] / element;
    }

    A[temp2][1] = temp + 1;
    A[temp2][0] = B[1][temp];

    for (int i = 0; i < cons; i++) {
        if (i != temp2) {
            double factor = A[i][3 + temp];
            for (int j = 0; j <=size; j++) {
                A[i][j + 2] -= factor * A[temp2][j + 2];
            }
        }
    }


    print(A, cons, var);
    printnext(B, cons, var);
    count++;

    return smg(A, B, cons, var, count, size);
}




int main() {
    int cons, var, count = 0, m = 10100;
    vector<vector<double>> A;
    vector<vector<double>> B;

    cout << "Enter the number of constraints:";
    cin >> cons;
    cout << "Enter the number of variables: ";
    cin >> var;

    int size = var + 2 * cons;

    A.resize(cons, vector<double>(size + 4, 0));
    B.resize(3, vector<double>(size, 0));

    cout << "Enter the coefficients of the objective function:" << endl;
    for (int j = 0; j < var; j++) {
        cin >> B[1][j];
    }

    for (int i = 0; i < cons; i++) {
        cout << "Enter Coefficients of constraint" << i + 1 << ": ";
        for (int j = 0; j < var; j++) {
            cin >> A[i][j + 3];
        }
        int con = 1;
        cout << "enter your choice:\n1) <=\n2) >=\n3) =" << endl;
        cin >> con;
        cout << "Enter right side of the constraint" << i + 1 << ": " << endl;
        cin >> A[i][2];

        if (con == 1) {
            A[i][3 + var + i] = 1;
            A[i][1] = var + i + 1;
        } else if (con == 2) {
            A[i][3 + var + i] = -1;
            A[i][3 + var + cons + i] = 1;
            A[i][1] = var + cons + i + 1;
            A[i][0] = -m;
            B[1][var + cons + i] = -m;
        } else {
            A[i][3 + var + i] = 0;
            A[i][3 + var + cons + i] = 1;
            A[i][1] = var + cons + i + 1;
            A[i][0] = -m;
            B[1][var + cons + i] = -m;
        }
   
    }

cout << endl
     << "1: Maximization" << endl
     << "2: Minimization" << endl;
int choice = 1;
cout << "Select the Option : ";
cin >> choice;


for (int j = 0; j < var; j++) {
    B[1][j] = (choice == 2) ? -B[1][j] : B[1][j];
}


print(A, cons, var);
printnext(B, cons, var);
smg(A, B, cons, var, count, size);

    sort(A.begin(), A.end(), sortcol);

    cout << "Final Table:-" << endl;
    print(A, cons, var);
    printnext(B, cons, var);
    if(unbound){
        return 0;
    }
    double Z = 0;
    for (int i = 0; i < cons; i++) {
        if (A[i][1] <= var) {
            cout << "X" << (A[i][1]) << " = " <<setprecision(5)<<(A[i][2]) << endl;
        }
    }
    int i = 0, j = 0;
    while (A[i][1] <= var) {
        if (A[i][1] == j + 1) {
            Z += A[i][2] * B[1][j];
            // cout<<Z<<endl;
            if(A[i][1]==var){
                break;
            }
            i++;
            j++;
        }
        else{
            j++; 
        }
    }

    if(choice==2){
        cout<<"Ans is "<<setprecision(5)<<(-1.00*Z);
    }
    else{
        cout<<"Ans is "<<setprecision(5)<<Z;
    }

    for (int i = 0; i < cons; i++) {
        if (A[i][1] > var && A[i][2]!=0) {
            cout <<endl<< "Infeasible solution" << endl;
        } 
        else {
        }
    }
    
    
    return 0;
}
