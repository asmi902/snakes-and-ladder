#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <string>
using namespace std;

class Square
{
    private:
        Square* next;
        Square* ladder;
        Square* snake;
        int pos;
    public:
        Square(int position, Square* nextSquare = nullptr, Square* ifLadder = nullptr, Square* ifSnake = nullptr)
        {
            pos = position;
            next = nextSquare;
            ladder = ifLadder;
            snake = ifSnake;
        } 

        Square* getNext()
        {
            return next;
        }

        int getPos()
        {
            return pos;
        }
        
        Square* getIfLadder()
        {
            return ladder;
        }

        Square* getIfSnake()
        {
            return snake;
        }
        
        void setIfLadder(Square* ladderTop)
        {
            ladder = ladderTop;
        }
        
        void setIfSnake(Square* snakeTail)
        {
            snake = snakeTail;
        }
        
        void setNext(Square* nextSquare)
        {
            next = nextSquare;
        }
};

class GameBoard
{
    public:
        Square* start;
        int boardLen;
        int boardWidth;

        GameBoard(int len, int width)
        {
            boardLen = len;
            boardWidth = width;
            start = new Square(1);
            Square* prev = start;
            Square* sq;
            
            for (int i = 2; i <= boardLen * boardWidth; i++)
            {
                sq = new Square(i);
                prev->setNext(sq);
                prev = sq;
            }
        }

        void display(Square* p1, Square* p2, int l, int b)
        {
            Square* sq = start;
            string x = "";
            string board = "";
            for(int i = 0; i < l; i++)
            {
                for(int j = 0; j < b; j++)
                {
                    if(i%2 == 0)
                    {
                        if(sq == p1)
                        {
                            x = x + "\033[3;43;30mP1\033[0m\t";
                        }
                        else if(sq == p2)
                        {
                            x = x + "\033[3;44;30mP2\033[0m\t";
                        }
                        else if (sq->getIfLadder())
                        {
                            x = x + "\x1B[32mL" + to_string(sq->getIfLadder()->getPos()) + "\033[0m\t";
                        }
                        else if (sq->getIfSnake())
                        {
                            x = x + "\x1B[31mS" + to_string(sq->getIfSnake()->getPos()) + "\033[0m\t";
                        }
                        else
                        {
                            x = x + to_string(sq->getPos()) + "\t";
                        }
                    }
                    else
                    {
                        if(sq == p1)
                        {
                            x = "\033[3;43;30mP1\033[0m\t" + x;
                        }
                        else if(sq == p2)
                        {
                            x = "\033[3;44;30mP2\033[0m\t" + x;
                        }
                        else if (sq->getIfLadder())
                        {
                            x = "\x1B[32mL" + to_string(sq->getIfLadder()->getPos()) + "\033[0m\t" + x;
                        }
                        else if (sq->getIfSnake())
                        {
                            x = "\x1B[31mS" + to_string(sq->getIfSnake()->getPos()) + "\033[0m\t" + x;
                        }
                        else
                        {
                            x = to_string(sq->getPos()) + "\t" + x;
                        }
                    }
                    sq = sq->getNext();
                }
                x = x + "\n";
                board = x + board;
                x = "";

            }
            cout << board;
        }  
        
        void addSnake(int tailPos, int tonguePos)
        {
            Square* currentSquare = start;

            while (currentSquare->getPos() != tailPos)
            {
                currentSquare = currentSquare->getNext();
            }
            Square* tail = currentSquare;
            while (currentSquare->getPos() != tonguePos)
            {
                currentSquare = currentSquare->getNext();
            }
            if (currentSquare->getIfLadder())
            {
                cout << "Failed! Ladder already at that position." << endl;
            }
            else
            {
                currentSquare->setIfSnake(tail);
            }
        }
        
        void addLadder(int bottomPos, int topPos)
        {
            Square* currentSquare = start;
            while (currentSquare->getPos() != bottomPos)
            {
                currentSquare = currentSquare->getNext();
            }
            Square* bottom = currentSquare;
            while (currentSquare->getPos() != topPos)
            {
                currentSquare = currentSquare->getNext();
            }
            if(currentSquare->getIfSnake())
            {
                cout << "Failed! Snake already at that position." << endl;
            }
            else
            {
                bottom->setIfLadder(currentSquare);
            }
        }
        
