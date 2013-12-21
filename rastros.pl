
/*******************************************************
 * Libraries
 *******************************************************/

:- use_module(library(random)), use_module(library(socket)).

/*******************************************************
 * Auxiliary predicates
 *******************************************************/

nthElem(1, [X | _Xs], X).
nthElem(N, [_X | Xs], _Y) :- N1 is N - 1, nthElem(N1, Xs, _Y).

getElemPos(X, [X | _Xs], 1).
getElemPos(X, [_Y | Xs], N) :- getElemPos(X, Xs, N1), N is N1 + 1.


/*******************************************************
 * Data structures
 *******************************************************/

cell(_HigherBoardCell, _MediumBoardCell, _LowerBoardCell).

repeatI.
repeatI :- write('Invalid input!\n'), repeatI.

emptyPlace('_').
emptyPlace('1').
emptyPlace('2').


/*******************************************************
 * Board Creation/Printing
 *******************************************************/

init2Pos(1, 7).
init1Pos(7, 1).
initRPos(3, 5).


createBoard(Board) :- Board = [ [cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('2', 'X', 'X')], 
						[cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('_', 'X', 'X')], 
						[cell('_', 'X', 'X'), cell('_', 'X', 'X'), cell('_', '_', 'X'), cell('_', '_', 'X'), cell('_', 'O', 'X'), cell('_', 'X', 'X'), cell('_', 'X', 'X')], 
						[cell('_', 'X', '_'), cell('_', 'X', '_'), cell('_', '_', '_'), cell('_', '_', '_'), cell('_', '_', '_'), cell('_', 'X', '_'), cell('_', 'X', '_')], 
						[cell('X', 'X', '_'), cell('X', 'X', '_'), cell('X', '_', '_'), cell('X', '_', '_'), cell('X', '_', '_'), cell('X', 'X', '_'), cell('X', 'X', '_')], 
						[cell('X', 'X', '_'), cell('X', 'X', '_'), cell('X', 'X', '_'), cell('X', 'X', '_'), cell('X', 'X', '_'), cell('X', 'X', '_'), cell('X', 'X', '_')], 
						[cell('X', 'X', '1'), cell('X', 'X', '_'), cell('X', 'X', '_'), cell('X', 'X', '_'), cell('X', 'X', '_'), cell('X', 'X', '_'), cell('X', 'X', '_')] ].

printBoard(Board) :- write('Eagle Eye:\n\n'), printBoardEagleEye(Board),
					write('\n\nIndividualBoards:\n\n'), printIndBoards(Board).
						
printBoardEagleEye([]).
printBoardEagleEye([X | Xs]) :- write('\n'), printRow(X), printBoardEagleEye(Xs).

printRow([]) :- write('\n').
printRow([X | Xs]) :- X = cell(S, M, L), write('('), write(S), write(','),  write(M), write(','),  write(L), write(') '), printRow(Xs).

printIndBoards(Board) :- write('Higher\n'), printIndBoard(Board, 1), 
						write('\nMedium\n'), printIndBoard(Board, 2), 
						write('\nLower\n'), printIndBoard(Board, 3).

printIndBoard([], _).
printIndBoard([X | Xs], Level) :- printIndBoardRow(X, Level), printIndBoard(Xs, Level).

printIndBoardRow([], _Level) :- write('\n').
printIndBoardRow([X | Xs], 1) :- X = cell(S, _, _), write(' '), write(S), printIndBoardRow(Xs, 1).
printIndBoardRow([X | Xs], 2) :- X = cell(_, M, _), write(' '), write(M), printIndBoardRow(Xs, 2).
printIndBoardRow([X | Xs], 3) :- X = cell(_, _, L), write(' '), write(L), printIndBoardRow(Xs, 3).


/*******************************************************
 * Game loop
 *******************************************************/

playGame :- askGameMode, createBoard(Board), !, playGame(1, Board).


