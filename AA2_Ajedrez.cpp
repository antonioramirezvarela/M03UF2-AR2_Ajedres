#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

// Definiciones

class Piece {
protected:
	string color;
	string type;

public:
	Piece(string _color, string _type) : color(_color), type(_type) {}
	virtual ~Piece() {}
	virtual bool isValidMove(int fromX, int fromY, int toX, int toY, vector<vector<Piece*>>& board) = 0;
	string getColor() const { return color; }
	string getType() const { return type; }
};

class Pawn : public Piece {
public:
	Pawn(string _color) : Piece(_color, "Pawn") {}

	bool isValidMove(int fromX, int fromY, int toX, int toY, vector<vector<Piece*>>& board) override {
		int dir = (color == "White") ? -1 : 1;

		// Movimiento básico hacia delante
		if (toX == fromX && toY == fromY + dir && board[toX][toY] == nullptr)
			return true;

		// Primer movimiento de dos casillas
		if (toX == fromX && toY == fromY + 2 * dir && fromY == ((color == "White") ? 6 : 1) && board[toX][toY] == nullptr && board[toX][toY - dir] == nullptr)
			return true;

		// Captura diagonal
		if (abs(toX - fromX) == 1 && toY == fromY + dir && board[toX][toY] != nullptr && board[toX][toY]->getColor() != color)
			return true;

		return false;
	}
};

// Clases de piezas

class Rook : public Piece {
public:
	Rook(string _color) : Piece(_color, "Rook") {}

	bool isValidMove(int fromX, int fromY, int toX, int toY, vector<vector<Piece*>>& board) override {
		if (fromX == toX) { // Moverse horizontalmente
			int step = (toY - fromY) / abs(toY - fromY);
			for (int y = fromY + step; y != toY; y += step) {
				if (board[fromX][y] != nullptr) {
					return false; // Hay una pieza en el camino
				}
			}
			return true;
		} else if (fromY == toY) { // Moverse verticalemte
			int step = (toX - fromX) / abs(toX - fromX);
			for (int x = fromX + step; x != toX; x += step) {
				if (board[x][fromY] != nullptr) {
					return false; // Hay una pieza en el camino
				}
			}
			return true;
		}
		return false;
	}
};

class Knight : public Piece {
public:
	Knight(string _color) : Piece(_color, "Knight") {}

	bool isValidMove(int fromX, int fromY, int toX, int toY, vector<vector<Piece*>>& board) override {
		int dx = abs(toX - fromX);
		int dy = abs(toY - fromY);
		return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
	}
};

class Bishop : public Piece {
public:
	Bishop(string _color) : Piece(_color, "Bishop") {}

	bool isValidMove(int fromX, int fromY, int toX, int toY, vector<vector<Piece*>>& board) override {
		if (abs(toX - fromX) == abs(toY - fromY)) {
			int xDir = (toX - fromX) > 0 ? 1 : -1;
			int yDir = (toY - fromY) > 0 ? 1 : -1;
			for (int x = fromX + xDir, y = fromY + yDir; x != toX; x += xDir, y += yDir) {
				if (board[x][y] != nullptr) {
					return false; // Hay una pieza en el camino
				}
			}
			return true;
		}
		return false;
	}
};

class Queen : public Piece {
public:
	Queen(string _color) : Piece(_color, "Queen") {}

	bool isValidMove(int fromX, int fromY, int toX, int toY, vector<vector<Piece*>>& board) override {
		return Rook(color).isValidMove(fromX, fromY, toX, toY, board) || Bishop(color).isValidMove(fromX, fromY, toX, toY, board);
	}
};

class King : public Piece {
public:
	King(string _color) : Piece(_color, "King") {}

	bool isValidMove(int fromX, int fromY, int toX, int toY, vector<vector<Piece*>>& board) override {
		int dx = abs(toX - fromX);
		int dy = abs(toY - fromY);
		return dx <= 1 && dy <= 1;
	}
};

class Board {
private:
	vector<vector<Piece*>> squares;

public:
	Board() {
		squares.resize(8, vector<Piece*>(8, nullptr));
	}

	~Board() {
		for (int i = 0; i < 8; ++i) {
			
			for (int j = 0; j < 8; ++j) {
				delete squares[i][j];


			}
		}
	}