        int getSize()
        {
            return (boardLen * boardWidth);
        }
};

class queueEntry 
{
    public:
        Square* block;
        int dist;
        queueEntry* next;
 
        queueEntry(int distance, Square* sq,queueEntry* nextQe = nullptr)
        {
        block = sq;
        dist = distance;
        next = nextQe;
        }
};

class queue
{
    public:
        queueEntry* front;
        queueEntry* rear;
     
        queue()
        {
            front = nullptr;
            rear = nullptr;
        }

        void enqueue(queueEntry* newNode)
        { 
            if (front != nullptr) 
            { 
                rear->next = newNode; 
                rear = newNode; 
            } 

            else 
            { 
                front = newNode; 
                rear = newNode; 
            } 
        }
        
        queueEntry* dequeue()
        { 
            if (front != nullptr) 
            { 
                queueEntry* popped = front; 
                front = front->next;
                queueEntry* temp = popped; 
                // delete popped; 
                return temp;
            } 

            else 
            { 
                return nullptr; 
            } 
        }
        
        bool isEmpty()
        {
            return (front == nullptr);
        }
};

int minMoves(GameBoard gb, int n)
{
    bool visited[gb.getSize()] = {};
    queue q;
    visited[n] = 1;
    Square* player = gb.start;
    for(int i = 0; i < n; i++)
    {
        player = player->getNext();
    }

    queueEntry* qe = new queueEntry(0, player);
    // cout << qe->block->getPos();
    q.enqueue(qe);
    Square* curr;
    while(!q.isEmpty())
    {
            qe = q.dequeue();
            int vertex = qe->block->getPos();
            curr = qe->block;
            // cout << endl << "v" << vertex << ":" << qe->dist<< endl;
            if (vertex == gb.getSize())
            {
                break;
            }

            for (int j = vertex + 1; (j <= (vertex + 6)) && (j <= gb.getSize()); j++)
            {
                curr = curr->getNext();
                // cout<<"pos: "<<curr->getPos()<<";;";
                if(!visited[j-1])
                {
                    // std :: cout << j << ", ";
                    queueEntry* newEntry;
                    visited[j-1] = 1;
                    if (curr->getIfLadder())
                    {
                        newEntry = new queueEntry((qe->dist)+1, curr->getIfLadder());
                        // cout<<"ladderd\n";
                    }
                    else if (curr->getIfSnake())
                    {
                        newEntry = new queueEntry(qe->dist+1, curr->getIfSnake());
                        // cout<<"snakd\n";
                    }
                    else
                    {
                        newEntry = new queueEntry((qe->dist)+1, curr);
                        // cout<<qe->dist<<endl;
                    }
                    q.enqueue(newEntry);
                }
            }
            // cout<<"visited:  ";
            // for(int i = 0; i < 25; i++)
            // {
            //     cout<<visited[i]<<";";
            // }
    }

    return qe->dist;
}

