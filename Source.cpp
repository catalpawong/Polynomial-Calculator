#include <iostream>
#include <string>
#include <cmath>

using namespace std;

bool isNum(string str) {
	int dot = 0, j, k, flag = 0, l = str.size();
	if (l == 1)
		if (str[0]<'0' || str[0]>'0')
			return false;//To avoid counting +/- as numbers.
	for (j = 0; j < l; j++) {
		if (str[j] == '.')
			dot++;
	}//Check if there're more than one dots in the target string.
	if (dot > 1)
		return false;
	if (str[0] < '0' || str[0] > '9')
		if (str[0] != '+' && str[0] != '-')
			return false;//The 1st character of a number can be a + or -.
	for (k = 1; k < l; k++) {
		if (str[k] < '0' || str[k] > '9')
			if (str[k] != '.')
				flag++;//Check the rest characters, they can only be numbers or dots.
	}
	if (flag)
		return false;
	else
		return true;
}

double intoNum(string str) {
	if (!isNum(str))
		return -99999999;
	int dot = 0, l = str.size(), k, k_0 = l;
	double num = 0;
	if (l == 1) {
		num = str[0] - '0';
		return num;
	}		
	for (k = 0; k < l; k++) {
		if (str[k] == '.') {
			dot++;
			k_0 = k;
		}
	}
	int i, j;
	for (i = 1; i < k_0; i++) {
		num += (str[k_0 - i] - '0') * pow(10, i - 1);
	}
	if (dot) {
		for (j = 1; j <= (l - k_0 - 1); j++) {
			num += (str[k_0 + j] - '0') * pow(10, -j);
		}
	}
	if (str[0] >= '0'&&str[0] <= '9')
		num += (str[0] - '0')*pow(10, k_0 - 1);
	else if (str[0] == '-')
		num = -num;
	return num;
}

bool isExp(string str) {
	if (str == "\0")
		return false;
	int k, f = 0;
	for (k = 0; k < str.size(); k++) {
		if (str[k] < '0' || str[k] > '9')
			f++;
	}
	if (f>0) 
		return false;
	else 
		return true;
}//We restrict exponents to non-negative integers only.

int intoInt(string str) {
	int s = 0, k, l = str.size();
	for (k = 0; k < l; k++) {
		s += (str[k] - '0')*pow(10, (l - k - 1));
	}
	return s;
}

string justify(string str) {
	if (str[0] != '-' && str[0] != '+')
		str = "+" + str;
	string justified_str = "\0";
	int l = str.size();
	int * connect = nullptr;
	int k;
	int count_connect = 0;
	for (k = 0; k < l; k++) {
		if (str[k] == '+' || str[k] == '-')
			count_connect++;
	}//To find out the number of possible terms in the given polynomial.
	connect = new int[count_connect];
	int i = 0;
	for (k = 0; k < l; k++) {
		if (str[k] == '+' || str[k] == '-') {
			connect[i] = k;
			i++;
		}
	}
	int q,
		r,
		con,
		nect,
		temp_len,
		flag_const,
		num_of_const = 0;
	string temp_term;
	for (q = 0; q < count_connect; q++) {
		con = connect[q];
		if (q + 1 > count_connect - 1)
			nect = l;
		else
			nect = connect[q + 1];
		temp_term = str.substr(con, nect - con);		
		temp_len = temp_term.size();
		flag_const = 0;
		if (temp_term[temp_len - 1] == 'x')
			temp_term += "^1";//If a term ends in "x", change it into a form that ends in "x^1".
		for (r = 0; r < temp_len; r++) {
			if (temp_term[r] == 'x')
				flag_const++;
		}
		if (flag_const == 0)
			if (temp_term != "+" && temp_term != "-") {
				temp_term = temp_term + "x^0";
			}//If a term has no "x" in it, see it as a term of constant, then change it into a form that ends in "x^0".
		if (temp_term[1] == 'x')
			temp_term.insert(1,"1");//If a term has no number between +/- and "x", see its coefficient as 1, the change it into a form that starts with "+/-1x".		

		justified_str += temp_term;
	}
	delete[] connect;
	connect = nullptr;
	return justified_str;	
}

