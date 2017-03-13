#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <ctype.h>
#include "stream.h"
#include "connect.h"

/*******
*header files look like this for peer
*['request peer id', 'thisishead']
*
*header files for hash
*['request hash','type of hash func','thisishead']
*
*///////

void HeadP(string & pack)//this makes head packets
{
	string head="thisishead";
	pack.append(head);
}

int shaHash(string & choose, char **con)//sends hash packets
{
    TCPConnector* connector = new TCPConnector();
	printf("Connecting to Peer\n");
    TCPStream* stream = connector->connect(con[2], atoi(con[1]));
	string * save=new string(choose);
	*save="HjasldPJ";
	string fin;

    if (stream) 
	{
		printf("Connected to Peer\n");

		stream->send(save, sizeof(choose));
		cout<<*save<<endl;

		stream->receive(save, sizeof(choose));
		cout<<*save<<endl;
		delete stream;
    }
	return 0;
}

int whichHash(string & choose,char **con)//to future proof if more hash's wanted
{
	string yes;
	cout<<"which hash function?"<<endl;
	cout<<"MB5 or SHA265"<<endl;
	cin>>yes;
	yes.resize(1);

	yes[0]=toupper(yes[0]);
	choose.append(yes);
	
	switch(choose[1])//check second char of string for what type of hash
	{
		case 'S':
			shaHash(choose,con);///NOT WORKING NEED TO LOOK INTO SHA FILE

			return 0;
		default:
			printf("Program terminated: Check Input\n");
			return -1;
	}
}

int PeerID(string & choose,char **con)//sends and recieves peer id 
{
    int len;

    TCPConnector* connector = new TCPConnector();
	printf("Connecting to Peer\n");
    TCPStream* stream = connector->connect(con[2], atoi(con[1]));
	string * save=new string(choose);

    if (stream) 
	{
		printf("Connected to Peer\n");
		stream->send(save, sizeof(choose));
		printf("Peer ID request sent\n");
		len=stream->receive(save, sizeof(choose));//assume msgL nvr >256
		printf("Peer ID is - %s\n", save->c_str());
		delete stream;
    }
    return 0;
}

int chooseMethod(string & choose,char **con)//chooses hash or peer
{
	switch(choose[0])
	{
		case 'P':
			HeadP(choose);
			PeerID(choose,con);
			return 0;
		case 'H':
			whichHash(choose,con);
			HeadP(choose);
			return 0;
		default:
			printf("Program terminated: Check Input");
			return -1;
	}
}

int main(int input, char** con)
{
    string line;//packet

    if (input != 3) 
	{
        printf("usage: %s <port> <ip>\n", con[0]);
		return -1;
    }

	cout<<"Pick one of two"<<endl;
	cout<<"Peer ID or Hash"<<endl;

	cin>>line;//account for human error and only take first char
				// if we have future commands that start w/ P or H
				 
	line[0]=toupper(line[0]);
	
	if(chooseMethod(line,con) < 0) return -1;//allow for future tasks 

    return 0;
}