int main()
{
    int choice, l, b, ch;
    bool flag = 1;
    int win = 0;
    GameBoard* board;
    Square* p1;
    Square* p2;
    cout << "Starting Game";
    Sleep(1000);
    cout << ".";
    Sleep(1000);
    cout << ".";
    Sleep(1000);
    cout << ".";
    system("cls");
    while(flag)//creation of board
    {
        cout << "Choose Option: " << endl;
        cout << "1. Use standard Board" << endl;
        cout << "2. Create Board" << endl;
        cin >> choice;
        bool boardFlag = 1;
        switch(choice)
        {
            case 1: 
                cin.get();
                board = new GameBoard(10,10);
                p1 = board->start;
                p2 = board->start;
                board->addLadder(2,55);
                board->addLadder(13,27);
                board->addLadder(31,67);
                board->addLadder(38,65);
                board->addLadder(60,62);
                board->addLadder(69,87);
                board->addLadder(76,95);
                board->addSnake(6,26);
                board->addSnake(11,30);
                board->addSnake(16,42);
                board->addSnake(8,75);
                board->addSnake(58,82);
                board->addSnake(70,89);
                board->addSnake(44,97);
                flag = 0;
                break;
            
            case 2:
                cin.get();
                cout <<"Enter size (length x breadth) of the board: ";
                cin >> l >> b;
                if((l <= 0) || (b <= 0))
                {
                    cout << "Please enter a valid size.";
                    break;
                }
                board = new GameBoard(l,b);
                p1 = board->start;
                p2 = board->start;
                while(boardFlag)
                {
                    cout << "Enter choice: 1. Add snake\t2. Add ladder\t3. Finish" << endl;
                    cin >> ch;
                    switch(ch)
                    {
                        case 1:
                            int to, ta;
                            cout << "Enter tounge and tail number of snake: ";
                            cin >> to >> ta;
                            if ((to > l*b - 1) || (to <= 0) || (ta < 0) || (ta > to))
                            {
                                cout << "Please enter a valid snake." << endl;
                                break;
                            }
                            board->addSnake(to,ta);
                            break;

                        case 2:
                            int bo, top;
                            cout << "Enter bottom and top number of ladder: ";
                            cin >> bo >> top;
                            if ((top > l*b) || (top <= 0) || (bo < 0) || (bo > top))
                            {
                                cout << "Please enter a valid ladder." << endl;
                                break;
                            }
                            board->addLadder(bo,top);
                            break;

                        case 3:
                            boardFlag = 0;
                            break;

                        default: 
                            cout << "Please choose a valid option." << endl;
                    }
                } 
                flag = 0;
                break;
            
            default: 
                cout << "Please choose a valid option." << endl;
        }

    }
    while(!win)
    {
        bool p1Turn = 0;
        bool p2Turn = 0;
        int moves;
        int k;
        system("cls");
        cout << "Player 1's turn." << endl << endl;
        while(!p1Turn)
        {
            cout << "Minimum number of moves for you to win: " << minMoves(*board, p1->getPos()) << endl;
            board->display(p1, p2, board->boardLen, board->boardWidth);
            cout << endl << "Press any key to roll dice.";
            cin.get();
            cout << endl << "Dice roll: ";
            srand(time(0));
            moves = rand() % 6 + 1;
            cout << moves << endl;
            k = moves;

            while(moves--)
            {
                p1 = p1->getNext();
                if (p1->getPos() == board->getSize())
                {
                    win = 1;
                    break;
                }
            }
            if(win)
            {
                break;
            }
            if (p1->getIfLadder())
            {
                cout << "Congratulations! You climbed a ladder!" << endl;
                p1 = p1->getIfLadder();
            }
            else if(p1->getIfSnake())
            {
                cout << "Oops! You got bit by a snake!" << endl;
                p1 = p1->getIfSnake();
            }
            
            if(k != 6)
            {
                p1Turn = 1;
                cout << "press any key to end turn";
                cin.get();
            }
            else
            {
                system("cls");
                cout << "Player 1's turn." << endl << endl;
                cout << "You rolled a 6! You get to roll again." << endl;
            }
        }
        if(win) 
        {
            break;
        }
        
        system("cls");
        cout << "Player 2' turn." << endl << endl;
        while(!p2Turn)
        {
            cout << "Minimum number of moves for you to win: " << minMoves(*board, p2->getPos()) << endl;
            board->display(p1, p2, board->boardLen, board->boardWidth);
            cout << endl << "Press any key to roll dice.";
            cin.get();
            cout << endl << "Dice roll: ";
            moves = rand() % 6 + 1;
            cout << moves << endl;
            k = moves;

            while(moves--)
            {
                p2 = p2->getNext();
                if (p2->getPos() == board->getSize())
                {
                    win = 2;
                    break;
                }
            }
            if(win)
            {
                break;
            }
            if (p2->getIfLadder())
            {
                cout << "Congratulations! You climbed a ladder!" << endl;
                p2 = p2->getIfLadder();
            }
            else if(p2->getIfSnake())
            {
                cout << "Oops! You got bit by a snake!" << endl;
                p2 = p2->getIfSnake();
            }

            if(k != 6)
            {
                p2Turn = 1;
                cout << "press any key to end turn";
                cin.get();
            }
            else
            {
                system("cls");
                cout << "Player 2's turn." << endl << endl;
                cout << "You rolled a 6! You get to roll again." << endl;
            }
        }
        if(win)
        {
            break;
        }
    }

    system("cls");
    if(win == 1)
    {
        cout << "CONGRATULATIONS \033[3;43;30mPLAYER 1!\033[0m\n\t YOU WON!";
    }
    else
    { 
        cout << "CONGRATULATIONS \033[3;44;30mPLAYER 2!\033[0m\n\t YOU WON!";
    }
    cin.get();
}