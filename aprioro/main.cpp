/*
c : {1,2} {1,3} {1,2,3} etc.

l : large item set

*/
//header file---------------

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

//--------------------------

//namespace-----------------
using namespace std;
//--------------------------

//struct--------------------
typedef struct set_node
{
	int *set_name;
	int count;
	struct set_node *next;
} c_set_node;
//--------------------------
//functions-----------------
c_set_node *force_l2(int min_support);
c_set_node *find_c(c_set_node *l_set_header);
c_set_node *find_l(c_set_node *c_set_header, int min_support);
int chartoint(char *input);
char *data_scanner(char *tmp, char *source);
void print(c_set_node *l_set_header);
//--------------------------
//global variable-----------
int layer = 0;
int item_number = 500;
bool terminate_flag = 0;
int large_set_counter = 0;
char file_name[] = "C:\\T15I6N0.5KD1K.txt";
//--------------------------

int main()
{

	int min_support;
	//cout << "pls enter min support :" ;
	cin >> min_support;
	/*cout << " pls enter item_number ";
	cin >> item_number;*/

	clock_t start, finish;
	double duration;
	start = clock();

	c_set_node *ptr, *l_ptr;
	l_ptr = force_l2(min_support);
	print(l_ptr);
	int flag = 0;
	while (1)
	{
		//cout << large_set_counter << endl;
		if (terminate_flag == 1) break;
		ptr = find_c(l_ptr);
		l_ptr = ptr;
		/*
		if(flag == 0)
		{
		c_set_node *test_ptr = l_ptr;
		while(test_ptr != NULL)
		{
		for (int failfish = 0 ; failfish < layer ; failfish++) cout << test_ptr -> set_name[failfish] << "|";
		cout << endl;
		test_ptr = test_ptr -> next;
		}
		flag = 1;
		}
		*/
		if (terminate_flag == 1) break;
		ptr = find_l(l_ptr, min_support);
		print(ptr);
		l_ptr = ptr;
		//cout << large_set_counter << endl;
	}
	finish = clock();
	cout << "amount of large set : " << large_set_counter << endl;
	duration = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "run time : " << duration << " seconds." << endl;
	cout << "請按任意鍵繼續. . .";
	cin.get();
	cin.get();
}

