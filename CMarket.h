#pragma once
#include<map>
#include<vector>
class Post;

class Market {
public:
	// Receives a command, splits it into a vector of words, checks that the first and second words are valid, then sends the vector of words to the correct command function.
	void process_input(const std::string &text);

private:
	// Where all the posts on the market are stored.
	std::map<int, Post> order_list;
	std::map<int, Post>::iterator iter;

	// Command Functions.
	void post(const std::vector<std::string> &words);
	void revoke(const std::vector<std::string> &words);
	void check(const std::vector<std::string> &words);
	void list(const std::vector<std::string> &words);
	void aggress(const std::vector<std::string> &words);

	// Check Functions: Check if the dealers, commodities and sides are valid, and that given strings are valid integers and floats.
	bool is_dealer_accepted(const std::string &word);
	bool is_commodity_accepted(const std::string &word);
	bool is_side_accepted(const std::string &word);
	bool is_positive_int(const std::string &word);
	bool is_positive_float(const std::string &word);

	// Other Functions: Error outputs, and order info outputs used in the list function.
	enum errors { Unauthorized, Unknown_Order, Unknown_Dealer, Unknown_Commodity }; // Used as arguments for flag_error().
	void flag_error(int x /* = 4 */); // Calling with no argument outputs 'INVALID MESSAGE'.
	void order_info(const std::map<int, Post>::iterator &it);

	// Constant vectors of accepted dealers and commodities.
	const std::vector<std::string> accepted_dealers = { "DB", "JPM", "UBS", "RBC", "BARX", "MS", "CITI", "BOFA", "RBS", "HSBC" };
	const std::vector<std::string> accepted_commodities = { "GOLD", "SILV", "PORK", "OIL", "RICE" };
};