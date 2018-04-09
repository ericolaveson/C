/*
 * Util.h
 *
 *  Created on: Apr 8, 2018
 *      Author: x-wing
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <iostream>
#include <string>


typedef unsigned char uchar;

#define comma ,
#define Generic template<class type>



Generic bool is_string(type datum)
{
	return (typeid(typeof(datum)) == typeid(std::string));
}

Generic bool is_array()
{
	return std::is_array<type>::value;
}

Generic bool is_pointer(type datum)
{
	return std::is_pointer<typeof(datum)>::value;
}

//Generic int length(type datum)
//{
//	int len;
//
//
//	if (is_string<type>(datum))
//	{
//		std::string *str = reinterpret_cast<std::string*>(&datum);
//
//
//		std::cout << "we have a string\n";
//		len = str->length();
//	}
//	else if (is_array<type>())
//	{
//		std::cout << "we have an array\n";
//		len = sizeof(type);
//	}
//	else
//	{
//		std::cout << "we have a regular key\n";
//		len = sizeof(type);
//	}
//
//
//	std::cout << "len: " << len << '\n';
//
//	return is_string<type>(datum) ? (*(std::string*)&datum).length() : sizeof(type);
//}




#endif /* UTIL_H_ */