c_set_node *force_l2(int min_support)
{
	large_set_counter = 0;
	fstream infile;
	infile.open(file_name, ios::in);
	char data_row[512];
	char *scanner;
	char numchar[8];
	int real_num;
	int count = 0;
	int *item = new int[item_number];
	for (int i = 0; i < item_number; i++) item[i] = 0;
	data_row[0] = '\0';
	while (!infile.eof())	/*this loop convert string to a number array , i'm sure this loop have no bug.*/
	{
		infile.getline(data_row, 512, '\n');
		scanner = data_row;
		numchar[0] = '\0';
		while (*scanner != '\0')
		{
			scanner = data_scanner(numchar, scanner);
			if (*scanner == ',') scanner += 1;
			real_num = chartoint(numchar);
			item[real_num] += 1;
			numchar[0] = '\0';
		}
		data_row[0] = '\0';
	}
	for (int i = 0; i < item_number; i++)
	{
		if (item[i] < min_support) item[i] = 0;
		else
		{
			cout << i << "| count : " << item[i] << endl;
			count += 1;
			large_set_counter += 1;
		}
	}
	int *candidate_array = new int[count];
	int candidate_number = 0;
	for (int i = 0; i < item_number; i++)
	{
		if (item[i] != 0)
		{
			candidate_array[candidate_number] = i;
			candidate_number += 1;
		}
		else;
	}
	c_set_node *c_set_header = NULL;
	c_set_node *c_set_rear = NULL;
	c_set_node *c_set_builder;
	int alpha = 0;
	int beta = 1;
	c_set_builder = new c_set_node;		/*creat a fake node to let us can easily creat linked list*/
	c_set_header = c_set_builder;
	c_set_rear = c_set_builder;
	c_set_builder->next = NULL;
	c_set_builder->set_name = new int[2];
	for (alpha = 0; alpha < count - 1; alpha++) /*this for loop force create all c2 node*/
	{
		for (beta = alpha + 1; beta < count; beta++)
		{
			c_set_builder = new c_set_node;
			c_set_rear->next = c_set_builder;
			c_set_builder->set_name = new int[2];
			c_set_builder->count = 0;
			c_set_builder->set_name[0] = candidate_array[alpha];
			c_set_builder->set_name[1] = candidate_array[beta];
			c_set_rear = c_set_builder;
			c_set_builder->next = NULL;
		}
	}
	c_set_builder = c_set_header;	/*now we remove the fake node*/
	c_set_header = c_set_header->next;
	delete c_set_builder;
	c_set_node *l_set_header = c_set_header;
	c_set_node *l_set_pointer = c_set_header;
	int int_tmp[100];
	int data_count = 0;
	infile.close();
	infile.clear();
	infile.open(file_name, ios::in);
	data_row[0] = '\0';
	numchar[0] = '\0';
	while (!infile.eof())	/*this loop will count all c2 node's counter*/
	{
		infile.getline(data_row, 512, '\n');
		if (data_row[0] == '\0') break;
		scanner = data_row;
		data_count = 0;
		while (*scanner != '\0')
		{
			scanner = data_scanner(numchar, scanner);
			if (*scanner == ',') scanner += 1;
			real_num = chartoint(numchar);
			int_tmp[data_count] = real_num;
			data_count += 1;
			numchar[0] = '\0';
		}
		l_set_pointer = l_set_header;
		while (l_set_pointer != NULL)
		{
			int round = 0;
			for (int i = 0; i < data_count; i++)
			{
				if (int_tmp[i] == l_set_pointer->set_name[round]) round += 1;
				if (round > 1)
				{
					l_set_pointer->count += 1;
					break;
				}
			}
			l_set_pointer = l_set_pointer->next;
		}
	}
	/*
	l_set_pointer = l_set_header;
	while(l_set_pointer != NULL)
	{
	cout << l_set_pointer -> set_name[0] << "|" << l_set_pointer -> set_name[1] << "|" << l_set_pointer -> count << endl;
	l_set_pointer = l_set_pointer -> next;
	}
	*/
	c_set_node *l_set_last;
	l_set_pointer = l_set_header;
	while (l_set_pointer != NULL)
	{
		//cout << l_set_pointer -> count << endl;
		if (l_set_pointer->count < min_support)
		{
			if (l_set_pointer == l_set_header)
			{
				l_set_header = l_set_pointer->next;
				delete l_set_pointer;
				l_set_pointer = l_set_header;
			}
			else
			{
				l_set_last->next = l_set_pointer->next;
				delete l_set_pointer;
				l_set_pointer = l_set_last->next;
			}
		}
		else
		{
			l_set_last = l_set_pointer;
			large_set_counter += 1;
			l_set_pointer = l_set_pointer->next;
		}
	}
	layer = 2;
	//cout << endl << "l_set_header " << l_set_header << endl;
	l_set_pointer = l_set_header;
	/*while(l_set_pointer != NULL)
	{
	large_set_counter += 1;
	cout << l_set_pointer -> set_name[0] << "|" << l_set_pointer -> set_name[1] << "|" << l_set_pointer -> count << endl;
	l_set_pointer = l_set_pointer -> next;
	}*/
	return l_set_header;
}


