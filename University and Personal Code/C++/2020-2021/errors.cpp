            if(maze.at(i) != '.' && maze.at(i) != '1'){
                throw std::runtime_error("could not read maze layout");
            }

                        if(temp != 'l' || temp != 'r' || temp != 'u' || temp != 'd'){
                throw std::runtime_error("invalid move");
            }




        std::cout << height << " " << width << std::endl;
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                std::cout << maze.at((i * width) + j) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << exit.at(0) << " " << exit.at(1) << std::endl;
        std::cout << start.at(0) << " " << start.at(1) << std::endl;
        for(int i = 0; i < moves.size(); i++){
            std::cout << moves.at(i);
        }
        std::cout << std::endl;