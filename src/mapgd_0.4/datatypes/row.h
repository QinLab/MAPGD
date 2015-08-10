#ifndef _ROW_H_
#define _ROW_H_

#include <iostream>
#include <string>
#include <map>
#include <list>
#include <cstring>

#include "key.h"
#include "../typedef.h"
#include "base_keys.h"

///A class to read store abstracted data in an ... 
/**	The maximum length of a row is 65,536 bytes (i.e. a row has a 16 bit offset). 
  *
  */
class row{
private:
	char delim_;				//!< fields delimiter used for text rows.
	

	std::map <std::string, key *> keys_by_name_;	//!< references to keys mapped by keyname.
	key *keys_by_num_[255];				//!< references to keys sorted by keynum.
	std::list <key> keys_;				//!< key.
	uint8_t *data_;					//the memory for all the stuff in the row.
	size_t row_size_;				//!< the size of the row in bytes.

	void read_row_text();			//!< private member called to read a row in text mode.
	void read_row_binary();			//!< private member called to read a row in binary mode.

	void write_row_text();			//!< private member called to write a row in text mode.
	void write_row_binary();		//!< private member called to write a row in binary mode.

	std::string get_name(const key &);	//!< private member called to obtain a unique name for a column.
public:
	row (std::list<key> keys );				//!< make a row from a list of keys.
	row (std::istream &);					//!< make a row from a header line.
	row ()		;					//!< make an empty row.
	~row ();						//!< destroy a row.	

	void add_key(const key &this_key);			//!< add new column to the row. 

	key get_key(const std::string &key_num) const;		//!< gets a key by name
	key get_key(const char *key_name) const;		//!< gets a key by number
	real_key get_key(const uint8_t &key_num) const;		//!< gets a key by name

	std::list <key> get_keys(void) const;			//!< returns all keys describing the row

	void fetch(const std::string &key_name, void *dst) const;	//!< copies data from row to dst
	void fetch(const char *key_name, void *dst) const;		//!< copies data from row to dst
	void fetch(const uint8_t &key_num, void *dst) const;		//!< copies data from row to dst

	void store(const std::string &key_name, void *src);		//!< copies data from src to row
	void store(const char *key_name, void *src);			//!< copies data from src to row
	void store(const uint8_t &key_num, void *src);			//!< copies data from src to row

	const uint8_t * begin(void) const {return data_;};	//!< just pointers to a memory block
	uint8_t * begin(void){return data_;};			//!< just pointers to a memory block.
	friend std::istream& get_row(std::istream& is, row& this_row);
	friend std::istream& read_keys(std::istream& is, row& this_row);
};

inline std::istream& get_row(std::istream& is, row& this_row)
{
	return is.read((char *)this_row.data_, this_row.row_size_);			//the memory for all the stuff in the row.
}

#endif
