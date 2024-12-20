    #include <iostream>
    #include <vector>
    #include<set>
    #include <string>
    #include <sstream>
    #include<queue>
    #include<stack>

    using namespace std;

    vector<pair<int,int>>holes;

    // Cell class refers to the cell itself the type and display function for it
    class Cell {
    public:
        enum Type { EMPTY, HOLE, RED_MAGNET, PURPLE_MAGNET, ROCK };

        Cell(Type type = EMPTY) {
            cell_type = type;
        }
        Type getType() const {
            return cell_type;
        }

        void setType(Type newType) {
            cell_type=newType;
        }

        char display() const {
            switch (cell_type) {
            case EMPTY:
                return '.';
            case HOLE:
                return 'O';
            case RED_MAGNET:
                return 'R';
            case PURPLE_MAGNET:
                return 'P';
            case ROCK:
                return 'X';
            default:
                return '?';
            }
        }
    private:
        Type cell_type;
    };
    ///////////// ////
    // Grid class -> has a grid of n X n each item is cell object
    // function -> placeCell for inital state , print grid

    class Grid {
    public:
        Grid (int size) {
            this->size = size;
            cells.resize(size, vector<Cell>(size));
        }

        string getStateString() const {
            ostringstream oss;
            for (const auto& row : cells) {
                for (const auto& cell : row) {
                    oss << cell.getType() << ",";
                }
                oss << ";";
            }
            return oss.str();
        }

        void placeCell(int x, int y, Cell::Type cell_type) {
            if (x >= 0 && x < size && y >= 0 && y < size) {
                cells[x][y].setType(cell_type);
            } else {
                cerr << "Invalid coordinates!" << endl;
            }
        }


        void print() {
            for(int i = 0; i <(size * 4); i++) {
                cout<<"_";
            }
            cout<<endl;
            cout<<"     |";
            for(int i = 0; i < size; i++) {
                cout<<" "<<i<<" ";
            }
            cout<<endl;
            for(int i = 0; i < size; i++) {
                for(int k = 0 ; k < (size * 4) ; k++) {
                    cout<<"_";
                }
                cout<<endl;
                cout<<i<<" -> ";

                for(int j = 0; j < size; j++) {
                    cout<<"|";
                    cout<<cells[i][j].display()<<" ";
                    cout<<"";
                }
                cout<<endl;
            }

        }

        vector<vector<Cell>>& getCells() {
            return cells;
        }


    private:
        int size;
        vector<vector<Cell>> cells;
    };
    //////////////////////////////////////////
    //// Game class has grid and the movement functions
    // vaildate , notAvailableCell , up , down , left right ,moveMagnet

    class Game {
    public:
        Game(int gridSize) : grid(gridSize) {}

        Grid& getGrid() {
            return grid;
        }
        void setGrid(Grid g) {
            grid = g;
        }
        // valid the destination
        bool validate(int x, int y) {
            return (x >= 0 && x < grid.getCells().size() && y >= 0 && y < grid.getCells().size());
        }


        bool notAvailableCell(int x, int y) {
            Cell currentCell = grid.getCells()[x][y];
            return (currentCell.getType() != Cell::EMPTY) && (currentCell.getType() != Cell::HOLE);
        }

        bool up(int x, int y) {
            int z = 0;
            int xx = max(z, x - 1);
            Cell currentCell = grid.getCells()[xx][y];
            return (currentCell.getType() == Cell::EMPTY);
        }

        bool down(int x, int y) {
            int z = grid.getCells().size() - 1;
            int xx = min(z, x + 1);
            Cell currentCell = grid.getCells()[xx][y];
            return (currentCell.getType() == Cell::EMPTY);
        }

        bool left(int x, int y) {
            int z = 0;
            int yy = max(z, y - 1);
            Cell currentCell = grid.getCells()[x][yy];
            return (currentCell.getType() == Cell::EMPTY);
        }

        bool right(int x, int y) {
            int z = grid.getCells().size() - 1;
            int yy = min(z, y + 1);
            Cell currentCell = grid.getCells()[x][yy];
            return (currentCell.getType() == Cell::EMPTY);
        }


        bool isStone(int x, int y) {
            return (grid.getCells()[x][y].getType() != Cell::EMPTY && grid.getCells()[x][y].getType() != Cell::HOLE );
        }
        void moveMagnet(int x_cur, int y_cur, int x_dest, int y_dest) {
            // valid
            if(!validate(x_cur, y_cur) || !validate(x_dest, y_dest)) {
                cerr << "Invalid destination!" << endl;
                return;
            }

            if(notAvailableCell(x_dest, y_dest)) {
                cerr << "Not Available Cell !" << endl;
                return ;
            }
            Cell& currentCell = grid.getCells()[x_cur][y_cur];
            Cell::Type movingType = currentCell.getType();
            currentCell.setType(Cell::EMPTY);
            if (movingType == Cell::PURPLE_MAGNET) {
                if (grid.getCells()[x_dest][y_dest].getType() == Cell::EMPTY ||
                        grid.getCells()[x_dest][y_dest].getType() == Cell::HOLE) {
                    shiftStones(x_dest, y_dest);
                } else {
                    cerr << "Invalid destination for Purple Magnet!" << endl;
                    return;
                }
            }
            if (movingType == Cell::RED_MAGNET) {
                if (grid.getCells()[x_dest][y_dest].getType() == Cell::EMPTY ||
                        grid.getCells()[x_dest][y_dest].getType() == Cell::HOLE) {

                    RedshiftStones(x_dest, y_dest);
                } else {
                    cerr << "Invalid destination for RED Magnet!" << endl;
                    return;
                }
            }
            grid.getCells()[x_dest][y_dest].setType(movingType);
        }
    private:
        Grid grid;
        void shiftStones(int x, int y) {
            // Shift left
            int i = 0;
            while(grid.getCells()[x][i].getType() != Cell::HOLE   && grid.getCells()[x][i].getType() != Cell::EMPTY   && i != y)i++;
            for (int j = i ; j < y ; j++) {
                if (isStone(x, j)) {
                    grid.getCells()[x][j - 1].setType(grid.getCells()[x][j].getType());
                    grid.getCells()[x][j].setType(Cell::EMPTY);
                }
            }

            i = grid.getCells().size() - 1;
            while(grid.getCells()[x][i].getType() != Cell::HOLE   && grid.getCells()[x][i].getType() != Cell::EMPTY && i != y)i--;
            // Shift right
            for (int j = i; j > y ; j--) {
                if (isStone(x, j)) {
                    grid.getCells()[x][j + 1].setType(grid.getCells()[x][j].getType());
                    grid.getCells()[x][j].setType(Cell::EMPTY);
                }
            }
            // shift up
            i = 0;
            while(grid.getCells()[i][y].getType() != Cell::HOLE &&  grid.getCells()[i][y].getType() != Cell::EMPTY && i != x)i++;
            for (int j = i ; j < x ; j++) {
                if (isStone(j, y)) {
                    grid.getCells()[j - 1][y].setType(grid.getCells()[j][y].getType());
                    grid.getCells()[j][y].setType(Cell::EMPTY);
                }
            }
            // shift down
            i = grid.getCells().size() - 1;
            while(grid.getCells()[i][y].getType() != Cell::HOLE   && grid.getCells()[i][y].getType() != Cell::EMPTY && i != x)i--;
            for (int j = i; j > x ; j--) {
                if (isStone(j, y)) {
                    grid.getCells()[j + 1][y].setType(grid.getCells()[j][y].getType());
                    grid.getCells()[j][y].setType(Cell::EMPTY);
                }
            }

        }

        void RedshiftStones(int x, int y) {
            // Shift left
            int i = y + 1;
            while( i < grid.getCells().size()&& grid.getCells()[x][i].getType() != Cell::HOLE   && grid.getCells()[x][i].getType() != Cell::EMPTY )i++;
            for (int j = i ; j <  grid.getCells().size() ; j++) {
                if (isStone(x, j)) {
                    grid.getCells()[x][j - 1].setType(grid.getCells()[x][j].getType());
                    grid.getCells()[x][j].setType(Cell::EMPTY);
                }
            }
            i = y - 1;
            while(i >= 0 &&  grid.getCells()[x][i].getType() != Cell::HOLE   && grid.getCells()[x][i].getType() != Cell::EMPTY )i--;
            for (int j = i ; j >=  0 ; j--) {
                if (isStone(x, j)) {
                    grid.getCells()[x][j + 1].setType(grid.getCells()[x][j].getType());
                    grid.getCells()[x][j].setType(Cell::EMPTY);
                }
            }
            i = x - 1;
            while( i >= 0 && grid.getCells()[i][y].getType() != Cell::HOLE   && grid.getCells()[i][y].getType() != Cell::EMPTY )i--;
            for (int j = i ; j >=  0 ; j--) {
                if (isStone(j, y)) {
                    grid.getCells()[j + 1][y].setType(grid.getCells()[j][y].getType());
                    grid.getCells()[j][y].setType(Cell::EMPTY);
                }
            }
            i = x + 1;
            while(i < grid.getCells().size() &&  grid.getCells()[i][y].getType() != Cell::HOLE   && grid.getCells()[i][y].getType() != Cell::EMPTY )i++;
            for (int j = i ; j <  grid.getCells().size() ; j++) {
                if (isStone(j, y)) {
                    grid.getCells()[j - 1][y].setType(grid.getCells()[j][y].getType());
                    grid.getCells()[j][y].setType(Cell::EMPTY);
                }
            }

        }



    };
    struct State {
        Grid grid;
        string stateString;

        State(const Grid& g) : grid(g), stateString(g.getStateString()) {}
    };
    // BFS implementation
    void bfs(Game& game) {
        queue<State> stateQueue;
        set<string> visitedStates;
        stateQueue.push(State(game.getGrid()));
        visitedStates.insert(stateQueue.front().stateString);
        cout<<stateQueue.front().stateString<<endl;
        int layers = 0;
        while (!stateQueue.empty() ) {
            layers++;
            State currentState = stateQueue.front();
            stateQueue.pop();

            // check if it is winning state
            int xx= 0;
            for(int i = 0; i < currentState.grid.getCells().size(); i++) {
                for(int j = 0 ; j < currentState.grid.getCells().size(); j++) {
                    if(currentState.grid.getCells()[i][j].getType() == Cell::HOLE) {
                        xx++;
                    }

                }
            }
            // Check if current state is a winning state
            if (xx == 0) {
                cout << "Found a winning state! in layer "<<layers<< endl;
                currentState.grid.print();
                return;
                //  return;
            } else {
                currentState.grid.print();
            }
            // create states tree
            for (int i = 0; i < game.getGrid().getCells().size(); ++i) {
                for (int j = 0; j < game.getGrid().getCells().size(); ++j) {
                    if(currentState.grid.getCells()[i][j].getType() == Cell::RED_MAGNET ||
                            currentState.grid.getCells()[i][j].getType() == Cell::PURPLE_MAGNET
                      ) {
                        for (int k = 0; k < game.getGrid().getCells().size(); ++k) {
                            for (int h = 0; h < game.getGrid().getCells().size(); ++h) {
                                if(currentState.grid.getCells()[k][h].getType() == Cell::EMPTY ||
                                        currentState.grid.getCells()[k][h].getType() == Cell::HOLE
                                  ) {
                                    Game newGame = game;
                                    newGame.setGrid(currentState.grid);
                                    newGame.moveMagnet(i, j, k, h);
                                    for(int b = 0; b < holes.size(); b++) {
                                        int x = holes[b].first;
                                        int y = holes[b].second;
                                        if(newGame.getGrid().getCells()[x][y].getType() == Cell::EMPTY) {
                                            newGame.getGrid().getCells()[holes[b].first][holes[b].second].setType(Cell::HOLE);
                                        }
                                    }
                                    State newState(newGame.getGrid());

                                    if (!visitedStates.count(newState.stateString)) {
                                        visitedStates.insert(newState.stateString);
                                        stateQueue.push(newState);
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }

        cout << "No solution found." << endl;
    }


     void dfs(Game& game) {
        stack<State> stateStack;
        set<string> visitedStates;
        stateStack.push(State(game.getGrid()));
        visitedStates.insert(stateStack.top().stateString);
        while (!stateStack.empty() ) {
            State currentState = stateStack.top();
            stateStack.pop();

            // check if it is winning state
            int xx= 0;
            for(int i = 0; i < currentState.grid.getCells().size(); i++) {
                for(int j = 0 ; j < currentState.grid.getCells().size(); j++) {
                    if(currentState.grid.getCells()[i][j].getType() == Cell::HOLE) {
                        xx++;
                    }

                }
            }
            // Check if current state is a winning state
            if (xx == 0) {
                cout << "Found a winning state!" << endl;
                currentState.grid.print();
                return;
                //  return;
            } else {
                currentState.grid.print();
            }
            // create states tree
            for (int i = 0; i < game.getGrid().getCells().size(); ++i) {
                for (int j = 0; j < game.getGrid().getCells().size(); ++j) {
                    if(currentState.grid.getCells()[i][j].getType() == Cell::RED_MAGNET ||
                            currentState.grid.getCells()[i][j].getType() == Cell::PURPLE_MAGNET
                      ) {
                        for (int k = 0; k < game.getGrid().getCells().size(); ++k) {
                            for (int h = 0; h < game.getGrid().getCells().size(); ++h) {
                                if(currentState.grid.getCells()[k][h].getType() == Cell::EMPTY ||
                                        currentState.grid.getCells()[k][h].getType() == Cell::HOLE
                                  ) {
                                    Game newGame = game;
                                    newGame.setGrid(currentState.grid);
                                    newGame.moveMagnet(i, j, k, h);
                                    for(int b = 0; b < holes.size(); b++) {
                                        int x = holes[b].first;
                                        int y = holes[b].second;
                                        if(newGame.getGrid().getCells()[x][y].getType() == Cell::EMPTY) {
                                            newGame.getGrid().getCells()[holes[b].first][holes[b].second].setType(Cell::HOLE);
                                        }
                                    }
                                    State newState(newGame.getGrid());

                                    if (!visitedStates.count(newState.stateString)) {
                                        visitedStates.insert(newState.stateString);
                                        stateStack.push(newState);
                                    }
                                }

                            }
                        }
                    }
                }
            }
        }

        cout << "No solution found." << endl;
    }



    vector<Grid>states;

    void initState1(Game &game) {
        game.getGrid().placeCell(0, 0, Cell::RED_MAGNET);
        game.getGrid().placeCell(0, 1, Cell::ROCK);
        game.getGrid().placeCell(1, 1, Cell::HOLE);
        game.getGrid().placeCell(2, 1, Cell::HOLE);
        game.getGrid().placeCell(3, 2, Cell::HOLE);
        game.getGrid().placeCell(0, 2, Cell::HOLE);
        game.getGrid().placeCell(1, 2, Cell::ROCK);
        game.getGrid().placeCell(3, 3, Cell::PURPLE_MAGNET);
        holes.push_back({1, 1});
        holes.push_back({2, 1});
        holes.push_back({3, 2});
        holes.push_back({0, 2});
    }

    void initState2(Game &game) {
        game.getGrid().placeCell(0, 0, Cell::PURPLE_MAGNET);
        game.getGrid().placeCell(0, 1, Cell::HOLE);
        game.getGrid().placeCell(0, 3, Cell::ROCK);
        game.getGrid().placeCell(0, 4, Cell::ROCK);
        game.getGrid().placeCell(1, 0, Cell::HOLE);
        game.getGrid().placeCell(1, 4, Cell::HOLE);
        game.getGrid().placeCell(2, 1, Cell::HOLE);
        game.getGrid().placeCell(3, 0, Cell::ROCK);
        game.getGrid().placeCell(3, 2, Cell::RED_MAGNET);
        holes.push_back({0, 1});
        holes.push_back({0, 3});
        holes.push_back({1, 0});
        holes.push_back({1, 4});
        holes.push_back({2, 1});
    }

    void initState3(Game &game) {
        game.getGrid().placeCell(0, 0, Cell::HOLE);
        game.getGrid().placeCell(0 , 2, Cell::HOLE);
        game.getGrid().placeCell(1 , 1, Cell::ROCK);
        game.getGrid().placeCell(2 , 0, Cell::PURPLE_MAGNET);
        holes.push_back({0, 0});
        holes.push_back({0, 2});
    }


    void checkHolesStatus(Game &game) {
        for(int i = 0; i < holes.size(); i++) {
            int x = holes[i].first;
            int y = holes[i].second;
            if(game.getGrid().getCells()[x][y].getType() == Cell::EMPTY) {
                game.getGrid().getCells()[holes[i].first][holes[i].second].setType(Cell::HOLE);
            }
        }
    }
    int win(Game &game) {
        int xx= 0;
        for(int i = 0; i < game.getGrid().getCells().size(); i++) {
            for(int j = 0 ; j < game.getGrid().getCells().size(); j++) {
                if(game.getGrid().getCells()[i][j].getType() == Cell::HOLE) {
                    xx++;
                }

            }
        }
        return xx;
    }


    int main() {
        cout<<"Enter the size of Grid : ";
        int sz = 0;
        cin>>sz;
        Game game(sz);

        cout<<"choose the level :1 , 2 , 3 \n ";
        int l = 1;
        cin>>l;

        if(l == 1){initState1(game);
        game.getGrid().print();
        }
        else if(l == 2){initState2(game);
        game.getGrid().print();
        }
        else{initState3(game);
        game.getGrid().print();
        }
        cout<<endl;

        bool t = true;
        int ch = 1;
        cout<<"choose :\n 1 : if you would like to play \n 2 : if you would BFS algorithm to Play and find the solution \n 3 : DFS \n ";
        cin >>ch;
        if(ch == 1) {
            while (t) {
                // check if holes status correct
                checkHolesStatus(game);
                game.getGrid().print();
                states.push_back(game.getGrid());
                // check if win
                if(win(game) == 0) {
                    cout<<"$$$$$$$$$      YOU WIN    $$$$$$$$$\n";
                    cout<<"Quit => q \n";
                    char ch ;
                    cin>>ch;
                    if(ch == 'q') {
                        t = false;
                        cout<<"You wants to show the states you played by 1\n";
                        int ii = 0;
                        cin>>ii;
                        if(ii == 1) {
                            for(int i = 0; i < states.size() ; i++) {
                                // states[i].print();
                                cout<<endl;
                            }
                        }
                    }

                }

                int x_cur, y_cur, x_dest, y_dest;
                cout<<"Enter current coordinates and destination (x_cur y_cur x_dest y_dest): ";
                cin>>x_cur>>y_cur>>x_dest>>y_dest;
                game.moveMagnet(x_cur, y_cur, x_dest, y_dest);
                cout<<"Quit => q \n";
                char ch ;
                cin>>ch;
                if(ch == 'q') {
                    t = false;
                    cout<<"You wants to show the states you played by 1 :";
                    int ii = 0;
                    cin>>ii;
                    if(ii == 1) {
                        for(int i = 0; i < states.size() ; i++) {
                            states[i].print();
                            cout<<endl;
                        }
                    }
                }
            }
            return 0;
        } else if (ch == 2){
            bfs(game);
        }
        else {
            dfs(game);
        }

    }
