#include<iostream>
#include<string>
#include<map>
#include<vector>
#include<sstream>
#include "CMarket.h"
#include "CPost.h"

#pragma region "Other Functions"
// Everytime an error occurs, this function is called with an int argument to specify which error message is printed.
void Market::flag_error(int x = 4) {
	std::cout << "> ";
	switch (x) {
		case 0: std::cout << "UNAUTHORIZED"; break;
		case 1: std::cout << "UNKNOWN_ORDER"; break;
		case 2: std::cout << "UNKNOWN_DEALER"; break;
		case 3: std::cout << "UNKNOWN_COMMODITY"; break;
		case 4: std::cout << "INVALID_MESSAGE"; break;
	}
	std::cout << "\n";
}

// Given an iterator to an element of the order list, it prints the details of the order.
void Market::order_info(const std::map<int, Post>::iterator &it) {
	std::cout << "> " << it->first << " " << it->second.get_dealer() << " " << it->second.get_side()
		<< " " << it->second.get_commodity() << " " << it->second.get_amount() << " " << it->second.get_price() << "\n";
}
#pragma endregion

void Market::process_input(const std::string& txt) {
	if (txt.length() > 255) { flag_error(); return; }

	// Splits the input string into a vector of words.
	std::stringstream in(txt);
	std::string word;
	std::vector<std::string> words;
	while (in >> word) words.push_back(word);

	// Checks that there are at least two words and that the first is a valid dealer.
	if (words.size() < 2) { flag_error(); return; }
	if (!is_dealer_accepted(words[0])) { flag_error(Unknown_Dealer); return; }

	// Calls the correct function. Thus when inside one of these functions, the first two words are known to be correct. The rest of the command will be checked in each function.
	if (words[1] == "POST")	{ post(words); }
	else if (words[1] == "REVOKE") { revoke(words); }
	else if (words[1] == "CHECK") { check(words); }
	else if (words[1] == "LIST") { list(words); }
	else if (words[1] == "AGGRESS") { aggress(words); }
	else flag_error(); // If the second word is not a valid input, this will catch it.
	return;
}

#pragma region "Command Functions"
void Market::post(const std::vector<std::string> &words) {
	static int assign_ID = 1; // Create a counter for the lifetime of the program that will be used to assign ID numbers to each post.

	// Perform all necessary checks to make sure that the command is valid. Return with an error message if anything is not correct.
	if (words.size() != 6) { flag_error(); return; } // Checks that there is the correct number of words (6).
	if (!is_side_accepted(words[2])) { flag_error(); return; } // Checks that the side is valid.
	if (!is_commodity_accepted(words[3])) { flag_error(Unknown_Commodity); return; } // Checks that the commodity is valid.
	if (!is_positive_int(words[4])) { flag_error(); return; } // Checks that the amount is a valid postive integer.
	if (!is_positive_float(words[5])) { flag_error(); return; } // Checks that the price is a valid positive float.

	// Convert the numbers.
	int amount = atoi(words[4].c_str());
	double price = atof(words[5].c_str());
	if (amount == 0) { flag_error(); return; }

	// Create and add post.
	Post X(words[0], words[2], words[3], amount, price);
	order_list[assign_ID] = X;

	// Print that the post has been created.
	std::cout << "> " << assign_ID << " " << words[0] << " " << words[2] << " " << words[3] << " " << amount << " " << price << " HAS BEEN POSTED\n";

	// Increment the ID counter after posting.
	++assign_ID;
}

void Market::revoke(const std::vector<std::string> &words) {
	// Checks that the command is the correct length and converts the integer.
	if (words.size() != 3) { flag_error(); return; }
	if (!is_positive_int(words[2])) { flag_error(); return; }
	int ID = atoi(words[2].c_str());

	// Checks if the order exists and the dealer is correct.
	iter = order_list.find(ID);
	if (iter == order_list.end()) { flag_error(Unknown_Order); return; }
	if (iter->second.get_dealer() != words[0]) { flag_error(Unauthorized); return; }

	// Removes the order.
	order_list.erase(ID);
	std::cout << "> " << ID << " HAS BEEN REVOKED\n";
}

void Market::check(const std::vector<std::string> &words) {
	// Checks that the command is the correct length and converts the integer.
	if (words.size() != 3) { flag_error(); return; }
	if (!is_positive_int(words[2])) { flag_error(); return; }
	int ID = atoi(words[2].c_str());

	// Checks if the order exists and the dealer is correct.
	iter = order_list.find(ID);
	if (iter == order_list.end()) { flag_error(Unknown_Order); return; }
	if (iter->second.get_dealer() != words[0]) { flag_error(Unauthorized); return; }

	// Displays the appropriate output.
	if (iter->second.get_amount() > 0) { order_info(iter); }
	else { std::cout << "> " << ID << " HAS BEEN FILLED\n"; }
}

