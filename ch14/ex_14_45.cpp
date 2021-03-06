#include <iostream>
using std::string;
using std::cout;
using std::endl;
using std::ostream;
using std::cin;
using std::istream;
class Sales_data {
	friend ostream& operator<<(ostream &os, const Sales_data &s);
	friend istream& operator>>(istream &is, Sales_data &s);
	friend Sales_data operator+(const Sales_data &lhs, const Sales_data &rhs);
public:
	Sales_data() = default;
	Sales_data(const string &isbn, unsigned sold, double rev): bookNo(isbn), units_sold(sold), revenue(rev) {}
	Sales_data(const string &isbn): Sales_data(isbn, 0, 0.0){};
	string isbn() const {return bookNo;}
	explicit operator string() const {
		return bookNo;
	}
	explicit operator double() const {
		return avg_price();
	}
	Sales_data& operator+=(const Sales_data &rhs);

private:
	inline double avg_price() const;
	string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
};
Sales_data& Sales_data::operator+=(const Sales_data &rhs) {
	units_sold += rhs.units_sold;
	revenue += rhs.revenue;
	return *this;
}

inline double Sales_data::avg_price() const {
	return units_sold ? revenue/units_sold : 0.0;
}

ostream& operator<<(ostream &os, const Sales_data &s) {
	os << s.bookNo << " " << s.units_sold << " " << s.revenue << "" << s.avg_price();
	return os;
}
istream& operator>>(istream &is, Sales_data &s) {
	double price;
	is >> s.bookNo >> s.units_sold >> price;
	if (is)
		s.revenue = price * s.units_sold;
	else
		s = Sales_data();
	return is;
}
Sales_data operator+(const Sales_data &lhs, const Sales_data &rhs) {
    Sales_data ret = lhs;
    ret += rhs;
    return ret;
}
