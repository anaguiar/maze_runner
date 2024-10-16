#include <stdio.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

// Matriz representando o labirinto
char** maze;
// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Pos a ser explorada
struct pos_t
{
	int i;
	int j;
};

pos_t initial_pos;
// Estrutura de dados contendo as próximas posições
// a serem exploradas no labirinto
stack<pos_t> valid_positions;

/* Inserir elemento: 

	 pos_t pos;
	 pos.i = 1;
	 pos.j = 3;
	 valid_positions.push(pos)
 */
// Retornar o numero de elementos: valid_positions.size();
//Retornar o primeiro elemento do vetor: valid_positions.top();
// Remover o elemento no topo: valid_positions.pop(); 


pos_t load_maze(const char* file_name)
{
	
	// Abre o arquivo para leitura (fopen)
	 ifstream file;
    file.open(file_name);
    if (!file) {
        cout << "Error: File could not be opened.\n";
        exit(1);
    }
	// Le o numero de linhas e colunas (fscanf) 
	// e salva em num_rows e num_cols

	cout<< "aquii";
	file >> num_rows >> num_cols;
    cout << "Labirinto " << num_rows << "x" << num_cols << ":\n";

    // Aloca a matriz maze (malloc)
    maze = new char*[num_rows];
    for (int i = 0; i < num_rows; ++i) {
        maze[i] = new char[num_cols];
    }



	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			// Le o valor da linha i+1,j do arquivo e salva na posiÃ§Ã£o maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
			 file >> maze[i][j];
            if (maze[i][j] == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
				cout << "i " << i << " j "<< j <<'\n';
            }
		}
	}
	file.close();
	
	return initial_pos;
}

void print_maze()
{
	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}

int walk(pos_t pos)
{
	//marcando que se encontra na posição inicial
	maze[pos.i][pos.j] = 'o';
	// Marcar a posição atual com o sÃ­mbolo 'o'

	/* Dado a posição atual, verifica quais sao as próximas posições válidas
		Checar se as posições abaixo são validas (i>0, i<num_rows, j>0, j <num_cols)
		 e se são posições ainda não visitadas (ou seja, caracter 'x') e inserir
		 cada uma delas no vetor valid_positions
		 - pos.i, pos.j+1
		 - pos.i, pos.j-1
		 - pos.i+1, pos.j
		 - pos.i-1, pos.j
	*/
	pos_t aux;
	bool auxIf = false;
	
	if(pos.j+1 < num_cols){
		if((maze[pos.i][pos.j+1] == 'x' || maze[pos.i][pos.j+1] == 's') && !auxIf){
			aux.i= pos.i;
			aux.j= pos.j+1;
			valid_positions.push(aux);
			auxIf = true;
		}
	}

	if(pos.i+1 < num_rows){
		if((maze[pos.i+1][pos.j] == 'x'  || maze[pos.i+1][pos.j] == 's') && !auxIf){
			aux.i= pos.i+1;
			aux.j= pos.j;
			valid_positions.push(aux);
			auxIf = true;
		} 
	}

	if(pos.i-1 >= 0){
		if((maze[pos.i-1][pos.j] == 'x'  || maze[pos.i-1][pos.j] == 's') && !auxIf){
			aux.i= pos.i-1;
			aux.j= pos.j;
			valid_positions.push(aux);
			auxIf = true;
		}
	}


	if(pos.j-1 >= 0){
		if((maze[pos.i][pos.j-1] == 'x' || maze[pos.i][pos.j-1] == 's')  && !auxIf){
			aux.i= pos.i;
			aux.j= pos.j-1;
			valid_positions.push(aux);
			auxIf = true;
		}
	}	

	system("cls");
	// Imprime o labirinto
	print_maze();
	// Verifica se o vetor nao esta vazio. 
	//cout << "error"<< valid_positions.size();
	if (valid_positions.empty()) {
        cout << "Não existem novas opções não visitadas \n";
        return 0;
    }

	// Caso não esteja, pegar o primeiro valor de 
	// valid_positions, remove-lo e chamar a funão walk com esse valor
	pos_t next_position = valid_positions.top();
	valid_positions.pop();
	maze[pos.i][pos.j] = '.';
	//Caso alguma das posições validas seja igual a 's', terminar o programa
	if(maze[next_position.i][next_position.j] == 's'){
		maze[next_position.i][next_position.j] = 'o';
		system("cls");
		print_maze();
		cout << "Labirinto Finalizado!\n";
        return 0;
	}
	else{
		return walk(next_position);
	}
}

int main(int argc, char* argv[])
{	
	cout << "teste\n";
	// carregar o labirinto com o nome do arquivo recebido como argumento (argv[])
	pos_t initial_pos = load_maze("maze2.txt");
	
	print_maze();
	walk(initial_pos); 

	for (int i = 0; i < num_rows; ++i) {
        delete[] maze[i];
    }
    delete[] maze;

	return 0;
}