//this function will use layer to find next level set
//this function construsts a new c_set_list
//combine_array are a dynamic integer point array
c_set_node *find_c(c_set_node *l_set_header)
{
	/* construst posible c_set_list */
	c_set_node *new_c_set_list_header = NULL;
	c_set_node *new_c_set_list_rear = NULL;
	c_set_node *new_c_set_builder = NULL;
	c_set_node *l_set_pointer, *l_set_finder;
	new_c_set_builder = new c_set_node;
	new_c_set_list_header = new_c_set_builder;
	new_c_set_list_rear = new_c_set_builder;
	new_c_set_builder->next = NULL;
	l_set_pointer = l_set_header;
	l_set_finder = l_set_header;
	bool check_point = 1;
	int ppd;
	while (l_set_pointer != NULL)
	{
		l_set_finder = l_set_pointer->next;
		while (l_set_finder != NULL)
		{
			for (ppd = 0; ppd < layer - 1; ppd++)
			{
				if (l_set_pointer->set_name[ppd] != l_set_finder->set_name[ppd]) break;
			}
			if (ppd == layer - 1)
			{
				new_c_set_builder = new c_set_node;
				new_c_set_builder->count = 0;
				new_c_set_list_rear->next = new_c_set_builder;
				new_c_set_list_rear = new_c_set_builder;
				new_c_set_builder->next = NULL;
				new_c_set_builder->set_name = new int[layer + 1];
				for (int q = 0; q < layer; q++) new_c_set_builder->set_name[q] = l_set_pointer->set_name[q];
				new_c_set_builder->set_name[layer] = l_set_finder->set_name[layer - 1];
			}
			else break;
			l_set_finder = l_set_finder->next;
		}
		l_set_pointer = l_set_pointer->next;
	}

	new_c_set_builder = new_c_set_list_header;
	new_c_set_list_header = new_c_set_list_header->next;
	delete new_c_set_builder;
	/*
	c_set_node *debug = new_c_set_list_header;
	layer++;
	cout << "---------------------------------------" << endl;
	cout << "---------------------------------------" << endl;
	cout << "---------------------------------------" << endl;
	cout << "---------------------------------------" << endl;
	cout << "---------------------------------------" << endl;
	print(debug);
	layer--;
	*/
	//cout << "now leave while(combine_array[0] < posibility_counter - layer -1)" << endl;
	/* now we remove some illegal node */
	l_set_pointer = l_set_header;
	c_set_node *c_set_police = new_c_set_list_header;
	c_set_node *c_set_last = NULL;
	//cout << "now enter while( c_set_police != NULL )" << endl;

	while (c_set_police != NULL)
	{
		//judgement
		bool result = 0;
		//bool *jury = new bool[layer+1];	 //all jury are 1 can pass the judgement 
		//bool check_switch = 0;
		int jumper = layer;
		bool for_flag = 1;
		l_set_pointer = l_set_header;
		while (l_set_pointer != NULL)
		{
			for_flag = 1;
			for (int i = 0; i < layer; i++)
			{
				if (i >= jumper)
				{
					if (l_set_pointer->set_name[i] == c_set_police->set_name[i + 1]) for_flag = 1;
					else
					{
						for_flag = 0;
						break;
					}
				}
				else
				{
					if (l_set_pointer->set_name[i] == c_set_police->set_name[i]) for_flag = 1;
					else
					{
						for_flag = 0;
						break;
					}
				}
			}
			if (for_flag == 1) jumper -= 1;
			if (jumper < 0)
			{
				result = 1;
				break;
			}
			l_set_pointer = l_set_pointer->next;
		}
		//linked_list process
		if (result == 1)
		{
			c_set_last = c_set_police;
			c_set_police = c_set_police->next;
		}
		else
		{
			if (c_set_police == new_c_set_list_header)
			{
				new_c_set_list_header = c_set_police->next;
				delete c_set_police;
				c_set_police = new_c_set_list_header;
			}
			else
			{
				c_set_last->next = c_set_police->next;
				delete c_set_police;
				c_set_police = c_set_last->next;
			}
		}
	}

	//cout << "now leave while( c_set_police != NULL )" << endl;
	/*
	c_set_police = new_c_set_list_header;
	cout << "now print the list" << endl;
	while(c_set_police != NULL)
	{
	for(int dota = 0 ; dota < layer+1 ; dota++) cout << c_set_police -> set_name[dota] << "|";
	cout << endl;
	c_set_police = c_set_police -> next;
	}
	*/
	layer += 1;
	if (new_c_set_list_header == NULL) terminate_flag = 1;
	return new_c_set_list_header;
}