bool isPoly(string str) {
	str = justify(str);
	int l = str.size();	
	int * unknow = nullptr,
		* connect = nullptr;
	int k;
	int count_unknow = 0,
		count_connect = 0;
	for (k = 0; k < l; k++) {
		if (str[k] == '+' || str[k] == '-')
			count_connect++;		
		else if (str[k] == 'x')
			count_unknow++;		
	}//To find out the number of terms in the given polynomial.
	if (count_unknow != count_connect)
		return false;
	unknow = new int[count_unknow];
	connect = new int[count_connect];
	int i = 0,
		j = 0;
	for (k = 0; k < l; k++) {
		if (str[k] == '+' || str[k] == '-') {
			connect[i] = k;
			i++;
		}			
		else if (str[k] == 'x') {
			unknow[j] = k;
			j++;
		}			
	}
	int flag_1 = 0,
		flag_2 = 0,
		flag_3 = 0,
		flag = 0,//We use 3 flags to check 3 different part so that we can easily find the exact part that problems occur.
		len = count_unknow,
		q, con, un, nect;
	string temp_coe, temp_exp;
	for (q = 0; q < len; q++) {
		con = connect[q];
		if (q + 1 > len - 1)
			nect = l;
		else
			nect = connect[q + 1];//Make sure the string in the place of the last exponent can be accessed and checked.
		un = unknow[q];
		temp_coe = str.substr(con, un - con);//We intercept the string in the place of the coefficient of the q-th term here.
		temp_exp = str.substr(un + 2, nect - un - 2);//We intercept the string in the place of the exponent of the q-th term here.
		if (str[un + 1] != '^')
			flag_1++;
		if (!isNum(temp_coe))
			flag_2++;
		if (!isExp(temp_exp))
			flag_3++;//For every term, it should be in the form of "number"+"x"+"^"+"integer".
	}
	delete [] unknow;
	delete [] connect;
	unknow = nullptr,
	connect = nullptr;
	flag += flag_1;
	flag += flag_2;
	flag += flag_3;
	if (flag > 0)
		return false;
	else
		return true;
}//Check whether a string represents a polynomial in the given form.

double* standardize(string str, int &size) {
	double * poly = nullptr;
	str = justify(str);
	if (!isPoly(str))
		return poly;
	int l = str.size();
	int *unknow = nullptr,
		*connect = nullptr;
	int k,
		num_of_terms = 0;
	for (k = 0; k < l; k++) {
		if (str[k] == '+' || str[k] == '-')
			num_of_terms++;		
	}
	unknow = new int[num_of_terms];
	connect = new int[num_of_terms];
	int i = 0,
		j = 0;
	for (k = 0; k < l; k++) {
		if (str[k] == '+' || str[k] == '-') {
			connect[i] = k;
			i++;
		}
		else if (str[k] == 'x') {
			unknow[j] = k;
			j++;
		}
	}
	int q, con, un, nect;
	double * coe = nullptr;
	int * exp = nullptr;
	coe = new double[num_of_terms];
	exp = new int[num_of_terms];
	for (q = 0; q < num_of_terms; q++) {
		con = connect[q];
		if (q + 1 > num_of_terms - 1)
			nect = l;
		else
			nect = connect[q + 1];
		un = unknow[q];
		coe[q] = intoNum(str.substr(con, un - con));
		exp[q] = intoInt(str.substr(un + 2, nect - un - 2));	
	}//Use an arry to store the given polynomial.
	delete [] unknow;
	delete [] connect;
	unknow = nullptr;
	connect = nullptr;
	int max_len = exp[0];
	int z;
	for (z = 0; z < num_of_terms; z++) {
		if (exp[z] > max_len)
			max_len = exp[z];
	}
	max_len += 1;//The number of possible slots shoud be one more than the number of the highest exponent.
	size = max_len;//Keep the size information of given polynomial so we can use it outside this function;
	poly = new double[max_len];
	int t,s;
	for (t = 0; t < max_len; t++) {
		poly[t] = 0;
		for (s = 0; s < num_of_terms; s++) {
			if (t == exp[s])
				poly[t] += coe[s];
		}
	}
	delete[] unknow;
	delete[] connect;
	unknow = nullptr,
	connect = nullptr;
	return poly;
}

