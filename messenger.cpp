#include "client.h"
#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <thread>
 using namespace std;

struct Chat
{
    string  name, num;
    vector<string> messages;
};
class User {
public:
    string username;
    void display(){
        cout << username << ": ";
    }
};
    User user;
    Chat chat;
int nch = 1;
void menu(map<string, Chat>& chats);
void add_chat(map<string, Chat>& chats, Client& client);
void enter_chat(map<string, Chat>& chats, Client& client);
void receiveMessages(Client& client);
int main(){

    Client client;

    if (!client.connectToServer("127.0.0.1", 8080))
    {
        cout << "Couldn't connect to server!\n";
        return 1;
    }
    map<string, Chat> chats;
    client.connectToServer("127.0.0.1", 8080);

    thread reseiver(receiveMessages, ref(client));
    reseiver.detach();
    string name, num;
    bool running = true;
    cout << "Enter Username: ";
    cin >> user.username;
    client.sendMessage("LOGIN " + user.username);
    while (running){
        int ch;
        cout << "\n______________________________\n";
        cout << "Type 1 to see chats:\n";
        cout << "Type 2 to add chats:\n";
        cout << "Type 3 to find chat:\n";
        cout << "Type 0 to exit: ";
        cin >> ch;
        cout << "______________________________\n";
        switch(ch){
            case 0: cout << "Exiting"; running = false; break;
            case 1:{
                menu(chats); break;}
            case 2:{
                add_chat(chats, client);
            break;}
            case 3:{
                enter_chat(chats, client);
            }
        }

 }     
 return 0;  
    }
void receiveMessages(Client& client)
{
    while (true)
    {
        string msg = client.receiveMessage();

        if (msg.empty())
            break;

        cout << "\n" << msg << std::endl;
    }
}
 void menu(map<string, Chat>& chats){
    cout << endl;
    for(const auto&[num, chat] : chats){
        cout << "--------------------------------------------------\n";
        cout << num << " : "<< chat.name << endl; 
        cout << "--------------------------------------------------\n";

    }
 }
 void add_chat(map<string, Chat>& chats, Client& client){
    string num, n;


    cout << "Name :";
    cin >> n;
    num = to_string(nch);
    chat.num = num;
    chat.name = n;
    client.sendMessage("Find " + chat.name);
    string reply = client.receiveMessage();
    if(reply.rfind("OK", 0) == 0)
{
    chats[num] = chat;
    nch++; 
}
else if(reply.rfind("NO", 0 ) == 0)
{
    cout << "User not found\n";
}
     
 }
 void enter_chat(map<string, Chat>& chats, Client& client){
    bool running = true;
    string en;
    cout << "Enter the num of chat: ";
    cin >> en;
    auto it = chats.find(en);
    if (it != chats.end()){
        cout << endl << "Chat: " << it->first << " : " << it->second.name  << endl << endl;
    for (const string& message : it->second.messages)
{
        user.display();
        cout << message << '\n';
}
    cout << "\nType /exit to exit chat: \n";
    while(running){
        string mesage;
        user.display();
        getline(cin >> ws, mesage);
        if(mesage == "/exit"){
            running = false;
        }
        else{
           client.sendMessage("U " + chat.name + " " + mesage);
    }
}
    }
    else cout << "\nChat not Found!\n";

 }
