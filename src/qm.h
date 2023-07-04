#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>

using namespace std;

typedef struct term{
    string data;
    int dash_count;
    int one_count;
    //int input_term_idx;
    set <int> record;
} Term;

class Quine_Mccluskey{

public:    

    int number_of_variables;
    int number_of_product_terms;
    int number_of_sum_of_minterms;

    vector <Term> input_term;
    vector <Term> min_terms;
    vector <Term> Prime_Implicant;
    vector <int> Prime_Implicant_literal_count;

    
    
    vector <set<int>> Patrick_SOP_term;
    vector <set<int>> Patrick_POS_term;
    int number_of_SOP_term; // Patrick_SOP_term.size()
    int number_of_POS_term; // Patrick_POS_term.size()
    
    
    //vector <int> _count;

    //vector <string> product_term;
    
    void Count_Literal();
    void Print_Prime_Implicant();
    void Comined_Duplicate_Terms();
    void Comined_Duplicate_Term(Term&, Term);
    bool Check_if_Duplicate(string, string);

    Quine_Mccluskey(ifstream&);
    
    void solve_qm();
    void Generate_Prime_Implicant(vector <Term>&, vector <Term>&, vector <Term>&);
    Term combine(Term, Term);
    bool Can_Be_Combined(string, string);
    void Expand_Dont_Care(int, Term&);
    
    void POS_to_SOP(int, set<int>&);
    void Generate_Patrick_POS();
    void Find_Optimized_Product(ofstream&);
    void solve_Patrick_Method(ofstream&);
    void print_set(set <int>&);
    

private:

    friend bool sort_input_term(const Term&, const Term&);
    friend bool sort_terms(const Term&, const Term&);
    friend bool is_term_equal(const Term&, const Term&);

};