double *addPoly(double *poly_1, double *poly_2, int size_1, int size_2,int &size){
	double * sum = nullptr;
	int mark_longer = 1;
	int longer = size_1,
		shorter = size_2;
	if (size_1 < size_2) {
		longer = size_2;	
		shorter = size_1;
		mark_longer = 2;
	}
	size = longer;
	sum = new double[longer];
	int i;
	for (i = 0; i < shorter; i++) {
		sum[i] = poly_1[i] + poly_2[i];
	}
	int j;
	for (j = shorter; j < longer; j++) {
		if (mark_longer == 1)
			sum[j] = poly_1[j];
		if (mark_longer == 2)
			sum[j] = poly_2[j];
	}	
	return sum;
}

double *subtractPoly(double *poly_1, double *poly_2, int size_1, int size_2, int &size) {
	double * difference = nullptr,
		   * subtractor = nullptr;
	subtractor = new double[size_2];	
	int i;
	for (i = 0; i < size_2; i++) {
		subtractor[i] = 0 - poly_2[i];
	}
	difference = addPoly(poly_1, subtractor, size_1, size_2, size);//We all know that subtract something means add its negation.
	delete[] subtractor;
	subtractor = nullptr;
	return difference;
}

double *multiPoly(double *poly_1, double *poly_2, int size_1, int size_2, int &size) {
	double * product = nullptr;
	size = size_1 + size_2 - 1;
	product = new double[size];
	int z;
	for (z = 0; z < size; z++) {
		product[z] = 0;
	}//Initialize the polynomial of product.
	int k,
		r,
		len;
	double * temp = nullptr;
	for (k = 0; k < size_2; k++) {
		len = size_1 + k;
		temp = new double[len];
		for (r = 0; r < len; r++) {
			if (r < k) {
				temp[r] = 0;
			}
			else {
				temp[r] = poly_1[r - k] * poly_2[k];
			}
		}//Calculate the first polynomial times the k-th term of the second polynomial.
		product = addPoly(product, temp, size, len, size);//Then add all temporary polynomials together.
		delete[] temp;
		temp = nullptr;
	}
	return product;
}

