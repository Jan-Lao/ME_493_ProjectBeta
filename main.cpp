//
//  main.cpp
//  Project Beta Draft 2 - Working QLearner
//
//  Created by Jan Lao on 3/18/17.
//  Copyright © 2017 University Nevada, Reno. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <assert.h>
#include <vector>
#include <random>
#include <fstream>

#define LYRAND (double)rand()/RAND_MAX

using namespace std;
long int supergoodreward = 100000;

/////================== Class: Agent ========================
class Agent{
public:
    void set_state(int s); //SENSE();
    int decide();   //DECIDE();
    void react(double r); //REACT();
    void Qtableinit();
    
    int state_alt;
    int prevstate_alt;
    
    int state;
    int prevstate;
    
    //Learner Inputs
    int action;
    double reward;
    
    double maxvalue;
    int maxindex;
    
    //Qlearner stuff
    double alpha = 0.1;
    double epsilon = 0.1;
    double gamma = 0.9;
    vector<vector<double>> Qtable;
    vector<double> Qrow;
    double Q;
    double Qmax;
    
    void TestD(vector<vector<double>> Qtable, vector<double> Qrow);
    void TestF();
};

/////============== Class: GridWorld =========================
class GridWorld {
public:
    //Preliminary Initializers
    void calc_agentstate();
    void start();
    int run(); //AGENT: ACT();
    
    //Agent Input
    int choice;
    
    //GridWorld stuff
    int x_dim;
    int y_dim;
    int body_x;
    int body_y;
    int target_x;
    int target_y;
    Agent* p1;
    
    //Qtable Construction
    vector<vector<double>> Qtable;
    vector<double> Qrow;
    
    void TestE();
    void TestG();
};

/////--------------- Agent Functions -------------------------
//// Set state function
void Agent::set_state(int s){ //Did this back in the day.
    state = s;
    state_alt = s;
}

void Agent::Qtableinit(){ // Did this back in the day.
    for(int i=0; i<25; i++){ // (x_dim+1)*(y_dim+1)
        for(int j=0; j<4; j++){ //4 zero's in one row ~ For each direction
            Qrow.push_back(LYRAND); //Initialize a row of nums close-to-zero's. Qrow is a vector per iteration (state).
        }
        //Qrow.resize(4);
        Qtable.push_back(Qrow); //Initialize a table of zero's
        Qrow.clear(); //Stop the rows from increasing in size every state iteration
    }
}


//// Decide() Function
int Agent::decide(){
    action = 0;
    double placeholder = LYRAND;
    cout << placeholder << endl; //debugging
    if (placeholder <= epsilon){
        //Choose the random action (epsilon)% of the time
        cout << "Random action taken..." << endl;
        action = rand()%4;
    }
    else if(placeholder > epsilon){ //BELOW is bubble structure found online.
        cout << "Greedy action taken..." << endl;
        //cout << Qtable.at(state).at(0) << "..." << Qtable.at(state).at(1) << "..." << Qtable.at(state).at(2) << "..." << Qtable.at(state).at(3) << endl; //debugging purposes
        
        maxindex = 0;
        maxvalue = *max_element(Qtable.at(state).begin(), Qtable.at(state).end());
        for(int c=0;c<4;c++){
            if(Qtable.at(state).at(c) == maxvalue){ //Finding its index in the original Qrow vector
                maxindex = c;
            }
        }
        action = maxindex;
        //cout << maxvalue << " ... " << maxindex << endl; //For debugging purposes prior reward structure
    }
    else{
        cout << "For some reason the placeholder did not fall within the epsilon, decide func" << endl;
    }
    cout << "The current action is: " << action << endl;
    return action;
}

//// React() Function
void Agent::react(double r){ //Buddy coded with Chris Jackson.
    //filling the Qtable per timestep using the stateful, action-value learner
    
    reward = r;
    
    Q = Qtable.at(prevstate).at(action); //Each element of a Qrow vectors
    //For equation purposes: Q = whatever is in the placement
    
    for(int d=0;d<4;d++){
        if(Qtable.at(state).at(d) == maxvalue){
            maxindex = d;
        }
    }
    
    Qmax = *max_element(Qtable.at(state).begin(), Qtable.at(state).end());
    Q = Q + alpha * (reward + gamma * Qmax - Q);
    
    //cout << "Qmax = " << Qmax << endl;
    //cout << "Reward = " << reward << endl;
    
    Qtable.at(prevstate).at(action) = Q; //Updating from values of the function
    //For Placement purposes: The placement of the table is the updated Q value
}

//// TestD() Function
void Agent::TestD(vector<vector<double>> Qtable, vector<double> Qrow){
    //int numStates = Qrow.size();
    for(int u=0; u< Qrow.size(); u++){
        assert(Q < reward);
        //comparing each Qvalue to the reward. Seeing if the reward is bigger than the Qvalue at every iteration.
    }
}

