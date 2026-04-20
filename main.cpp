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
#include <filesystem>
#include <iomanip>

using json=nlohmann::json;

int main(int argc, char* argv[])
{
  bool loop=true;
  system("clear");

  std::filesystem::path exec_absolute=std::filesystem::absolute(argv[0]);
  std::filesystem::path build_dir=exec_absolute.parent_path();
  std::filesystem::path project_dir=build_dir.parent_path();

  const std::string filename="../stodos.json";
  if(!std::filesystem::exists(filename))
  {
    std::cerr<<"Warning: "<<filename<<" does not exist\n\n";
  }
  
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
  std::printf("=== STODO_List ===\n");
  std::printf("==================\n");

  while(loop)
  {
    std::string options_answer; 
    std::string todo_title, due_date, todo_desc;

    std::printf("What would you like to do?\n");
    std::printf("1) Add a new STODO\n");
    std::printf("2) List all your STODOs\n");
    std::printf("3) Erase STODOs\n");
    std::printf("Q/q) Quit\n\n");
    getline(std::cin, options_answer);

    if(options_answer=="1")
    {
      std::printf("How do you want to name this STODO?\n");
      getline(std::cin, todo_title);
      std::printf("\n");
      
      std::printf("Add a description to this STODO\n");
      getline(std::cin, todo_desc);
      std::printf("\n");

      json new_stodo={
        {"stodo_id", 0},
        {"todo_title", todo_title},
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
      std::cout<<std::setw(4)<<stodo_data<<"\n\n";
    }
    else if(options_answer=="3")
    {
      std::printf("Enter the ID of the STODO you want to delete\n");
      int ID_response;
      std::string id_input;
      getline(std::cin, id_input);

      try
      {
        ID_response=std::stoi(id_input);
      } catch(const std::exception&)
      {
        std::cerr<<"Error: please enter a valid number\n\n";
        continue;
      }

      auto& stodos=stodo_data["stodos"];
      bool found=false;

      for(auto it=stodos.begin(); it!=stodos.end(); ++it)
      {
        if(it->contains("stodo_id")&&it->value("stodo_id", 0)==ID_response)
        {
          stodos.erase(it);
          found=true;
          break;
        }
      }

      if(found)
      {
        std::ofstream output(filename);
        if(output.is_open())
        {
          output<<stodo_data<<std::endl;
          std::cout<<"Object with ID "<<ID_response<<"deleted successfully\n\n";
        } else{
          std::cerr<<"Error: Could not save the file.\n\n";
        }
      } else{
        std::cout<<"No object with ID "<<ID_response<<" found.\n\n";
      }
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