playGame(Player, Board) :- hasGameEnded(Board, Player).
						
playGame(Player, Board) :- move(Player, Board, FinalBoard), 
						getNextPlayer(Player, NextPlayer),
						!, 
						playGame(NextPlayer, FinalBoard).

getNextPlayer(1, 2).
getNextPlayer(2, 1).


/*******************************************************
 * Player actions
 
  1 - moveUp
  2 - moveDown
  3 - moveLeft
  4 - moveRight
  5 - moveDiagUpLeft
  6 - moveDiagUpRight
  7 - moveDiagDownLeft
  8 - moveDiagDownRight
  9 - rotateCenter
 10 - climb
 11 - descend
 12 - exit
 *******************************************************/

tests([testMoveUp, testMoveDown, testMoveLeft, testMoveRight, testMoveDiagUpLeft, testMoveDiagUpRight, 
	testMoveDiagDownLeft, testMoveDiagDownRight, testRotateCenter, testClimb, testDescend]).

moves([moveUp, moveDown, moveLeft, moveRight, moveDiagUpLeft, moveDiagUpRight, 
	moveDiagDownLeft, moveDiagDownRight, rotateCenter, climb, descend]).

move(Player, Board, NextBoard) :- player(Player, T), T = human, move(Player, Board, NextBoard, 0).
move(Player, Board, NextBoard) :- player(Player, T), T = computer, moveAuto(Player, Board, NextBoard).
move(_Player, Board, Board, 2).
move(Player, Board, NextBoard, PrevState) :- printBoard(Board),
									findMoves(Board, List, PrevState), 
									promptPlay(Player, List, Move, PrevState, NextState), 
									updateBoard(Board, Move, IntermBoard),
									!, move(Player, IntermBoard, NextBoard, NextState).

promptPlay(Player, List, Move, PrevState, NextState) :- printChoices(Player, List), 
														receiveChoice(Move), 
														validateChoice(List, Move, PrevState, NextState).

printChoices(Player, List):- write('\nPlayer '), write(Player), write('\nWhat do you want to do?\n'), printChoices(List).
printChoices([]).
printChoices([1 | Ls]) :- write('(1) - Move up\n'), printChoices(Ls).
printChoices([2 | Ls]) :- write('(2) - Move down\n'), printChoices(Ls).
printChoices([3 | Ls]) :- write('(3) - Move left\n'), printChoices(Ls).
printChoices([4 | Ls]) :- write('(4) - Move right\n'), printChoices(Ls).
printChoices([5 | Ls]) :- write('(5) - Move diagonally up-left\n'), printChoices(Ls).
printChoices([6 | Ls]) :- write('(6) - Move diagonally up-right\n'), printChoices(Ls).
printChoices([7 | Ls]) :- write('(7) - Move diagonally down-left\n'), printChoices(Ls).
printChoices([8 | Ls]) :- write('(8) - Move diagonally down-right\n'), printChoices(Ls).
printChoices([9 | Ls]) :- write('(9) - Rotate center board\n'), printChoices(Ls).
printChoices([10 | Ls]) :- write('(10) - Climb one level\n'), printChoices(Ls).
printChoices([11 | Ls]) :- write('(11) - Descend one level\n'), printChoices(Ls).
printChoices([12 | Ls]) :- write('(12) - End turn\n'), printChoices(Ls).

receiveChoice(Move) :- repeatI, read(Move), get_char(_).

validateChoice(_List, 12, 1, 2).
validateChoice(List, Move, 0, 1) :- Move < 10, member(Move, List).
validateChoice(List, Move, 0, 0) :- member(Move, List).
validateChoice(List, Move, 1, 1) :- member(Move, List).

updateBoard(Board, 12, Board).
updateBoard(Board, N, IntermBoard) :- moves(MovesList), nthElem(N, MovesList, M), X =.. [M, Board, IntermBoard], X.

/*******************************************************
 * Moves testing
 *******************************************************/

