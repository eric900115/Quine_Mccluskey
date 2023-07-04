#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <fstream>
#include "qm.h"

using namespace std;

bool sort_input_term(const Term& lhs, const Term& rhs)
{
    if(lhs.dash_count != rhs.dash_count)
        return lhs.dash_count < rhs.dash_count;
    else if(lhs.dash_count != rhs.dash_count)
        return lhs.one_count < rhs.one_count;
    else
        return lhs.data < rhs.data;
}

bool sort_terms(const Term& lhs, const Term& rhs)
{
    if(lhs.one_count != rhs.one_count)
        return lhs.one_count < rhs.one_count;
    else
        return lhs.data < rhs.data;
}

void Quine_Mccluskey::Expand_Dont_Care(int level, Term& term)
{
    if(level >= number_of_variables){
        Term t;
        t.data = term.data;
        t.dash_count = term.dash_count;
        t.one_count = term.one_count;
        min_terms.push_back(t);
        return;
    }
    else{
        if(term.data[level] == '-'){
            term.data[level] = '1';
            Expand_Dont_Care(level + 1, term);
            term.data[level] = '0';
            Expand_Dont_Care(level + 1, term);
            term.data[level] = '-';
        }
        else{
            Expand_Dont_Care(level + 1, term);
        }
    }
}

// to determine whether Term a is equal to Term b
// ex: if a.data = 10-0 and b.data = 10-0, then is_term_equal(a, b) return true
bool is_term_equal(const Term& a, const Term& b)
{
    if(a.data == b.data)
        return true;
    else
        return false;
}

Quine_Mccluskey::Quine_Mccluskey(ifstream& infile)
{
    int idx, i, dash_count, one_count, level, number_of_minterms;
    string str;
    level = -1;

    //cin >> number_of_variables;
    //cin >> number_of_product_terms;

    infile >> number_of_variables;
    infile >> number_of_product_terms;

    // read input data
    for(i = 0; i < number_of_product_terms; i++)
    {
        //cin >> str;
        infile >> str;
        Term term;
        one_count = dash_count = idx = 0;
        
        while(str[idx] != '\0')
        {
            if(str[idx] == '-')
                dash_count++;
            if(str[idx] == '1')
                one_count++;
            idx++;
        }
        term.data = str;
        term.one_count = one_count;
        term.dash_count = dash_count;
        term.record.insert(i);
        input_term.push_back(term);
    }

    // Expand the don't care term, and store all the data in vector <min_terms>
    // vector <minterms> store 
    for(i = 0; i < number_of_product_terms; i++)
    {
        if(input_term[i].dash_count == 0){
            Term t;
            t.data = input_term[i].data;
            t.dash_count = input_term[i].dash_count;
            t.one_count = input_term[i].one_count;
            min_terms.push_back(t);
        }
        else{
            Expand_Dont_Care(0, input_term[i]);
        }
    }
    

    number_of_sum_of_minterms = number_of_minterms = min_terms.size();
    for(int i = 0; i < number_of_minterms; i++)
    {
        min_terms[i].record.insert(i);
    }

    sort(min_terms.begin(), min_terms.end(), sort_terms);
    //min_terms.erase(unique(min_terms.begin(), min_terms.end(), is_term_equal), min_terms.end());
    //sort(min_terms.begin(), min_terms.end(), sort_terms);
    Comined_Duplicate_Terms();
    sort(min_terms.begin(), min_terms.end(), sort_terms);

}

void Quine_Mccluskey::Comined_Duplicate_Terms()
{
    vector <bool> record_if_combined;
    vector <Term> Combined_Terms;
    int number_of_minterms = min_terms.size();

    for(int i = 0; i < number_of_minterms; i++)
    {
        record_if_combined.push_back(false);
    }

    for(int i = 0; i < number_of_minterms; i++)
    {
        if(record_if_combined[i] == true)
        {
            continue;
        }

        for(int j = i + 1; j < number_of_minterms; j++)
        {
            if(Check_if_Duplicate(min_terms[i].data, min_terms[j].data))
            {
                Comined_Duplicate_Term(min_terms[i], min_terms[j]);
                record_if_combined[j] = true;
            }
        }
    }

    for(int i = 0; i < number_of_minterms; i++)
    {
        if(record_if_combined[i] == false)
        {
            Combined_Terms.push_back(min_terms[i]);
        }
    }

    min_terms.clear();
    number_of_minterms = Combined_Terms.size();

    for(int i = 0; i < number_of_minterms; i++)
    {
        min_terms.push_back(Combined_Terms[i]);
    }

    Combined_Terms.clear();

}

