#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "accept.h"
#include <string>
#include "sha256.h"

enum {
	Peer=0,
	Sha256=1,
	MD5=2
};//for readability

void PeerID(string * pack,TCPStream * stream)
{
	string *peer=new string;
	*peer=stream->getPeerIP();
	int msgL=peer->length();
	string hel=to_string(msgL);
	peer->resize(6);//random number to make peer id shorter

	if(msgL<10)
	{
		*pack+=hel;//set message length and put in packet
		*pack+="%%";	 //message length is blocked for indices 1-3
	}
	else if(msgL<100)
	{
		*pack+=hel;
		(pack[3])='%';
	}
	else *pack+=hel;
	
	pack->append(*peer);
}

int whichRecHash(string & pack)
{
	switch(pack[1])
	{
		case 'S':
			return Sha256;
		case 'M'://stubbed out for now
			return MD5;
		default:
			printf("Program terminated: Check Input");
			return -1;
	}
}

int determineHead(string & line,TCPStream * stream)
{
	string * pass=new string(line);//to ease up on stack memory
	switch(line[0])
	{
		case 'P':
			pass->resize(1);//why don't care about "thisishead" portion so remove
			printf("Peer request received\n");
			PeerID(pass,stream);
			line=*pass;
			return Peer;//PeerId==0
		case 'H':
			line.resize(2);
			printf("Hash request received\n");
			return whichRecHash(line);
		default:
			printf("input error");
			return -1;
	}
}

int main(int input, char** con)
{
    if (input < 2 || input > 4) 
	{
        printf("usage: server <port> [<ip>]\n");
        return 1;
    }

    TCPStream* stream = NULL;
    TCPAcceptor* acceptor = NULL;
    if (input == 3) 
        acceptor = new TCPAcceptor(atoi(con[1]), con[2]);//create an acceptor with port and address
    else 
        acceptor = new TCPAcceptor(atoi(con[1]));//create an acceptor if no client yet
	
	if (acceptor->start() == 0) //connection is setup
	{
        while (1) //for passive connection
		{
            stream = acceptor->accept();//stream is the new connection
            if (stream != NULL) 
			{
				int det;
                ssize_t len;
				string line;
				string hashed;
				string * shahash=new string(hashed);
				string * save=&line;

                while ((len = stream->receive(save, sizeof(line))) > 0) //while receiving continue
				{
					if(line.find("thisishead")!=-1)//find the matching string to mark head
					det=determineHead(line,stream);

					if(det==Sha256){
						cout<<"currently hashing..."<<endl;
						cout<<line<<endl;
						string d= sha256(line);
						cout<<d<<endl;
						shahash->append(line);
						stream->send(shahash,sizeof(hashed));
					}
					else
					{
						cout<<line<<endl;
						stream->send(save, sizeof(line));
					}
                }
				cout<<"request completed"<<endl;
                delete stream;
            }
        }
    }
    return 0;
}