bool printPoly(double *poly, int size) {	
	if (poly==nullptr)
		return false;
	double const epsilon = pow(10, -13);
	int r=0;
	while (poly[r] == 0) {
		r++;
	}//We use r to point out the first non-zero term in the given polynomial.
	if (r >= size) {//If all the coefficients are 0, then the given polynomial equals to 0.
		cout << 0;
		return true;
	}		
	if (r == 0) {//We don't print "x" when facing a term of constant.
		cout << poly[r];
		if (size < 2)
			return true;//Check if there are still terms remaining.
		//We print "x" instead of "x^1" for a better look.
		if (poly[r + 1] > 0) {
			cout << "+";
			if (fabs(poly[r + 1] - 1) < epsilon)//Check whether the coefficient is 1 or -1.
				cout << "x";//To print "1x" as "x" for a better look.
			else
				cout << poly[r + 1] << "x";
		}
		else if (poly[r + 1] < 0) {
			if (fabs(poly[r + 1] + 1) < epsilon)//Check whether the coefficient is 1 or -1.
				cout << "-x";//To print "1x" as "x" for a better look.
			else
				cout << poly[r + 1] << "x";
		}
	}
	else if (r == 1) {
		if (fabs(poly[r] - 1) < epsilon)//Check whether the coefficient is 1 or -1.
			cout << "x";//To print "1x" as "x" for a better look.
		else if (fabs(poly[r] + 1) < epsilon)//Check whether the coefficient is 1 or -1.
			cout << "-x"; //To print "1x" as "x" for a better look.
		else
			cout << poly[r] << "x";		
		if (size < 3)
			return true;//Check if there are still terms remaining.
		if (poly[r+1] < 0) {
			if (fabs(poly[r + 1] + 1) < epsilon)//Check whether the coefficient is 1 or -1.
				cout << "-x^" << r + 1;//To print "-1x" as "-x" for a better look.
			else
				cout << poly[r+1] << "x^" << r+1;
		}
		else if (poly[r+1] > 0) {
			cout << "+";
			if (fabs(poly[r + 1] - 1) < epsilon)//Check whether the coefficient is 1 or -1.
				cout << "x^" << r + 1;//To print "1x" as "x" for a better look.
			else
				cout << poly[r+1] << "x^" << r+1;
		}
	}
	else {
		if (fabs(poly[r] - 1) < epsilon)
			cout << "x^" << r;
		else if (fabs(poly[r] + 1) < epsilon)//Check whether the coefficient is 1 or -1.
			cout << "-x^" << r;//To print "1x" as "x" for a better look.
		else
			cout << poly[r] << "x^" << r;
		if ((r + 1) > size - 1)
			return true;//Check if there are still terms remaining.
		if (poly[r + 1] < 0) {
			if (fabs(poly[r + 1] + 1) < epsilon)//Check whether the coefficient is 1 or -1.
				cout << "-x^" << r + 1;//To print "-1x" as "-x" for a better look.
			else
				cout << poly[r + 1] << "x^" << r + 1;
		}
		else if (poly[r + 1] > 0) {
			cout << "+";
			if (fabs(poly[r + 1] - 1) < epsilon)//Check whether the coefficient is 1 or -1.
				cout << "x^" << r + 1;//To print "1x" as "x" for a better look.
			else
				cout << poly[r + 1] << "x^" << r + 1;
		}
	}
	if ((r + 2) > size - 1)
		return true;//Check if there are still terms remaining.
	int i;
	for (i = r + 2; i < size; i++) {//Since r+2>=2, we can print "x^" without hesitation now.
		if (poly[i] < 0) {
			if (fabs(poly[i] + 1) < epsilon)//Check whether the coefficient is 1 or -1.
				cout << "-x^" << i;//To print "-1x" as "-x" for a better look.
			else
				cout << poly[i] << "x^" << i;
		}
		else if (poly[i] > 0) {
			cout << "+";
			if (fabs(poly[i] - 1) < epsilon)//Check whether the coefficient is 1 or -1.
				cout << "x^" << i;//To print "1x" as "x" for a better look.
			else
				cout << poly[i] << "x^" << i;
		}
	}	
	return true;
}

void menu() {
	cout << "Welcome to the Polynomial Calculator!\n";
	cout << "(1) Addition\n(2) Subtraction\n(3) Multiplication\n(4) Exit\n";
	cout<< "Please enter 1-4 to select the corresponding option:\n";
}

bool isLegal_Option(string str) {
	if (str.size() > 1)
		return false;
	char option = str[0];
	if (option > '4')
		return false;
	else if (option < '1')
		return false;
	else
		return true;
}

void guide(int i) {
	if (i == 1) {
		cout << "Please enter the first polynomial:\n";
	}
	if (i == 2) {
		cout << "Please enter the second polynomial:\n";
	}
	if (i == 3) {
		cout << "You select the ADDITION function.\n";
	}

	if (i == 4) {
		cout << "You select the SUBTRACTION function.\n";
	}
	if (i == 5) {
		cout << "You select the MULTIPLICATION function.\n";
	}
	if (i == 6) {
		cout << "You select to EXIT the program.\n";
	}
}