void Quine_Mccluskey::Comined_Duplicate_Term(Term& a, Term b)
{
    set<int>::iterator it = b.record.begin();
    for(; it != b.record.end(); it++)
    {
        a.record.insert(*it);
    }
}

bool Quine_Mccluskey::Check_if_Duplicate(string a, string b)
{
    int difference_count;

    difference_count = 0;

    for(int i = 0; i < number_of_variables; i++)
    {
        if(a[i] != b[i])
            difference_count++;
    }

    if(difference_count == 0)
        return true;
    else
        return false;
}

 void Quine_Mccluskey::solve_qm()
 {
    int level, j;
    int Implicant_size, len_min_terms;
    vector <Term> terms;
    vector <Term> Implicant;

    len_min_terms = min_terms.size();
    
    for(int k = 0; k < len_min_terms; k++)
    {
        terms.push_back(min_terms[k]);
    }

    

    for(level = 0; level < number_of_variables; level++)
    {
        Generate_Prime_Implicant(terms, Implicant, Prime_Implicant);
        //cout << "\n~~~~~~~~~\n";
        cout << "level " << level << "\n";
        terms.clear();

        Implicant_size = Implicant.size();
        for(int i = 0; i < Implicant_size; i++)
        {
            terms.push_back(Implicant[i]);
        }

        Implicant.clear();
    }

    //Print_Prime_Implicant();

    /*cout << "Prime_Implicant\n";
    for(int i = 0; i < Prime_Implicant.size(); i++)
    {
        cout << Prime_Implicant[i].data << "\n";
        //for(int j = 0; j < number_of_variables; j++)
        //{
        //    if(Prime_Implicant[i].data[j] != '-')
        //        totel_literal_count++;
        //}        
    }*/
    //cout << "total literal : " << totel_literal_count << "\n";
    //cout << "size : " << Prime_Implicant.size() << "\n";
}

// ouput all prime Implicant
void Quine_Mccluskey::Print_Prime_Implicant()
{
    cout << "\n\n==============";
    cout << "\nPrime Imp\n";
    int totel_literal_count = 0;
    for(int i = 0; i < Prime_Implicant.size(); i++)
    {
        cout << Prime_Implicant[i].data << "\n";
        //print_set(Prime_Implicant[i].record);
        for(int j = 0; j < number_of_variables; j++)
        {
            if(Prime_Implicant[i].data[j] != '-')
                totel_literal_count++;
        }
    }
    cout << "total literal : " << totel_literal_count << "\n";
    cout << "size : " << Prime_Implicant.size() << "\n";
}

void Quine_Mccluskey::Generate_Prime_Implicant(vector <Term>& terms, 
            vector <Term>& Implicant, vector <Term>& Prime_Implicant)
{
    int *one_pos;
    int len_terms, len_Implicant, len_Prime_Implicant;
    bool *is_prime_implicant;
    int dash_count, one_count, idx;
    
    Term term;

    len_terms = terms.size();
    is_prime_implicant = new bool[len_terms];

    sort(terms.begin(), terms.end(), sort_terms);
    
    // is_prime_implicant stored wheter the variable is prime_implicant
    // if the term is combined with the other term, then it's not prime implicant
    for(int i = 0; i < len_terms; i++)
    {
        is_prime_implicant[i] = true;
    }

    for(int i = 0; i < len_terms; i++)
    {
        for(int j = i+1; j < len_terms; j++)
        {
            //if(terms[i].one_count == terms[j].one_count)
            //    continue;
            
            if(terms[i].one_count + 1 < terms[j].one_count)
                break;
            
            if(Can_Be_Combined(terms[i].data, terms[j].data))
            {
                term = combine(terms[i], terms[j]);
                Implicant.push_back(term);
                is_prime_implicant[i] = false;
                is_prime_implicant[j] = false;
            }
        }
    }

    // record the terms which are not combined with others
    for(int i = 0; i < len_terms; i++)
    {
        if(is_prime_implicant[i] == true)
            Prime_Implicant.push_back(terms[i]);
    }    

    // delete duplicate prime implicant when combining
    sort(Implicant.begin(), Implicant.end(), sort_terms);
    Implicant.erase(unique(Implicant.begin(), Implicant.end(), is_term_equal), Implicant.end());
    sort(Implicant.begin(), Implicant.end(), sort_terms);
    // save the prime implicant after removing duplicate element above
    /*len_Implicant = Implicant.size();
    for(int i = 0; i < len_Implicant; i++)
    {
        Prime_Implicant.push_back(Implicant[i]);
    }*/

    /*cout << "IM\n";
    for(int i = 0; i < Implicant.size(); i++)
    {
        cout << Implicant[i].data << "\n";
    }*/
    //return Prime_Implicant;
}