findMoves(Board, List, 0) :- iteratePossibleMoves(Board, List, 1).
findMoves(Board, List, 1) :- iteratePossibleMoves(Board, L1, 10), append(L1, [12], List).

iteratePossibleMoves(_Board, [], 12) :- !.
iteratePossibleMoves(Board, [N | Ls], N) :- tests(TestList), nthElem(N, TestList, T),
									X =.. [T, Board], X, N1 is N + 1,
									!, iteratePossibleMoves(Board, Ls, N1).
iteratePossibleMoves(Board, Ls, N) :- N < 12, N1 is N + 1,
									!, iteratePossibleMoves(Board, Ls, N1).
/*******************************************************
 * Player actions
 
  1 - moveUp
  2 - moveDown
  3 - moveLeft
  4 - moveRight
  5 - moveDiagUpLeft
  6 - moveDiagUpRight
  7 - moveDiagDownLeft
  8 - moveDiagDownRight
  9 - rotateCenter
 10 - climb
 11 - descend
 12 - exit
 *******************************************************/
testMove(1, Board) :- testMoveUp(Board), writeResponseToStream("Success.").
testMove(2, Board) :- testMoveDown(Board), writeResponseToStream("Success.").
testMove(3, Board) :- testMoveLeft(Board), writeResponseToStream("Success.").
testMove(4, Board) :- testMoveRight(Board), writeResponseToStream("Success.").
testMove(5, Board) :- testMoveDiagUpLeft(Board), writeResponseToStream("Success.").
testMove(6, Board) :- testMoveDiagUpRight(Board), writeResponseToStream("Success.").
testMove(7, Board) :- testMoveDiagDownLeft(Board), writeResponseToStream("Success.").
testMove(8, Board) :- testMoveDiagDownRight(Board), writeResponseToStream("Success.").
testMove(9, Board) :- testRotateCenter(Board), writeResponseToStream("Success.").
testMove(10, Board) :- testClimb(Board), writeResponseToStream("Success.").
testMove(11, Board) :- testDescend(Board), writeResponseToStream("Success.").
testMove(_, _) :- writeResponseToStream("Failure.").

testMoveUp(Board) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr - 1, 
					getElemInPos(Nr1, Nc, Board, Level, X), emptyPlace(X).
testMoveDown(Board) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr + 1, 
					getElemInPos(Nr1, Nc, Board, Level, X), emptyPlace(X).
testMoveLeft(Board) :- getCurrPos(Board, Nr, Nc, Level), Nc1 is Nc - 1, 
					getElemInPos(Nr, Nc1, Board, Level, X), emptyPlace(X). 
testMoveRight(Board) :- getCurrPos(Board, Nr, Nc, Level), Nc1 is Nc + 1, 
						getElemInPos(Nr, Nc1, Board, Level, X), emptyPlace(X).
testMoveDiagUpLeft(Board) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr - 1, Nc1 is Nc - 1,
							getElemInPos(Nr1, Nc1, Board, Level, X), emptyPlace(X).
testMoveDiagUpRight(Board) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr - 1, Nc1 is Nc + 1,
									getElemInPos(Nr1, Nc1, Board, Level, X), emptyPlace(X). 
testMoveDiagDownLeft(Board) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr + 1, Nc1 is Nc - 1,
									getElemInPos(Nr1, Nc1, Board, Level, X), emptyPlace(X). 
testMoveDiagDownRight(Board) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr + 1, Nc1 is Nc + 1,
									getElemInPos(Nr1, Nc1, Board, Level, X), emptyPlace(X). 

testRotateCenter(Board) :- getCurrPos(Board, Nr, Nc, _Level),
						Nr < 6 , Nr > 2, Nc < 6, Nc > 2.

testClimb(Board) :- getCurrPos(Board, Nr, Nc, Level), Level1 is Level - 1,
					getElemInPos(Nr, Nc, Board, Level1, '_').

