/*
 * main.cpp
 *
 *  Created on: Apr 7, 2018
 *      Author: x-wing
 */

#include "Dictionary.h"
#include "Util.h"

#include <iostream>
#include <typeinfo>
#include <type_traits>
#include <cstring>
#include <stdio.h>

using namespace std;


static const char DEFAULT_CHAR = 0xff;
static const int  DEFAULT_INT  = 99;
static int assertions_passed = 0;

void assert(uchar expression)
{
	if (!expression)
	{
		std::cout << "<~~ ASSERT FAILED -- Test # " << assertions_passed;
		exit(1);
	}
	++assertions_passed;
}


/**
 * INSERT   : dict[key] = data;
 * LOOKUP   : dict[key]
 * RETRIEVE : dict[key]
 */


//	dict[key] = data;
//	assert(dict[key] == data);




void test_pointer_key_overwrite()
{
	Dictionary<int*,int> dict(DEFAULT_INT);
	int  key_switcher1 = 11;
	int  key_switcher2 = 700;
	int  different_key;

	int *key1 = &key_switcher1;
	int *key2 = &key_switcher1;
	int *key3 = &key_switcher1;
	int *key4;
	int data1 = 1;
	int data2 = 2;
	int data3 = 3;

	dict[key1] = data1;

	assert(dict[key1] == data1);
	assert(dict[key2] == data1);
	assert(dict[key3] == data1);
	assert(dict[&key_switcher1] == data1);

	key_switcher1 = 22;

	assert(dict[key1] == DEFAULT_INT);
	assert(dict[key2] == DEFAULT_INT);
	assert(dict[key3] == DEFAULT_INT);
	assert(dict[&key_switcher1] == DEFAULT_INT);
	assert((key1 == key2) && (key2 == key3) && (key3 == &key_switcher1));

	dict[key2] = data2;

	assert(dict[key1] == data2);
	assert(dict[key2] == data2);
	assert(dict[key3] == data2);
	assert(dict[&key_switcher1] == data2);

	key1 = &key_switcher2;

	assert(dict[key1] == DEFAULT_INT);
	assert(dict[key2] == data2);
	assert(dict[key3] == data2);
	assert(dict[&key_switcher1] == data2);
	assert((key1 != key2) && (key2 == key3) && (key3 == &key_switcher1));

	dict[key1] = data3;

	assert(dict[key1] == data3);
	assert(dict[key2] == data2);
	assert(dict[key3] == data2);
	assert(dict[&key_switcher1] == data2);

	key3 = key1;

	assert((key1 != key2) && (key2 != key3) && (key3 == key1));
	assert(dict[key1] == data3);
	assert(dict[key2] == data2);
	assert(dict[key3] == data3);
	assert(dict[&key_switcher1] == data2);

	printf("PASSED: test_pointer_key_overwrite\n");
}




void test_array_key()
{
	const int KEY_LEN = 7;
	Dictionary<int[KEY_LEN],char> dict(DEFAULT_CHAR);

	int  i;
	char data = 'z';
	int  key1[] = {0,2,2,7,4,5,5};
	int *key2   = (int*) malloc(sizeof(int)*KEY_LEN);

	for (i = 0; i < KEY_LEN; ++i)
		key2[i] = key1[i];

	dict[key1] = data;
	assert(dict[key2] == data);

	/* change last index to test off by one */
	int test_key1[] = {0,2,2,7,3,5,5};
	assert(dict[test_key1] == DEFAULT_CHAR);

	int test_key2[] = {0,2,2,7,4,5,6};
	assert(dict[test_key2] == DEFAULT_CHAR);

	key2[KEY_LEN-1] = 6;
	dict[key2] = data;
	assert(dict[key2] == data);

	assert(dict[test_key1] == DEFAULT_CHAR);
	assert(dict[test_key2] != DEFAULT_CHAR);

	free(key2);
	printf("PASSED: test_array_key\n");
}


void test_multiple_inserts()
{
	const int INSERT_BOUND = 10;
	Dictionary<int,char> dict(DEFAULT_CHAR);

	int  i;
	int  key  = 99;
	char data1 = 'Z';
	char data2 = 'Y';
	char test  = 'F';

	dict[key] = data1;
	assert(dict[key] != DEFAULT_CHAR);
	assert(dict[key] == data1);

	dict[key] = data1;
	assert(dict[key] != DEFAULT_CHAR);
	assert(dict[key] == data1);


	for (i = 0; i < INSERT_BOUND; ++i)
		dict[key] = data1;

	assert(dict[key] != DEFAULT_CHAR);
	assert(dict[key] == data1);

	dict[key] = data2;

	assert(dict[key] != DEFAULT_CHAR);
	assert(dict[key] != data1);
	assert(dict[key] == data2);

	printf("PASSED: test_multiple_inserts\n");
}


void test_string_key()
{
	const uchar default_uchar = 0xff;

	Dictionary<string,uchar> dict(default_uchar);
	uchar data1 = 0x77;
	uchar data2 = 0x16;
	uchar test  = 0;

	dict["key1"] = data1;
	assert(dict["key1"] != default_uchar);
	assert(dict["key1"] == data1);

	dict["key2"] = data2;
	assert(dict["key2"] != default_uchar);
	assert(dict["key2"] == data2);

	assert(dict["key12"] == default_uchar);
	assert(dict["key22"] == default_uchar);
	assert(dict["key"] == default_uchar);
	assert(dict["ke"] == default_uchar);
	assert(dict["k"] == default_uchar);
	assert(dict[""] == default_uchar);

	printf("PASSED: test_string_key\n");
}


int main()
{
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";

	test_string_key();
	test_multiple_inserts();
	test_array_key();
	test_pointer_key_overwrite();

	cout << "\n\nThe program has terminated gracefully\n\n";

	return 0;
}