void Market::list(const std::vector<std::string> &words) {
	// If there is only two words, then there are no search restrictions and the whole list is printed.
	if (words.size() == 2) {
		// Run through the order list and print each post (that has a positive quantity).
		for (iter = order_list.begin(); iter != order_list.end(); iter++) {
			if (iter->second.get_amount() > 0) { order_info(iter); }
		}
	}

	// If there are three words, then the third word is checked to be an accepted commodity. Then the search is filtered accordingly.
	else if (words.size() == 3) {
		// Check that the command is valid, return an error message if not.
		if (!is_commodity_accepted(words[2])) { flag_error(Unknown_Commodity); return; }

		// Run through the order list and print the posts that satisfy the criteria (and that have a positive quantity).
		for (iter = order_list.begin(); iter != order_list.end(); iter++) {
			if ((iter->second.get_amount() > 0) && (iter->second.get_commodity() == words[2])) { order_info(iter); }
		}
	}

	// If there are four words, the third is checked to be an accepted commodity and the fourth is checked to be an accepted dealer. Then the search is filtered accordingly.
	else if (words.size() == 4) {
		// Check that the command is valid, return an error message if not..
		if (!is_commodity_accepted(words[2])) { flag_error(Unknown_Commodity); return; }
		if (!is_dealer_accepted(words[3])) { flag_error(Unknown_Dealer); return; }

		// Run through the order list and print the posts that satisfy the criteria (and that have a positive quantity).
		for (iter = order_list.begin(); iter != order_list.end(); iter++) {
			if ((iter->second.get_amount() > 0) && (iter->second.get_commodity() == words[2]) && (iter->second.get_dealer() == words[3])) { order_info(iter); }
		}
	}
	std::cout << "> END OF LIST\n";
}

void Market::aggress(const std::vector<std::string> &words) {
	if (words.size() % 2 != 0) { flag_error(); return; } // A valid command must have an even number of words, otherwise it is invalid.

	// Convert all numbers to integers and store them in a vector.
	std::vector<int> numbers(words.size() - 2);
	for (int i = 2; i < words.size(); i++) {
		if (!is_positive_int(words[i])) { flag_error(); return; } // Checks that the amounts and IDs are valid positive integers.
		numbers[i - 2] = atoi(words[i].c_str());
	}

	// Loops through all the orders in the list. (Note that each order comprises of two numbers, the ID and the amount, which is why the for loop increments by 2 each time).
	for (int i = 0; i < numbers.size(); i += 2) {
		iter = order_list.find(numbers[i]);
		if (iter == order_list.end()) { flag_error(Unknown_Order); continue; } // If the order ID cannot be found, an error is shown and the loop skips to the next one.
		if (numbers[i + 1] > iter->second.get_amount()) { flag_error(Unauthorized); continue; } // numbers[i+1] is the amount wanted so must be less than or equal to the amount available.
		iter->second.subtract_amount(numbers[i + 1]);

		// Output result.
		std::string action, tf;
		if (iter->second.get_side() == "BUY") {
			action = "SOLD";
			tf = "TO";
		}
		else {
			action = "BOUGHT";
			tf = "FROM";
		}
		std::cout << "> " << action << " " << numbers[i + 1] << " " << iter->second.get_commodity() << " @ " << iter->second.get_price() << " " << tf << " " << iter->second.get_dealer() << "\n";
	}
}
#pragma endregion

#pragma region "Check Functions"
// Checks if the given string is the name of one of the accepted dealers, returning true if so.
bool Market::is_dealer_accepted(const std::string &word) {
	for (int i = 0; i < accepted_dealers.size(); i++) {
		if (word == accepted_dealers[i]) return true;
	}
	return false;
}

// Checks if the given string is the name of one of the accepted commodities, returning true if so.
bool Market::is_commodity_accepted(const std::string &word) {
	for (int i = 0; i < accepted_commodities.size(); i++) {
		if (word == accepted_commodities[i]) return true;
	}
	return false;
}

// Checks if the given string is either BUY or SELL, returning true if so.
bool Market::is_side_accepted(const std::string &word) {
	if ((word == "BUY") || (word == "SELL")) return true;
	else return false;
}

// Checks if the given string is a valid positive integer.
bool Market::is_positive_int(const std::string &word) {
	for (int i = 0; i < word.length(); i++) {
		if (!isdigit(word[i])) { return false; }
	}
	return true;
}

// Checks if the given string is a valid positive float.
bool Market::is_positive_float(const std::string &word) {
	int decimal_points = 0;
	for (int i = 0; i < word.length(); i++) {
		if (word[i] == '.') { ++decimal_points; }
		else if (!isdigit(word[i])) {
			return false;
		}
	}
	if (decimal_points > 1) { return false; }
	return true;
}
#pragma endregion