testDescend(Board) :- getCurrPos(Board, Nr, Nc, Level), Level1 is Level + 1,
					getElemInPos(Nr, Nc, Board, Level1, '_').
						 
/*******************************************************
 * Movement
 *******************************************************/

moveUp(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr - 1, 
							getElemInPos(Nr1, Nc, Board, Level, X), emptyPlace(X),
							changeElemInMatrix(Board, Nr, Nc, Level, '#', IntermBoard),
							changeElemInMatrix(IntermBoard, Nr1, Nc, Level, 'O', NextBoard).

moveDown(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr + 1, 
							getElemInPos(Nr1, Nc, Board, Level, X), emptyPlace(X),
							changeElemInMatrix(Board, Nr, Nc, Level, '#', IntermBoard),
							changeElemInMatrix(IntermBoard, Nr1, Nc, Level, 'O', NextBoard).

moveLeft(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, Level), Nc1 is Nc - 1, 
							getElemInPos(Nr, Nc1, Board, Level, X), emptyPlace(X),
							changeElemInMatrix(Board, Nr, Nc, Level, '#', IntermBoard),
							changeElemInMatrix(IntermBoard, Nr, Nc1, Level, 'O', NextBoard).

moveRight(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, Level), Nc1 is Nc + 1, 
							getElemInPos(Nr, Nc1, Board, Level, X), emptyPlace(X),
							changeElemInMatrix(Board, Nr, Nc, Level, '#', IntermBoard),
							changeElemInMatrix(IntermBoard, Nr, Nc1, Level, 'O', NextBoard).

moveDiagUpLeft(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr - 1, Nc1 is Nc - 1,
									getElemInPos(Nr1, Nc1, Board, Level, '_'),
									changeElemInMatrix(Board, Nr, Nc, Level, '#', IntermBoard),
									changeElemInMatrix(IntermBoard, Nr1, Nc1, Level, 'O', NextBoard).

moveDiagUpRight(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr - 1, Nc1 is Nc + 1,
									getElemInPos(Nr1, Nc1, Board, Level, X), emptyPlace(X),
									changeElemInMatrix(Board, Nr, Nc, Level, '#', IntermBoard),
									changeElemInMatrix(IntermBoard, Nr1, Nc1, Level, 'O', NextBoard).

moveDiagDownLeft(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr + 1, Nc1 is Nc - 1,
									getElemInPos(Nr1, Nc1, Board, Level, X), emptyPlace(X),
									changeElemInMatrix(Board, Nr, Nc, Level, '#', IntermBoard),
									changeElemInMatrix(IntermBoard, Nr1, Nc1, Level, 'O', NextBoard).

moveDiagDownRight(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, Level), Nr1 is Nr + 1, Nc1 is Nc + 1,
									getElemInPos(Nr1, Nc1, Board, Level, X), emptyPlace(X),
									changeElemInMatrix(Board, Nr, Nc, Level, '#', IntermBoard),
									changeElemInMatrix(IntermBoard, Nr1, Nc1, Level, 'O', NextBoard).

rotateCenter(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, _Level),
								Nr < 6 , Nr > 2, Nc < 6, Nc > 2,
								getElemInPos(3, 3, Board, 2, Cell1),
								getElemInPos(3, 4, Board, 2, Cell2),
								getElemInPos(3, 5, Board, 2, Cell3),
								getElemInPos(4, 3, Board, 2, Cell4),
								getElemInPos(4, 5, Board, 2, Cell6),
								getElemInPos(5, 3, Board, 2, Cell7),
								getElemInPos(5, 4, Board, 2, Cell8),
								getElemInPos(5, 5, Board, 2, Cell9),
								changeElemInMatrix(Board, 3, 3, 2, Cell7, IntermBoard1),
								changeElemInMatrix(IntermBoard1, 3, 4, 2, Cell4, IntermBoard2),
								changeElemInMatrix(IntermBoard2, 3, 5, 2, Cell1, IntermBoard3),
								changeElemInMatrix(IntermBoard3, 4, 3, 2, Cell8, IntermBoard4),
								changeElemInMatrix(IntermBoard4, 4, 5, 2, Cell2, IntermBoard5),
								changeElemInMatrix(IntermBoard5, 5, 3, 2, Cell9, IntermBoard6),
								changeElemInMatrix(IntermBoard6, 5, 4, 2, Cell6, IntermBoard7),
								changeElemInMatrix(IntermBoard7, 5, 5, 2, Cell3, NextBoard).								

climb(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, Level), Level1 is Level - 1,
						getElemInPos(Nr, Nc, Board, Level1, '_'),
						changeElemInMatrix(Board, Nr, Nc, Level, '_', IntermBoard),
						changeElemInMatrix(IntermBoard, Nr, Nc, Level1, 'O', NextBoard).

descend(Board, NextBoard) :- getCurrPos(Board, Nr, Nc, Level), Level1 is Level + 1,
						getElemInPos(Nr, Nc, Board, Level1, '_'),
						changeElemInMatrix(Board, Nr, Nc, Level, '_', IntermBoard),
						changeElemInMatrix(IntermBoard, Nr, Nc, Level1, 'O', NextBoard).

/*******************************************************
 * Position accessing/modification
 *
 * 1 - Higher Level / 2 - Medium Level / 3 - Lower Level
 *******************************************************/
						
getCurrPos(Board, Nr, Nc, 1) :- getElemPos(Row, Board, Nr), getElemPos(cell('O', _, _), Row, Nc).
getCurrPos(Board, Nr, Nc, 2) :- getElemPos(Row, Board, Nr), getElemPos(cell(_, 'O', _), Row, Nc).
getCurrPos(Board, Nr, Nc, 3) :- getElemPos(Row, Board, Nr), getElemPos(cell(_, _, 'O'), Row, Nc).

getCell(Nr, Nc, Board, Cell) :- nthElem(Nr, Board, Row), nthElem(Nc, Row, Cell).

getElemInPos(Nr, Nc, Board, 1, Elem) :- nthElem(Nr, Board, Row), nthElem(Nc, Row, cell(Elem, _, _)).
getElemInPos(Nr, Nc, Board, 2, Elem) :- nthElem(Nr, Board, Row), nthElem(Nc, Row, cell(_, Elem, _)).
getElemInPos(Nr, Nc, Board, 3, Elem) :- nthElem(Nr, Board, Row), nthElem(Nc, Row, cell(_, _, Elem)).

changeElemInMatrix(OldMtrx, Nr, Nc, Level, NewElem, NewMtrx) :- changeElemInMatrix(OldMtrx, Nr, Nc, Level, NewElem, NewMtrx, 1).
changeElemInMatrix([O | Os], Nr, Nc, Level, NewElem,  [N | Os], Nr) :- changeElemInRow(O, Nc, Level, NewElem, N).
changeElemInMatrix([O | Os], Nr, Nc, Level, NewElem, [O | Ns], RowIt) :- RowIt1 is RowIt + 1, changeElemInMatrix(Os, Nr, Nc, Level, NewElem, Ns, RowIt1).

changeElemInRow(OldRow , Nc, Level, NewElem, NewRow) :- changeElemInRow(OldRow, Nc, Level, NewElem, NewRow, 1).
changeElemInRow([O | Os], Nc, 1, NewElem, [N | Os], Nc) :- O = cell(_, M, L), N = cell(NewElem, M, L).
changeElemInRow([O | Os], Nc, 2, NewElem, [N | Os], Nc) :- O = cell(H, _, L), N = cell(H, NewElem, L).
changeElemInRow([O | Os], Nc, 3, NewElem, [N | Os], Nc) :- O = cell(H, M, _), N = cell(H, M, NewElem).
changeElemInRow([O | Os], Nc, Level, NewElem, [O | Ns], ColIt) :- ColIt1 is ColIt + 1, changeElemInRow(Os, Nc, Level, NewElem, Ns, ColIt1).

changeCellInMatrix(OldMtrx, Nr, Nc, NewCell, NewMtrx) :- changeCellInMatrix(OldMtrx, Nr, Nc, NewCell, NewMtrx, 1).
changeCellInMatrix([O | Os], Nr, Nc, NewCell,  [N | Os], Nr) :- changeCellInRow(O, Nc, NewCell, N).
changeCellInMatrix([O | Os], Nr, Nc, NewCell, [O | Ns], RowIt) :- RowIt1 is RowIt + 1, changeCellInMatrix(Os, Nr, Nc, NewCell, Ns, RowIt1).

changeCellInRow(OldRow , Nc, NewCell, NewRow) :- changeCellInRow(OldRow, Nc, NewCell, NewRow, 1).
changeCellInRow([_O | Os], Nc, NewCell, [NewCell | Os], Nc).
changeCellInRow([O | Os], Nc, NewCell, [O | Ns], ColIt) :- ColIt1 is ColIt + 1, changeCellInRow(Os, Nc, NewCell, Ns, ColIt1).

/*******************************************************
 * Game ending
 *******************************************************/

hasGameEnded(Board, _Player) :- getCurrPos(Board, 1, 7, 1), declareWinner(2).
hasGameEnded(Board, _Player) :- getCurrPos(Board, 7, 1, 3), declareWinner(1).
hasGameEnded(Board, Player) :- findMoves(Board, L, 0), L = [], getNextPlayer(Player, Winner), declareWinner(Winner).
hasGameEnded(Board, Player) :- findMoves(Board, L, 0), L = [9], getNextPlayer(Player, Winner), declareWinner(Winner).

declareWinner(Player) :- write('CONGRATULATIONS PLAYER '), write(Player), write(', YOU HAVE WON!!!\n').

/*******************************************************
 * Game Modes
 *******************************************************/
 
 askGameMode :- write('Select game mode:\n'),
				write('(1) - Human vs Human\n'),
				write('(2) - Human vs Computer\n'),
				write('(3) - Computer vs Human\n'),
				write('(4) - Computer vs Computer\n'),
				receiveGameMode(Mode),
				validateGameMode(Mode),
				retractall(player(_,_)), retractall(level(_)),
				setGameMode(Mode).

receiveGameMode(Mode) :- repeatI, read(Mode), get_char(_).

validateGameMode(1).
validateGameMode(2).
validateGameMode(3).
validateGameMode(4).

setGameMode(1) :- asserta(player(1, human)), asserta(player(2, human)).
setGameMode(2) :- asserta(player(1, human)), asserta(player(2, computer)), askDifficultyLevel.
setGameMode(3) :- asserta(player(1, computer)), asserta(player(2, human)), askDifficultyLevel.
setGameMode(4) :- asserta(player(1, computer)), asserta(player(2, computer)), askDifficultyLevel.
				
 
 /*******************************************************
 * Artificial Intelligence
 *******************************************************/
 
playerPrefMoves(1, 11, [7, 2, 3, 9, 5, 8, 1, 4, 6]).
playerPrefMoves(2, 10, [6, 1, 4, 9, 8, 5, 2, 3, 7]).
 
moveAuto(Player, Board, NextBoard) :- findMoves(Board, Avail, 0),
									playerPrefMoves(Player, Dir, Pref),
									moveAuto(Player, Board, Avail, Dir, Pref, NextBoard),
									printBoard(NextBoard), write('Press [Enter] to continue\n'), get_char(_).

moveAuto(_Player, Board, AvailMoves, _Dir, [_P | _Ps], NextBoard) :- 
			level(X), random(0, 100, Y), Y =< X, random_member(Z, AvailMoves), updateBoard(Board, Z, NextBoard).

moveAuto(_Player, Board, AvailMoves, Dir, [P | _Ps], NextBoard) :-

			member(Dir, AvailMoves), updateBoard(Board, Dir, IntermBoard1),
			findMoves(IntermBoard1, AvailMoves1, 0),
			member(P, AvailMoves1), updateBoard(IntermBoard1, P, IntermBoard2),
			findMoves(IntermBoard2, AvailMoves2, 1),
			member(Dir, AvailMoves2), updateBoard(IntermBoard2, Dir, NextBoard).

moveAuto(_Player, Board, AvailMoves, Dir, [P | _Ps], NextBoard) :-
			member(Dir, AvailMoves), updateBoard(Board, Dir, IntermBoard), 
			findMoves(IntermBoard, AvailMoves1, 0),
			member(P, AvailMoves1), updateBoard(IntermBoard, P, NextBoard).
			

moveAuto(_Player, Board, AvailMoves, Dir, [P | _Ps], NextBoard) :-
			member(P, AvailMoves), updateBoard(Board, P, IntermBoard),
			findMoves(IntermBoard, AvailMoves2, 1),
			member(Dir, AvailMoves2), updateBoard(IntermBoard, Dir, NextBoard).
			
moveAuto(_Player, Board, AvailMoves, _Dir, [P | _Ps], NextBoard) :-
			member(P, AvailMoves), updateBoard(Board, P, NextBoard).

moveAuto(Player, Board, AvailMoves, Dir, [P | Ps], NextBoard) :-
			\+member(P, AvailMoves), 
			moveAuto(Player, Board, AvailMoves, Dir, Ps, NextBoard).

 /*******************************************************
 * Difficulty Level
 *******************************************************/

  askDifficultyLevel :- write('Select difficulty level:\n'),
				write('(1) - Easy\n'),
				write('(2) - Medium\n'),
				write('(3) - Hard\n'),
				write('(4) - Extreme\n'),
				receiveDifficultyLevel(Level),
				validateDifficultyLevel(Level),
				setDifficultyLevel(Level).

receiveDifficultyLevel(Level) :- repeatI, read(Level), get_char(_).

validateDifficultyLevel(1).
validateDifficultyLevel(2).
validateDifficultyLevel(3).
validateDifficultyLevel(4).

setDifficultyLevel(1) :- asserta(level(75)).
setDifficultyLevel(2) :- asserta(level(50)).
setDifficultyLevel(3) :- asserta(level(25)).
setDifficultyLevel(4) :- asserta(level(0)).


 /*******************************************************
 * Socket Interface
 *******************************************************/

port(30075).

openSocket :- port(SocketPort), tcp_socket(InitSocket), tcp_bind(InitSocket, SocketPort),
			retractall(socketStream(_)),
			asserta(socketStream(SocketPort)),
			connectToClient(InitSocket).

connectToClient(InitSocket) :- write('Waiting for connection\n'),
						tcp_listen(InitSocket, 1),
						tcp_open_socket(InitSocket, AcceptFd, _),
						tcp_accept(AcceptFd, SocketID, _Peer),
						tcp_open_socket(SocketID, IStream, OStream),
						write('Connected.\n'),
						asserta(socketInputStream(IStream)),
						asserta(socketOutputStream(OStream)).

receiveInputFromSocketStream(Messg) :- socketInputStream(IStream), receiveInputFromStream(IStream, Messg), read(IStream, Messg).

writeResponseToSocketStream(Messg) :-  socketOutputStream(OStream), format(OStream, Messg, []).

closeSocket :- socketInputStream(IStream), socketOutputStream(OStream), close(IStream), close(OStream), socketStream(Stream), close(Stream, [force(true)]).
closeSocket :- write('None socket opened yet\n').

run :- 
	openSocket,
	loop.

loop :- 
	receiveInputFromSocketStream(Messg),
	Messg,
	loop.
loop :- writeResponseToSocketStream("Failure."), loop.