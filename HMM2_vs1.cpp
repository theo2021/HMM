hmm2_fred_v1.cpp
Type
C++
Size
9 KB (9,019 bytes)
Storage used
0 bytes
Location
AI_A1
Owner
Theodor Panagiotakopoulos
Modified
Sep 8, 2019 by Theodor Panagiotakopoulos
Opened
11:41 PM by me
Created
Sep 8, 2019
Add a description
Viewers can download

/////////////////////
// Assignment 1 - AI
////////////////////

#include <cstdlib>
#include <iostream>
#include <string.h>
using namespace std;

struct vec{ //create a new type of variable called vec
    int rows;
    int cols;
    float **pi;
};

////////
// HMM0
////////

vec mat_mul(vec v_in1, vec v_in2){//input 1 is transition matrix and input 2 is emission matrix
    //if(v_in1.cols == v_in1.rows){
    //tratar excecao    
        vec v_out; //create vector-object output
        v_out.pi = new float *[v_in1.rows]; //adress pi of vector-output to ohave the same number of slots as the number of rows of input 1 
        v_out.rows = v_in1.rows;
        v_out.cols = v_in2.cols; //remember: A(axb)*B(bxc)=C(axc)

        for(int i=0; i<v_in1.rows; i++){ //

            v_out.pi[i] = new float[v_in2.cols]; //
            for(int j=0; j<v_in2.cols; j++){ //

                float lin_comb = 0; //
                for (int k=0;k<v_in1.cols;k++){ //

                    lin_comb+= v_in1.pi[i][k]*v_in2.pi[k][j]; //
                }
                v_out.pi[i][j] = lin_comb; //
            }
        }
        return v_out;
    //}
}

void print_array_inline(vec A){
    cout << A.rows << ' ' << A.cols;
    for (int i=0; i<A.rows; i++)    {
        for(int j=0; j< A.cols; j++){
            cout << ' ' << A.pi[i][j];
        }
    }
}

void print_vec(vec A){
    for(int i=0; i<A.rows; i++){
        for(int j=0; j<A.cols; j++){
            cout << A.pi[i][j] << " ";
        }
        cout << endl;
    }
}

vec getinput(){
    string line;
    float input[5000]={};
    int start = 0;
    int counter = 0;
    
    getline(cin, line);
    line += " ";
    bool flag = true;

    for (int i=0; i<line.length(); i++){
        if (line.at(i) == ' ' && flag){
            input[counter] = stof(line.substr(start,i-start));
            start = i+1;
            counter++;
            flag = false;
        }else if (line.at(i) != ' '){
            flag = true;
        }
    }
    vec A;
    A.rows = (int)input[0];
    A.cols = (int)input[1];
    A.pi = new float *[A.rows];
    for(int i=0; i<A.rows; i++){
        A.pi[i] = new float[A.cols];
        for(int j=0; j<A.cols; j++){
            A.pi[i][j] = input[2 + i*A.cols+ j];
        }
    }
    return A;   
}

////////
// HMM1
////////

struct list{
    int *pi;
    int size;
};

list get_observations(){
    int nums;
    cin >> nums;
    list o;
    o.size = nums;
    o.pi = new int[nums];
    for (int i=0;i<nums;i++){
        cin >> o.pi[i];
    }
    return o;
}

void print_list(list a){
    cout << '[';
    for(int i=0;i<a.size - 1;i++){
        cout << a.pi[i] << ' ';
    }
    if(a.size>0){cout << a.pi[a.size-1];}
    cout << ']';
}

vec elmnt_wise(vec in1,vec in2){
    //note: in1 and in2 must have the same dimension -> lack exception implementation//
    int n = in1.rows;
    int m = in1.cols;
    vec out; 
    out.pi = new float *[in1.rows]; 
    out.rows = n;
    out.cols = m; 

    for(int i=0; i<n; i++){
        out.pi[i] = new float[m]; //
        for(int j=0; j<m; j++){
            out.pi[i][j] = in1.pi[i][j]*in2.pi[j][i]; //remember: element wise is row by column 
        }
    }
    return out;
}

vec extrct_col(vec A, int loc){
    vec B;
    B.rows = A.rows;
    B.cols = 1;
    B.pi = new float *[A.rows];
    for(int i=0;i<A.rows;i++){
        B.pi[i] = new float[B.cols];
        B.pi[i][0] = A.pi[i][loc];
    }
    return B;

}

vec extrct_row(vec A, int loc){
    vec B;
    B.rows = 1;
    B.cols = A.cols;
    B.pi = new float *[1];
    B.pi[0] = new float[B.cols];
    for(int i=0;i<A.cols;i++){
        B.pi[0][i] = A.pi[loc][i];
    }
    return B;
}

vec a_pass(list ob,vec A,vec B,vec P){
    int ns = A.rows;
    vec ALPHA;
    ALPHA.rows = ob.size;
    ALPHA.cols = ns;
    ALPHA.pi = new float *[ALPHA.rows];
    vec tmp = elmnt_wise(P,extrct_col(B,ob.pi[0]));
    ALPHA.pi[0] = tmp.pi[0];
    

    for(int i=1; i<ob.size; i++){
        vec tmp1 = mat_mul(extrct_row(ALPHA,i-1),A);
        ALPHA.pi[i] = elmnt_wise(tmp1,extrct_col(B,ob.pi[i])).pi[0];
    }
    return ALPHA;
}

