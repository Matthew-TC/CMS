#pragma once

class Post {
public:
	Post(std::string d, std::string s, std::string c, int a, double p) : dealer(d), commodity(c), side(s), amount(a), price(p) {} // Constructor.
	Post(const Post &x) : dealer(x.dealer), side(x.side), commodity(x.commodity), amount(x.amount), price(x.price) {} // Copy-Constructor.
	Post() {} // Required to prevent errors, but will not be used.

	// Mutator.
	void subtract_amount(int x) { amount -= x; } // Removes x off the amount. Checking that x < amount is done on the market class rather than here.

	// Accessors.
	const std::string& get_dealer() { return dealer; }
	const std::string& get_side() { return side; }
	const std::string& get_commodity() { return commodity; }
	int	get_amount() { return amount; }
	double get_price() { return price; }

private:
	std::string dealer;
	std::string side;
	std::string commodity;
	int	amount;
	double price;
};
