/*

   ======Smart To Do List======
   Ongoing project to improve some skills with C++

   Start date: 03/30/2026

   I'll probably be adding some more stuff to this project throughout my journey.

*/

#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json=nlohmann::json;
bool loop=true;

int main()
{
  const std::string filename="stodo.json";

  json stodo_data;

  std::ifstream input(filename);
  if(input.is_open())
  {
    try 
    {
      input>>stodo_data;
    }
    catch(...) 
    {
      stodo_data=json::object();
    }
    input.close();
  } else {
    std::cerr<<"Error: Could not open file '"<<filename<<"'\n";
    return 1;
  }

  if(!stodo_data.contains("stodos") || !stodo_data["stodos"].is_array())
  {
    stodo_data["stodos"]=json::array();
  }
  
  std::printf("==================\n");
  std::printf("==  STODO_List  ==\n");
  std::printf("==================\n");

  while(loop)
  {
    std::string options_answer; 
    std::string todo_title, due_date, todo_desc;

    std::printf("What would you like to do?\n");
    std::printf("1) Add a new STODO\n");
    std::printf("2) List all your STODOs\n");
    std::printf("3) Erase STODOs");
    std::printf("Q/q) Quit\n\n");
    getline(std::cin, options_answer);

    if(options_answer=="1")
    {
      std::printf("How do you want to name this STODO?\n");
      getline(std::cin, todo_title);
      std::printf("\n");
      
      std::printf("When is this STODO due?\n");
      getline(std::cin, due_date);
      std::printf("\n");

      std::printf("Add a description to this STODO\n");
      getline(std::cin, todo_desc);
      std::printf("\n");

      json new_stodo={
        {"stodo_id", 0},
        {"todo_title", todo_title},
        {"todo_due_date", due_date},
        {"todo_desc", todo_desc}
      };

      int nextId=1;
      for(const auto& stodo : stodo_data["stodos"])
      {
        if(stodo.contains("stodo_id") && stodo["stodo_id"].is_number_integer())
        {
          nextId=std::max(nextId, stodo["stodo_id"].get<int>() + 1);
        }
      }
      new_stodo["stodo_id"]=nextId;

      stodo_data["stodos"].push_back(new_stodo);

      std::ofstream output(filename);
      if(output.is_open())
      {
        output<<stodo_data<<"\n";
        std::cout<<"Your new STODO has been added successfully! ID = "<<nextId<<"\n";
      } else {
        std::cerr<<"Error: Could not add your STODO";
      }
    }
    else if(options_answer=="2")
    {
      std::printf("=== STODOs List ===\n\n");
      std::cout<<std::setw(4)<<stodo_data<<std::endl;
    }
    else if(options_answer=="3")
    {
      
    }
    else if(options_answer=="Q"||options_answer=="q")
    {
      std::printf("Exiting the program...");
      return loop=false;
    }
    else
    {
      std::cerr<<"Error: Invalid answer\n\n";
    }
  }
}
