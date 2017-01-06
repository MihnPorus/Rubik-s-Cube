#include "RubikSolver.h"

RubikSolver::RubikSolver()
{
    //ctor
}

RubikSolver::~RubikSolver()
{
    //dtor
}

RubikSolver::RubikSolver(Cubies* cbs)
{
	goalState = Cube(solvedCube).toCubiesFromSides();
	initialState = cbs;
}


bool RubikSolver::Solve()
{
	int f;
	int depth = -1;
	int nodesCount = 0;
	int popCount = 0;
	Stack _stack;

    // Array to save solution.
    short path[30];
    path[0] = -1;

    DataBlock current;
    Cubies* NextStates[20];
    Cubies initi = *Cubies::Copy(initialState);
    Cubies *deleted ;
    int dist ;
    int statesCount ;

    while (1)
	{
		if (_stack.length == 0)
		{
			if (Solved)
                return true;
            initialState = new Cubies(initi) ;
			_stack.push(initialState, 0);
			depth++;
			printf("Searching depth %d\n", depth);
		}

    	popCount++;
		current = _stack.top();
        deleted = _stack.pop() ;
        dist = current.distance ;
		/* update the path array */
        if (dist > 0) {
            path[dist - 1] = deleted->lastOp;
            path[dist] = -1;
        }
		if (dist == depth)
		{
			/*
			*  if the cube is at current depth goal, check if it's solved.
			*/
			if (deleted->isSolved())
			{
				Solved = true;
				actionLog = new short[30];
				int i = 0;
				while (path[i] != -1)
                {
                    actionLog[i] = path[i];
                    i++;
                }
                actionLog[i] = -1;
                while (_stack.size())
                    _stack.pop_clr();
				//actionLog = current.node->Solution;
                delete deleted;

				break;
			}
		}
		else
		{
			/*
			* not at depth yet, Generate Next States, applying heuristics pruning
			*/

			deleted->GenerateNextStates(NextStates);
            statesCount = deleted->statesCount ;

            //if ( depth > 13 )
                for (int i = 0; i < statesCount; i++)
                    for (int j = i + 1; j < statesCount; j++)
                        if (NextStates[i]->Heuristic() < NextStates[j]->Heuristic()) swap(NextStates[i], NextStates[j]);


            for (int i = 0; i < statesCount; i++)
			{
				Cubies* cbs = new Cubies(*NextStates[i]);

				f = dist + cbs->Heuristic() + 1;

				if (f > depth)
                {
                    delete cbs;
                    continue;
                }

				_stack.push(cbs, dist + 1);
			}
            for ( int i = 0 ; i < statesCount ; i++ )
                delete NextStates[i] ;


    	}
	    delete deleted;

    }

    return false;
}
