#include"buffer.h"
#include<iostream>
#include<stdexcept>

using namespace std;

int main(int argc,char* argv[])
{
	buffer<int> buf(10);
	for(int i=0;i<50;i++)
	{
		buf.push(i);
	}

	for(int i=0;i<100;i++)
	{
		try
		{
			cout << buf[i] << endl;
		}
		catch (out_of_range& err)
		{
			cout << "Out of range: " << err.what() << endl;
		}
	}
	return 0;
}