// combine term a and term b
// ex: a.data = 10-0, b.data = 00-0
// combined_term = -0-0 
Term Quine_Mccluskey::combine(Term a, Term b)
{
    Term combined_term;
    int difference_bit, one_count, dash_count;

    difference_bit = -1;
    one_count = dash_count = 0;

    for(int i = 0; i < number_of_variables; i++)
    {
        if(a.data[i] != b.data[i])
            difference_bit = i;
    }

    combined_term.data = a.data;
    combined_term.data[difference_bit] = '-';

    for(int i = 0; i < number_of_variables; i++)
    {
        if(combined_term.data[i] == '1')
            one_count++;
        if(combined_term.data[i] == '-')
            dash_count++;
    }

    combined_term.dash_count = dash_count;
    combined_term.one_count = one_count;

    // combine record
    set<int>::iterator it = a.record.begin();
    for(; it != a.record.end(); it++)
    {
        combined_term.record.insert(*it);
    }   

    it = b.record.begin();
    for(; it != b.record.end(); it++)
    {
        combined_term.record.insert(*it);
    }   

    return combined_term;
}

// Find how many bit of a differ from b
// if a differ from b only in 1 bit, then it can be combined
bool Quine_Mccluskey::Can_Be_Combined(string a, string b)
{
    int difference_count;

    difference_count = 0;

    for(int i = 0; i < number_of_variables; i++)
    {
        if(a[i] != b[i])
            difference_count++;
    }

    if(difference_count == 1)
        return true;
    else
        return false;
}

// Patrick method : tranfer the POS Form to SOP Form 
// POS : POS Form ex: 5 vars(a, b, c, d, e), POS = {{1,3},{1,2,4}} = (a+c)(a+b+d)
// product : intermidiate set in order to generate product term for SOP Form
void Quine_Mccluskey::POS_to_SOP(int level, set<int>& product)
{
    if(level == number_of_POS_term){
        Patrick_SOP_term.push_back(product);
    }
    else{
        set<int>::iterator it = Patrick_POS_term[level].begin();
        for(; it != Patrick_POS_term[level].end(); it++)
        {
            int val = *it;
            if(product.find(val) == product.end())
            {
                POS_to_SOP(level + 1, product);
            }
            else
            {
                product.insert(val);
                POS_to_SOP(level + 1, product);
                product.erase(val);
            
            }
        }
        POS_to_SOP(level + 1, product);
    }
}

