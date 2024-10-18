#include <stdio.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <thread>
#include <vector>

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
            // Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
            // Se o valor for 'e' salvar o valor em initial_pos
            file >> maze[i][j];
            if (maze[i][j] == 'e') {
                initial_pos.i = i;
                initial_pos.j = j;
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

void clear_console()
{
    // Limpar o console para atualizar o estado do labirinto
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void walk(pos_t pos)
{
    // Marcar a posição atual com o símbolo 'o'
    maze[pos.i][pos.j] = 'o';

    // Limpar o console e imprimir o estado atual do labirinto
    clear_console();
    print_maze();
    this_thread::sleep_for(chrono::milliseconds(200)); // Pausa para visualizar a exploração

    pos_t aux;
    vector<pos_t> new_positions;

    // Verificar e adicionar as posições válidas adjacentes
    if (pos.j + 1 < num_cols && (maze[pos.i][pos.j + 1] == 'x' || maze[pos.i][pos.j + 1] == 's')) {
        aux.i = pos.i;
        aux.j = pos.j + 1;
        new_positions.push_back(aux);
    }
    if (pos.i + 1 < num_rows && (maze[pos.i + 1][pos.j] == 'x' || maze[pos.i + 1][pos.j] == 's')) {
        aux.i = pos.i + 1;
        aux.j = pos.j;
        new_positions.push_back(aux);
    }
    if (pos.i - 1 >= 0 && (maze[pos.i - 1][pos.j] == 'x' || maze[pos.i - 1][pos.j] == 's')) {
        aux.i = pos.i - 1;
        aux.j = pos.j;
        new_positions.push_back(aux);
    }
    if (pos.j - 1 >= 0 && (maze[pos.i][pos.j - 1] == 'x' || maze[pos.i][pos.j - 1] == 's')) {
        aux.i = pos.i;
        aux.j = pos.j - 1;
        new_positions.push_back(aux);
    }

    // Se encontrar a saída
    if (!new_positions.empty() && maze[new_positions[0].i][new_positions[0].j] == 's') {
        maze[new_positions[0].i][new_positions[0].j] = 'o';
        clear_console();
        print_maze();
        cout << "Labirinto Finalizado!\n";
        return;
    }

    // Criar novas threads para explorar caminhos adicionais
    vector<thread> threads;
    for (size_t i = 1; i < new_positions.size(); ++i) {
        threads.emplace_back(walk, new_positions[i]);
    }

    // Explorar o primeiro caminho com a thread atual
    if (!new_positions.empty()) {
        walk(new_positions[0]);
    }

    // Esperar as threads adicionais terminarem
    for (auto& t : threads) {
        t.join();
    }
}

int main(int argc, char* argv[])
{
    // Carregar o labirinto com o nome do arquivo recebido como argumento (argv[])
    pos_t initial_pos = load_maze("./data/maze5.txt");

    print_maze();
    walk(initial_pos);

    // Liberar a memória alocada
    for (int i = 0; i < num_rows; ++i) {
        delete[] maze[i];
    }
    delete[] maze;

    return 0;
}