////////
// HMM2
////////
vec inv(vec A){
    vec B;
    B.cols = A.rows;
    B.rows = A.cols;
    B.pi = new float*[A.cols];
    for(int j=0;j<A.cols;j++){
        B.pi[j] = new float[A.rows];
        for(int i=0;i<A.rows;i++){
            B.pi[j][i]=A.pi[i][j];
        }
    }
    return B;
}
pair <vec, vec> maximus(vec input){
    //pair <vec, vec> OUT;
    vec out_pr,out_indx;
    out_pr.rows = input.rows;
    out_pr.cols = 1;
    out_pr.pi = new float *[input.rows];
    out_indx.rows = input.rows;
    out_indx.cols = 1;
    out_indx.pi = new float *[input.rows];
    int bin;
    for(int i=0;i<input.rows;i++){
        out_pr.pi[i] = new float[1];
        out_indx.pi[i] = new float[1];
        out_pr.pi[i][0] = input.pi[i][0];
        bin= 1;
        out_indx.pi[i][0] = 1;
        for(int j=1;j<input.cols;j++){
            bin*=2;
            if(out_pr.pi[i][0] < input.pi[i][j]){
               out_pr.pi[i][0] = input.pi[i][j];
               out_indx.pi[i][0] = bin;
            }else if(out_pr.pi[i][0] == input.pi[i][j]){
               out_indx.pi[i][0] += bin;
            }
        }
    }
    pair <vec, vec> OUT (out_pr,out_indx);
    return OUT;
}
void addrows(vec &A, vec B){
    float **newpi = new float *[A.rows + B.rows];
    for(int i=0;i< A.rows;i++){
        newpi[i] = A.pi[i];
    }
    for (int i=0;i<B.rows;i++){
        newpi[A.rows+i] = new float[B.cols];
        for (int j=0;j<B.cols;j++){
            newpi[A.rows + i][j] = B.pi[i][j];
        }
        
    }
    A.rows += B.rows;
    A.pi = newpi;
}
vec find_prev_state(vec di,int time,int start){
    vec out;
    out.rows = 0;
    out.cols = time;
    //out.pi[0] = new float[di.rows];
    for(int i=0; start>0; i++){
        if(start%2){
            vec tmp;
            if (time == 1){
                tmp.rows=1; tmp.cols=1;tmp.pi = new float *[1];tmp.pi[0] = new float[1];
                tmp.pi[0][0] = i + 0.0;
            }else{
                vec prev = find_prev_state(di, time-1, di.pi[time-1][i]);
                tmp.rows=prev.rows; tmp.cols=time;tmp.pi = new float *[tmp.rows]; // initialization
                for(int ik=0;ik<tmp.rows;ik++){
                    tmp.pi[ik] = new float[time];
                    tmp.pi[ik][tmp.cols-1] = i + 0.0;
                    for(int j=0;j<tmp.cols-1;j++){
                        tmp.pi[ik][j] = prev.pi[ik][j];
                    }
                }
            }
            //print_vec(tmp);
            addrows(out,tmp);
        }   
        start/=2;  
    }
    return out;    
}

vec Viterbi(vec A, vec B, vec p, list obsv){
    
    vec d = elmnt_wise(p,extrct_col(B,obsv.pi[0]));
    //print_vec(d);
    //cout << endl;
    vec dindx = extrct_row(p,0);
    for(int i=1;i<obsv.size;i++){
        /*
        cout << "--------" << endl;
        print_vec(elmnt_wise(mat_mul(extrct_col(B,obsv.pi[i]), extrct_row(d,i-1)),A));
        cout << "||" << endl;;
        print_vec(extrct_row(d,i-1));
        cout << "||" << endl;;
        print_vec(extrct_col(B,obsv.pi[i]));
        cout << "||" << endl;;
        print_vec(mat_mul(extrct_col(B,obsv.pi[i]), extrct_row(d,i-1)));
        cout << "--------" << endl;
        */
        pair<vec,vec> ans = maximus(elmnt_wise(mat_mul(extrct_col(B,obsv.pi[i]), extrct_row(d,i-1)),A));
        //cout << "--------" << endl;
        //print_vec(ans.first);
        //cout << "--------" << endl;
        addrows(d,inv(ans.first));
        addrows(dindx,inv(ans.second));
    }
    //print_vec(d);
    //print_vec(dindx);
    pair<vec,vec> ans = maximus(extrct_row(d,d.rows-1));
    return find_prev_state(dindx, dindx.rows, ans.second.pi[0][0]);
    //print_vec(ans.second);
    return d;

}

////////
// HMM3
////////

/////
// C
/////

///////////
// DUCKHUNT
///////////

/////////////
// Programme
/////////////

int main (){
    vec A = getinput();
    vec B = getinput();
    vec p = getinput();
    list obsv = get_observations();
    vec Alpha = a_pass(obsv,A,B,p);
    float sum_alpha = 0;
    for(int i=0; i<A.rows; i++){
        sum_alpha += Alpha.pi[obsv.size-1][i];
    }

    /*
    cout << endl;
    printf("A");
    cout << endl;
    print_vec(A);

    cout << endl;
    printf("B");
    cout << endl;
    print_vec(B);
    
    cout << endl;
    printf("pi");
    cout << endl;
    print_vec(p);

    cout << endl;
    printf("Observations");
    cout << endl;
    print_list(obsv);
    cout << endl;
    */
    /*
    cout << endl;
    printf("output file");
    cout << endl;
    vec tmp = mat_mul(p,A);
    print_array_inline(mat_mul(tmp,B));
    
    cout << endl;
    printf("Observations");
    cout << endl;
    print_list(obsv);

    cout << endl;
    printf("Alpha t");
    cout << endl;
    print_vec(Alpha);
    */
    //pair<vec, vec> out = maximus(A);
    //print_vec(out.first);
    //print_vec(out.second);
    //cout << sum_alpha;
    print_vec(Viterbi(A,B,p,obsv));
    
    return 0;
}