	void initialize() {
	// Inicializar las piezas en el tablero
		squares[0][0] = new Rook("Black");
		squares[0][1] = new Knight("Black");
		squares[0][2] = new Bishop("Black");
		squares[0][3] = new Queen("Black");
		squares[0][4] = new King("Black");
		squares[0][5] = new Bishop("Black");
		squares[0][6] = new Knight("Black");
		squares[0][7] = new Rook("Black");

		squares[7][0] = new Rook("White");
		squares[7][1] = new Knight("White");
		squares[7][2] = new Bishop("White");
		squares[7][3] = new Queen("White");
		squares[7][4] = new King("White");
		squares[7][5] = new Bishop("White");
		squares[7][6] = new Knight("White");
		squares[7][7] = new Rook("White");

		for (int i = 0; i < 8; ++i) {
			squares[1][i] = new Pawn("Black");
			squares[6][i] = new Pawn("White");
		}
	}

	void print() {
		cout << " a b c d e f g h" << endl;
		for (int i = 0; i < 8; ++i) {
			cout << 8 - i << " ";
			for (int j = 0; j < 8; ++j) {
				if (squares[i][j] != nullptr) {
					cout << squares[i][j]->getType()[0] << squares[i][j]->getColor()[0] << " ";

				}
				else {
					cout << "- ";
				}
			}
			cout << endl;
		}
	}

	Piece* getPiece(int x, int y) {
		if (x < 0 || x >= 8 || y < 0 || y >= 8)
			return nullptr;
		return squares[x][y];
	}
	void setPiece(int x, int y, Piece* piece) {
		squares[x][y] = piece;
	}
};

class ChessGame {
private:
	Board board;
	bool isWhiteTurn;

public:
	ChessGame() : isWhiteTurn(true) {}

	void start() {
		board.initialize();
		while (!isGameOver()) {
			board.print();
			if (isWhiteTurn) {
				cout << "Turno de las blancas." << endl;
			}
			else {
				cout << "Turno de las negras." << endl;
			}
			if (!makeMove()) {
				cout << "Movimiento no válido, inténtalo de nuevo." << endl;
			}
			isWhiteTurn = !isWhiteTurn;
		}
		cout << "Fin del juego." << endl;
	}

	bool isGameOver() {
		// Verificar si e rey está en jaque mate
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				Piece* piece = board.getPiece(i, j);
				if (piece != nullptr && piece->getType() == "King" && isKingInCheck(i, j)) {
					cout << "Jaquemate. El juego ha terminado." << endl;
					return true;
				}
			}
		}
		return false;
	}

	bool isKingInCheck(int kingX, int kingY) {
		return false;
	}

	bool makeMove() {
		int fromX, fromY, toX, toY;
		cout << "Ingrese las coordenadas de la casilla de la pieza que desea mover (dila columna) ";
		char fromCol, toCol;
		cin >> fromCol >> fromY;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Ingrese las coordenadas de destino para la pieza (fila columna): ";
		cin >> toCol >> toY;
		fromX = 8 - (fromY - '0');
		toX = 8 - (toX - '0');
		fromY = fromCol - 'a';
		toY = toCol - 'a';

		Piece* piece = board.getPiece(fromX, fromY);
		if (piece == nullptr || (piece->getColor() == "White" && !isWhiteTurn) || (piece->getColor() == "Black" && isWhiteTurn)) {
			return false; // Intento de mover una pieza del color incorrecto
		}
	}

		// Realizar el movimiento
		board.setPiece(toX, toY, piece);
		board.setPiece(fromX, fromY, nullptr);
		if (destPiece != nullptr) {
			delete destPiece; // Eliminar la pieza capturada si hay una
		}

		if (!piece->isValidMove(fromX, fromY, toX, toY, board.squares)) {
			return false; // Movimiento no válido para la pieza
		}

		Piece* destPiece = board.getPiece(toX, toY);
		if (destPiece != nullptr && destPiece->getColor() == piece->getColor()) {
			return false; //Intento de capturar una pieza del mismo color
		}

		return true;
	}
	};


	int main() {
		ChessGame game;
		game.start();
		return 0;
	}