c_set_node *find_l(c_set_node *c_set_header, int min_support)
{
	//cout << "in find_l" << endl;
	fstream infile;
	infile.open(file_name, ios::in);
	char data_row[512];
	c_set_node *c_set_ptr = c_set_header;
	char *scanner;
	char numchar[8];
	int count = 0;
	numchar[0] = '\0';
	while (!infile.eof())	/*this loop will scan data base*/
	{
		count = 0;
		int real_num, int_tmp[100] = { 0 };
		infile.getline(data_row, 512, '\n');
		if (data_row[0] == '\0') break;
		scanner = data_row;
		while (*scanner != '\0')		/*this while convert data_row to a integer array names int_tmp*/
		{
			scanner = data_scanner(numchar, scanner);
			if (*scanner == ',') scanner += 1;
			real_num = chartoint(numchar);
			int_tmp[count] = real_num;
			count += 1;
			numchar[0] = '\0';
		}
		/*
		cout << endl;
		for(int jaja = 0 ; jaja < count ; jaja++) cout << int_tmp[jaja] <<"|";
		cout << endl;
		*/
		c_set_ptr = c_set_header;
		while (c_set_ptr != NULL)	/*this while scan c_set_list to add some node's count if hit*/
		{
			int round = 0;
			for (int i = 0; i < count; i++)
			{
				if (int_tmp[i] == c_set_ptr->set_name[round]) round += 1;
				if (round > layer - 1)
				{
					c_set_ptr->count += 1;
					break;
				}
			}
			c_set_ptr = c_set_ptr->next;
		}
		data_row[0] = '\0';
	}
	/*
	c_set_ptr = c_set_header;
	while(c_set_ptr != NULL)
	{
	cout << c_set_ptr -> count << endl;
	c_set_ptr = c_set_ptr -> next;
	}
	*/
	c_set_ptr = c_set_header;
	c_set_node *l_set_header = c_set_header;
	c_set_node *c_set_last;
	while (c_set_ptr != NULL)
	{
		if (c_set_ptr->count < min_support)
		{
			if (c_set_ptr == l_set_header)
			{
				l_set_header = c_set_ptr->next;
				delete c_set_ptr;
				c_set_ptr = l_set_header;
			}
			else
			{
				c_set_last->next = c_set_ptr->next;
				delete c_set_ptr;
				c_set_ptr = c_set_last->next;
			}
		}
		else
		{
			//cout << " i got it !! " << endl;
			large_set_counter += 1;
			c_set_last = c_set_ptr;
			c_set_ptr = c_set_ptr->next;
		}
	}
	infile.close();
	infile.clear();
	if (l_set_header == NULL) terminate_flag = 1;
	return l_set_header;
}


int chartoint(char *input)
{
	char *ptr = input;
	int real_number = 0;
	int i = 1;
	for (int j = 1; j < strlen(input); j++) i *= 10;
	//cout << "i = " << i << endl;
	for (ptr; *ptr != '\0'; ptr++)
	{
		//cout << *ptr << endl;
		real_number += (int)((*ptr) - 48) * i;
		//cout << "real_number = " << real_number << endl ;
		i = i / 10;
	}
	return real_number;
}

/* scan data to tmp array , when find ',' or '\0' will stop, and return scanner's pointer */
//									already tested (?)
char *data_scanner(char *tmp, char *source)
{
	char *ptr = source;
	while (1)
	{
		if (*ptr == ',' || *ptr == '\0') break;
		else
		{
			if (*ptr != ' ') strncat(tmp, ptr, 1);
			ptr += 1;
		}
	}
	return ptr;
}

void print(c_set_node *l_set_header)
{
	c_set_node *l_set_ptr;
	l_set_ptr = l_set_header;
	while (l_set_ptr != NULL)
	{
		for (int i = 0; i < layer; i++) cout << l_set_ptr->set_name[i] << "|";
		cout << " count : " << l_set_ptr->count << endl;
		l_set_ptr = l_set_ptr->next;
	}
}