void Quine_Mccluskey::Find_Optimized_Product(ofstream& outfile)
{
    /*long long int min_literal_count = number_of_variables * min_terms.size();
    int min_literal_idx = -1;
    //cout << " " <<number_of_SOP_term << "f\n";
    // Iterate SOP
    for(int i = 0; i < number_of_SOP_term; i++)
    {
        int count = 0, idx;
        // iterate throught all product term
        //cout << "\n" << i << "\n";
        //cout << "~~~~~~~~~~~~~~~~~~~~~~~~\n";
        set<int>::iterator it = Patrick_SOP_term[i].begin();
        for(; it != Patrick_SOP_term[i].end(); it++)
        {
            // count the total literal in each term
            // *it
            idx = *it;
            for(int k = 0; k < number_of_variables; k++)
            {
                if(Prime_Implicant[idx].data[k] != '-')
                    count++;
            }
            //cout << *it << "\n";
        }
        if(count < min_literal_count)
        {
            min_literal_count = count;
            min_literal_idx = i;
        }
    }
    cout << "~~~~~~~~~~\n";
    cout << min_literal_count << "   " << min_literal_idx;
    print_set(Patrick_SOP_term[0]);
    print_set(Patrick_SOP_term[1]);
    
    set<int>::iterator it = Patrick_SOP_term[min_literal_idx].begin();
    for(; it != Patrick_SOP_term[min_literal_idx].end(); it++)
    {
        // output the optimized solution
        //for(int k = 0; k < number_of_variables; k++)
        //{
        cout << Prime_Implicant[*it].data << "\n";
        //}
    }*/
    
    // ============== //

    int min_count, min_idx, j;
    bool flag;
    set <int> solution;

    for(int i = 0; i < number_of_POS_term; i++)
    {
        set<int>::iterator it = Patrick_POS_term[i].begin();
    
        min_count = Prime_Implicant_literal_count[*(it)];
        min_idx = *it;
        flag = true;

        for(; it != Patrick_POS_term[i].end(); it++)
        {
            int val = *it;
            if(solution.find(val) != solution.end())
            {
                flag = false;
                break;
            }

            if(Prime_Implicant_literal_count[(*it)] < min_count)
            {
                min_idx = (*it);
                min_count = Prime_Implicant_literal_count[(*it)];
            }
        }   
        if(flag == true)
        {
            solution.insert(min_idx);
        }
    }

    //cout << "solution\n";
    //cout << solution.size() << "\n";
    int totel_literal_count = 0;
    set<int>::iterator it = solution.begin();
    for(; it != solution.end(); it++)
    {
        for(int i = 0; i < number_of_variables; i++)
        {
            if(Prime_Implicant[*it].data[i] != '-')
                totel_literal_count++;
        }
    }   

    // output literal count
    cout << totel_literal_count << "\n";
    outfile << totel_literal_count << "\n";
    
    // output the number of product term after minimize
    //cout << solution.size() << "\n";
    outfile << solution.size() << "\n";

    // output terms
    it = solution.begin();
    for(; it != solution.end(); it++)
    {
        //cout << Prime_Implicant[*it].data << "\n";
        outfile << Prime_Implicant[*it].data << "\n";
    }

}

// Generate POS from Prime Implicant found in QM Method 
void Quine_Mccluskey::Generate_Patrick_POS()
{
    int len, Prime_Imp_len;

    Prime_Imp_len = Prime_Implicant.size();

    // Iterate through all Prime Implicant
    // Find POS For Patrick Method
    for(int i = 0; i < number_of_sum_of_minterms; i++)
    {
        set <int> arr;
        Patrick_POS_term.push_back(arr);
    }

    for(int i = 0; i < Prime_Imp_len; i++)
    {
        set<int>::iterator it = Prime_Implicant[i].record.begin();
        for(; it != Prime_Implicant[i].record.end(); it++)
        {
            int val = *it;
            Patrick_POS_term[val].insert(i);
        }
    }
    
    /*for(int i = 0; i < number_of_sum_of_minterms; i++)
    {
        cout << "------------------\n";
        cout << "row" << i << ": ";
        set<int>::iterator it = Patrick_POS_term[i].begin();
        for(; it != Patrick_POS_term[i].end(); it++)
        {
            cout << *it << " ";
        }   
        cout << "\n";
    }*/
}

// solve the Patrick Method
void Quine_Mccluskey::solve_Patrick_Method(ofstream& outfile)
{
    set<int> product;
    Generate_Patrick_POS();
    number_of_POS_term = Patrick_POS_term.size();
    //POS_to_SOP(0, product);
    number_of_SOP_term = Patrick_SOP_term.size();
    Count_Literal();
    Find_Optimized_Product(outfile);
}

void Quine_Mccluskey::Count_Literal()
{
    int len = Prime_Implicant.size();
    for(int i = 0; i < len; i++)
    {
        int count = 0;
        
        for(int j = 0; j < number_of_variables; j++)
        {
            if(Prime_Implicant[i].data[j] != '-')
                count++;
        }
        Prime_Implicant_literal_count.push_back(count);
    }
}

void Quine_Mccluskey::print_set(set <int>& s)
{
    set<int>::iterator it = s.begin();
    for(; it != s.end(); it++)
    {
        cout << *it << "\n";
    }   
}