//// TestF() Function
void Agent::TestF(){
    int optimalsteps = 7; //Least number of moves to get to the goal (0,0) -> (2,3)
    assert(Qtable.size() <= optimalsteps); //NOTE~~~What is a better way to implement this?
    //THE ASSERT IS GONNA FAIL: That is why it is commented out; but this assert logic for TestF() seems sound.
}

/////------------ Gridworld functions -----------------------
//// Calc_agentstate() function
void GridWorld::calc_agentstate(){ //Did this back in the day.
    p1->prevstate = p1->state;
    p1->state = body_x + body_y * x_dim; //Calculates each cell, which is indexed numerically
    p1->set_state(p1->state);
}

//// TestE() Function
void GridWorld::TestE(){
    //Checking to see if it reinitializes
    assert(body_x == 0); //NOTE~~~What is a better way to implement this?
    assert(body_y == 0);
    //Checking to see if the Qvalue = the original placeholder when initializing the Qtable
    assert(p1->Q != 0); //The Qtable doesnt reset: Qvalue != initializer (zero or close to zero)
}

//// TestG() function
void GridWorld::TestG(){
    p1->prevstate_alt = p1->state_alt;
    p1->state_alt = body_y + body_x * y_dim; //Calc each cell based on y-dim. Find alt way to calculate?
    p1->set_state(p1->state_alt);
}

//// Start() Function
void GridWorld::start(){
    x_dim = 4;
    y_dim = 4;
    body_x = 0;
    body_y = 0;
    target_x = 2;
    target_y = 3;
}

//// Run() Function
int GridWorld::run(){
    ofstream fout("/Users/Jan/Desktop/ProjectBetaResults1.txt", fstream::trunc);
    start();
    p1->Qtableinit();
    calc_agentstate();
    TestG(); //Calculate alternate state
    for(int w=0; w<100; w++){ //100 episodes per statistical run
        int moves = 0;
        while(body_x != target_x || body_y != target_y){
            cout << "============================================================" << endl;
            cout << endl << "Q-Learner: New Iteration" << endl;
            cout << "-------------------------------------------------" << endl;
            cout << "For debug sake: the Goal is at (x,y)= (" << target_x << "," << target_y << ")" << endl;
            cout << "   Before Choice: X-Location: ((" << body_x << "))    " << "Y_Location: ((" << body_y << "))" << endl << endl;
            
            p1->decide(); //dereference the pointer to do the decide function in the agent
            choice = p1->action; //choice = dereferenced action from the agent
            
            /// Actual Movement
            if (choice == 0 ){
                body_x--;
            }
            if (choice == 1){
                body_y--;
            }
            if (choice == 2){
                body_y++;
            }
            if (choice == 3){
                body_x++;
            }
            if (choice != 0 && choice != 1 && choice != 2 && choice != 3){
                cout << "That is not a valid choice. Please enter 0, 1, 2, or 3" << endl;
            }
            cout << "The choice made was: " << choice << endl << endl;
            
            /// Boundary Teleportation
            while(body_x<0){
                body_x++;
                cout << "Agent Bounced Back into the GridWorld" << endl;
            }
            while(body_y<0){
                body_y++;
                cout << "Agent Bounced Back into the GridWorld" << endl;
            }
            while(body_x > x_dim){
                body_x--;
                cout << "Agent Bounced Back into the GridWorld" << endl;
            }
            while(body_y>y_dim){
                body_y--;
                cout << "Agent Bounced Back into the GridWorld" << endl;
            }
            
            /// Reward Structure
            double r;
            if(body_x == target_x && body_y == target_y){
                r = supergoodreward;
            }
            else {
                r = -1;
            }
            
            /// Finish off the Run()
            calc_agentstate();
            TestG(); //Calcluate alternate state
            moves++;
            p1->react(r); //dereference the pointer to the react function of the agent
            cout << "   After Choice: X-Location: ((" << body_x << "))    " << "Y_Location: ((" << body_y << "))" << endl << endl;
            cout << "The Agent/Human has not yet met the Goal..." << endl << endl;
        }
        cout << "...THE AGENT/HUMAN HAS MET THE GOAL" << endl << endl;
        cout << "============================================================" << endl;
        cout << "The moves taken are: " << moves << endl << endl;
        fout << moves << endl;
        cout << "============================================================" << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "GAME/EPISODE " << w+1 << " ENDED." << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
        start();
        TestE();
    }
    fout.close();
    p1->TestD(Qtable, Qrow);
    p1->TestF();
    //Qtable.clear();
    return 1;
}

////==================== MAIN =============================================
int main(){
    srand( static_cast<unsigned int>(time(NULL))); //debugging purposes
    
    GridWorld grid;
    Agent agent;
    
    cout << "Welcome to GridWorld, Human/Agent!" << endl;
    Agent* p1;
    p1 = &agent; //pointer references to the agent's address
    grid.p1 = p1;
    
    // Grid pointer a = agent pointer a. Inputting the agent to the grid
    
    grid.run();
    
    //If assert fails the program will stop here
    cout << "GAME/STATISTICAL RUN ENTIRELY TERMINATED." << endl << endl << endl;
    
    return 0;
}