void main() {
	string option;
	int option_num;
	bool menu_flag = true;
	string polynomial_1, polynomial_2;
	int len_1 = 0,
		len_2 = 0,
		len = 0;
	double *poly_1 = nullptr,
		*poly_2 = nullptr,
		*poly = nullptr;
	while (menu_flag) {
		do {
			menu();
			cin >> option;
			if (!isLegal_Option(option))
				system("cls");
		} while (!isLegal_Option(option));
		option_num = option[0] - '0';
		switch (option_num) {
		case 1: {
			guide(3);
			do {
				guide(1);
				cin >> polynomial_1;
				if (!isPoly(polynomial_1)) {
					system("cls");
					menu();
					cout << "1\n";
					guide(3);
				}
			} while (!isPoly(polynomial_1));
			poly_1 = standardize(polynomial_1, len_1);			
			do {
				guide(2);
				cin >> polynomial_2;
				if (!isPoly(polynomial_2)) {
					system("cls");
					menu();
					cout << "1\n";
					guide(3);
					guide(1);
					cout << polynomial_1 << endl;					
				}
			} while (!isPoly(polynomial_2));
			poly_2 = standardize(polynomial_2, len_2);
			poly = addPoly(poly_1, poly_2, len_1, len_2, len);
			cout << "Here's the result:\n";
			printPoly(poly, len);
			cout << "\n";
			delete[] poly_1;
			delete[] poly_2;
			poly_1 = nullptr;
			poly_2 = nullptr;
			break;
		}
		case 2:{
			guide(4);
			do {
				guide(1);
				cin >> polynomial_1;
				if (!isPoly(polynomial_1)) {
					system("cls");
					menu();
					cout << "2\n";
					guide(4);
				}
			} while (!isPoly(polynomial_1));
			poly_1 = standardize(polynomial_1, len_1);
			do {
				guide(2);
				cin >> polynomial_2;
				if (!isPoly(polynomial_2)) {
					system("cls");
					menu();
					cout << "2\n";
					guide(4);
					guide(1);
					cout << polynomial_1 << endl;
				}
			} while (!isPoly(polynomial_2));
			poly_2 = standardize(polynomial_2, len_2);
			poly = subtractPoly(poly_1, poly_2, len_1, len_2, len);
			cout << "Here's the result:\n";
			printPoly(poly, len);
			cout << "\n";
			delete[] poly_1;
			delete[] poly_2;
			poly_1 = nullptr;
			poly_2 = nullptr;
			break;
		}
		case 3:{
			guide(5);
			do {
				guide(1);
				cin >> polynomial_1;
				if (!isPoly(polynomial_1)) {
					system("cls");
					menu();
					cout << "3\n";
					guide(5);
				}
			} while (!isPoly(polynomial_1));
			poly_1 = standardize(polynomial_1, len_1);
			do {
				guide(2);
				cin >> polynomial_2;
				if (!isPoly(polynomial_2)) {
					system("cls");
					menu();
					cout << "3\n";
					guide(5);
					guide(1);
					cout << polynomial_1 << endl;
				}
			} while (!isPoly(polynomial_2));
			poly_2 = standardize(polynomial_2, len_2);
			poly = multiPoly(poly_1, poly_2, len_1, len_2, len);
			cout << "Here's the result:\n";
			printPoly(poly, len);
			cout << "\n";
			delete[] poly_1;
			delete[] poly_2;
			poly_1 = nullptr;
			poly_2 = nullptr;
			break;
		}
		default:{
			guide(6);
			cout << "Thank you for using the Polynomial Calculator!" << endl;
			return;
		}
		}
		delete[] poly;
		poly = nullptr;
		system("pause");
		system("cls");
	}
}




