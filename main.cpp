    #include <iostream>
    #include <vector>
    using namespace std;
    // Cell class refers to the cell itself the type and display function for it
    class Cell {
    public:
        enum Type { EMPTY, HOLE, RED_MAGNET, PURPLE_MAGNET, ROCK };

        Cell(Type type = EMPTY){
            cell_type = type;
        }
        Type getType() const {return cell_type;}

        void setType(Type newType) {cell_type=newType; }

        char display() const {
            switch (cell_type) {
                case EMPTY: return '.';
                case HOLE: return 'O';
                case RED_MAGNET: return 'R';
                case PURPLE_MAGNET: return 'P';
                case ROCK: return 'X';
                default: return '?';
            }
        }
    private:
        Type cell_type;
    };
    ///////////// ////
    // Grid class -> has a grid of n X n each item is cell object
    // function -> placeMagnet for inital state , print grid
    class Grid {
    public:
        Grid (int size){
            this->size = size;
            cells.resize(size, vector<Cell>(size));
        }

        void placeMagnet(int x, int y, Cell::Type cell_type) {
            if (x >= 0 && x < size && y >= 0 && y < size) {
                cells[x][y].setType(cell_type);
            } else {
                cerr << "Invalid coordinates!" << endl;
            }
        }


        void print() {
            for(int i = 0;i <(size * 4);i++){
                cout<<"_";
            }
            cout<<endl;
            cout<<"     |";
            for(int i = 0;i < size;i++){
                cout<<" "<<i<<" ";
            }
            cout<<endl;
            for(int i = 0;i < size;i++){
                for(int k = 0 ;k < (size * 4) ;k++){
                    cout<<"_";
                }
                cout<<endl;
                cout<<i<<" -> ";

                for(int j = 0;j < size;j++){
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
        // valid the destination
        bool validate(int x , int y){
              return (x >= 0 && x < grid.getCells().size() && y >= 0 && y < grid.getCells().size());
        }


        bool notAvailableCell(int x , int y){
            Cell currentCell = grid.getCells()[x][y];
            return (currentCell.getType() != Cell::EMPTY) && (currentCell.getType() != Cell::HOLE);
        }

        bool up(int x , int y){
            int z = 0;
            int xx = max(z , x - 1);
            Cell currentCell = grid.getCells()[xx][y];
            return (currentCell.getType() == Cell::EMPTY);
        }

        bool down(int x , int y){
            int z = grid.getCells().size() - 1;
            int xx = min(z , x + 1);
            Cell currentCell = grid.getCells()[xx][y];
            return (currentCell.getType() == Cell::EMPTY);
        }

        bool left(int x , int y){
            int z = 0;
            int yy = max(z , y - 1);
            Cell currentCell = grid.getCells()[x][yy];
            return (currentCell.getType() == Cell::EMPTY);
        }

        bool right(int x , int y){
            int z = grid.getCells().size() - 1;
            int yy = min(z , y + 1);
            Cell currentCell = grid.getCells()[x][yy];
            return (currentCell.getType() == Cell::EMPTY);
        }


        bool isStone(int x , int y){
            return (grid.getCells()[x][y].getType() != Cell::EMPTY && grid.getCells()[x][y].getType() != Cell::HOLE );
        }


        void moveMagnet(int x_cur, int y_cur, int x_dest, int y_dest) {
           // valid
           if(!validate(x_cur , y_cur) || !validate(x_dest , y_dest)){
                cerr << "Invalid destination!" << endl;
                return;
            }

            if(notAvailableCell(x_dest , y_dest)){
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

                RedshiftStones(x_dest , y_dest);
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
                if (isStone(x , j)) {
                    grid.getCells()[x][j - 1].setType(grid.getCells()[x][j].getType());
                    grid.getCells()[x][j].setType(Cell::EMPTY);
                }
            }

            i = grid.getCells().size() - 1;
            while(grid.getCells()[x][i].getType() != Cell::HOLE   && grid.getCells()[x][i].getType() != Cell::EMPTY && i != y)i--;
            // Shift right
            for (int j = i; j > y ; j--) {
                if (isStone(x , j)) {
                    grid.getCells()[x][j + 1].setType(grid.getCells()[x][j].getType());
                    grid.getCells()[x][j].setType(Cell::EMPTY);
                }
            }
            // shift up
            i = 0;
            while(grid.getCells()[i][y].getType() != Cell::HOLE &&  grid.getCells()[i][y].getType() != Cell::EMPTY && i != x)i++;
            for (int j = i ; j < x ; j++) {
                if (isStone(j , y)) {
                    grid.getCells()[j - 1][y].setType(grid.getCells()[j][y].getType());
                    grid.getCells()[j][y].setType(Cell::EMPTY);
                }
            }
             // shift down
            i = grid.getCells().size() - 1;
            while(grid.getCells()[i][y].getType() != Cell::HOLE   && grid.getCells()[i][y].getType() != Cell::EMPTY && i != x)i--;
            for (int j = i; j > x ; j--) {
                if (isStone(j , y)) {
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
                if (isStone(x , j)) {
                    grid.getCells()[x][j - 1].setType(grid.getCells()[x][j].getType());
                    grid.getCells()[x][j].setType(Cell::EMPTY);
                }
            }
            i = y - 1;
            while(i >= 0 &&  grid.getCells()[x][i].getType() != Cell::HOLE   && grid.getCells()[x][i].getType() != Cell::EMPTY )i--;
            for (int j = i ; j >=  0 ; j--) {
                if (isStone(x , j)) {
                    grid.getCells()[x][j + 1].setType(grid.getCells()[x][j].getType());
                    grid.getCells()[x][j].setType(Cell::EMPTY);
                }
            }
            i = x - 1;
            while( i >= 0 && grid.getCells()[i][y].getType() != Cell::HOLE   && grid.getCells()[i][y].getType() != Cell::EMPTY )i--;
            for (int j = i ; j >=  0 ; j--) {
                if (isStone(j , y)) {
                    grid.getCells()[j + 1][y].setType(grid.getCells()[j][y].getType());
                    grid.getCells()[j][y].setType(Cell::EMPTY);
                }
            }
            i = x + 1;
            while(i < grid.getCells().size() &&  grid.getCells()[i][y].getType() != Cell::HOLE   && grid.getCells()[i][y].getType() != Cell::EMPTY )i++;
            for (int j = i ; j <  grid.getCells().size() ; j++) {
                if (isStone(j , y)) {
                    grid.getCells()[j - 1][y].setType(grid.getCells()[j][y].getType());
                    grid.getCells()[j][y].setType(Cell::EMPTY);
                }
            }

            }




    };


    int main() {
        cout<<"Enter the size of Grid : ";
        int sz = 0;
        cin>>sz;
        Game game(sz);
        cout<<"choose the level :1 , 2 ";
        int l = 1;
        cin>>l;
            vector<pair<int,int>>holes;

        game.getGrid().placeMagnet(0, 0, Cell::RED_MAGNET);
        game.getGrid().placeMagnet(0, 1, Cell::ROCK);
        game.getGrid().placeMagnet(1, 1, Cell::HOLE);
        game.getGrid().placeMagnet(1, 2, Cell::HOLE);
        game.getGrid().placeMagnet(3, 2, Cell::HOLE);
        game.getGrid().placeMagnet(2, 1, Cell::HOLE);
        game.getGrid().placeMagnet(2, 2, Cell::ROCK);
        game.getGrid().placeMagnet(3, 3, Cell::PURPLE_MAGNET);
        holes.push_back({1 , 1});
        holes.push_back({1 , 2});
        holes.push_back({3 , 2});
        holes.push_back({2 , 1});


        vector<Grid>states;
        bool t = true;
        while (t) {

            for(int i = 0;i < holes.size();i++){
                    int x = holes[i].first;
                    int y = holes[i].second;
                if(game.getGrid().getCells()[x][y].getType() == Cell::EMPTY){
                  game.getGrid().getCells()[holes[i].first][holes[i].second].setType(Cell::HOLE);
                }
            }
            game.getGrid().print();

            int xx= 0;
            for(int i = 0;i < game.getGrid().getCells().size();i++){
                for(int j = 0 ;j < game.getGrid().getCells().size();j++){
                    if(game.getGrid().getCells()[i][j].getType() == Cell::HOLE){
                        xx++;
                    }

                }
            }

            states.push_back(game.getGrid());
            if(xx == 0){
                cout<<"$$$$$$$$$      YOU WIN    $$$$$$$$$";
        cout<<"Quit => q \n";
            char ch ;
            cin>>ch;
            if(ch == 'q'){
                t = false;
                cout<<"You wants to show the states you played by 1";
                int ii = 0;
                cin>>ii;
                if(ii == 1){
                   for(int i = 0;i < states.size() ;i++){
                        states[i].print();
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
            if(ch == 'q'){
                t = false;
                cout<<"You wants to show the states you played by 1 :";
                int ii = 0;
                cin>>ii;
                if(ii == 1){
                   for(int i = 0;i < states.size() ;i++){
                        states[i].print();
                        cout<<endl;
                   }
                }
            }
        }
        return 0;
    }
