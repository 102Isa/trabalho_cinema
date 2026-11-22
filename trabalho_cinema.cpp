#include <iostream>
#include <fstream>
#include <cstring>
#include <limits>
#include <cstdio>
#include <iomanip>
#include <string>

using namespace std;

// Nome dos arquivos utilizados
const char ARQUIVO[] = "filmes.dat";
const char ARQUIVO_TXT[] = "relatorio_filmes.txt";

// Estrutura que representa um filme
struct Filme {
    int codigo;
    char titulo[100];
    char genero[50];
    char diretor[100];
    int ano;
    int duracao;
    int classificacao;
    char status;
};

// Limpa o buffer de entrada
void limparBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Lê um número inteiro com validação
int lerInteiro(string mensagem) {
    int valor;

    while (true) {
        cout << mensagem;

        if (cin >> valor) {
            limparBuffer();
            return valor;
        }

        cout << "Entrada invalida! Digite um numero.\n";
        cin.clear();
        limparBuffer();
    }
}

// Lê um texto
void lerTexto(string mensagem, char destino[], int tamanho) {
    string texto;

    do {
        cout << mensagem;
        getline(cin, texto);

        if (texto.empty()) {
            cout << "O campo nao pode ficar vazio!\n";
        } else if (texto.length() >= tamanho) {
            cout << "Texto muito longo! Tente novamente.\n";
        }

    } while (texto.empty() || texto.length() >= tamanho);

    strcpy(destino, texto.c_str());
}

// Gera automaticamente o próximo código
int gerarCodigo() {
    ifstream arquivo(ARQUIVO, ios::binary);

    Filme filme;
    int maiorCodigo = 0;

    while (arquivo.read(reinterpret_cast<char*>(&filme),
                        sizeof(Filme))) {
        if (filme.codigo > maiorCodigo) {
            maiorCodigo = filme.codigo;
        }
    }

    arquivo.close();

    return maiorCodigo + 1;
}

// Cadastra um novo filme
void cadastrarFilme() {
    Filme filme;

    cout << "\n========== CADASTRAR FILME ==========\n";

    filme.codigo = gerarCodigo();

    cout << "Codigo gerado: " << filme.codigo << endl;

    lerTexto("Titulo: ", filme.titulo, 100);
    lerTexto("Genero: ", filme.genero, 50);
    lerTexto("Diretor: ", filme.diretor, 100);

    do {
        filme.ano = lerInteiro("Ano de lancamento: ");

        if (filme.ano < 1888 || filme.ano > 2100) {
            cout << "Ano invalido!\n";
        }
    } while (filme.ano < 1888 || filme.ano > 2100);

    do {
        filme.duracao = lerInteiro("Duracao em minutos: ");

        if (filme.duracao <= 0) {
            cout << "Duracao invalida!\n";
        }
    } while (filme.duracao <= 0);

    do {
        filme.classificacao =
            lerInteiro("Classificacao indicativa (0, 10, 12, 14, 16 ou 18): ");

        if (filme.classificacao != 0 &&
            filme.classificacao != 10 &&
            filme.classificacao != 12 &&
            filme.classificacao != 14 &&
            filme.classificacao != 16 &&
            filme.classificacao != 18) {
            cout << "Classificacao invalida!\n";
        }

    } while (filme.classificacao != 0 &&
             filme.classificacao != 10 &&
             filme.classificacao != 12 &&
             filme.classificacao != 14 &&
             filme.classificacao != 16 &&
             filme.classificacao != 18);

    filme.status = 'A';

    ofstream arquivo(ARQUIVO, ios::binary | ios::app);

    if (!arquivo) {
        cout << "Erro ao abrir o arquivo!\n";
        return;
    }

    arquivo.write(reinterpret_cast<char*>(&filme), sizeof(Filme));

    arquivo.close();

    cout << "\nFilme cadastrado com sucesso!\n";
    cout << "Codigo do filme: " << filme.codigo << endl;
}

// Consulta um filme pelo código
void consultarFilme() {
    int codigo;

    cout << "\n========== CONSULTAR FILME ==========\n";

    codigo = lerInteiro("Digite o codigo do filme: ");

    ifstream arquivo(ARQUIVO, ios::binary);

    if (!arquivo) {
        cout << "Nenhum cadastro encontrado.\n";
        return;
    }

    Filme filme;
    bool encontrado = false;

    while (arquivo.read(reinterpret_cast<char*>(&filme),
                        sizeof(Filme))) {

        if (filme.codigo == codigo && filme.status == 'A') {
            cout << "\n------ DADOS DO FILME ------\n";
            cout << "Codigo: " << filme.codigo << endl;
            cout << "Titulo: " << filme.titulo << endl;
            cout << "Genero: " << filme.genero << endl;
            cout << "Diretor: " << filme.diretor << endl;
            cout << "Ano: " << filme.ano << endl;
            cout << "Duracao: " << filme.duracao << " minutos\n";
            cout << "Classificacao: "
                 << filme.classificacao << " anos\n";
            cout << "Status: Ativo\n";

            encontrado = true;
            break;
        }
    }

    arquivo.close();

    if (!encontrado) {
        cout << "Filme nao encontrado ou excluido.\n";
    }
}

// Exporta os registros para um arquivo TXT
void exportarTXT() {
    ifstream arquivo(ARQUIVO, ios::binary);

    if (!arquivo) {
        cout << "\nNenhum registro para exportar.\n";
        return;
    }

    ofstream txt(ARQUIVO_TXT);

    if (!txt) {
        cout << "Erro ao criar o arquivo TXT!\n";
        return;
    }

    Filme filme;
    int quantidade = 0;

    txt << "========================================\n";
    txt << "       RELATORIO DE FILMES - CINEMANAGER\n";
    txt << "========================================\n\n";

    while (arquivo.read(reinterpret_cast<char*>(&filme),
                        sizeof(Filme))) {

        quantidade++;

        txt << "Codigo: " << filme.codigo << endl;
        txt << "Titulo: " << filme.titulo << endl;
        txt << "Genero: " << filme.genero << endl;
        txt << "Diretor: " << filme.diretor << endl;
        txt << "Ano: " << filme.ano << endl;
        txt << "Duracao: " << filme.duracao << " minutos\n";
        txt << "Classificacao: " << filme.classificacao << " anos\n";
        txt << "Status: "
            << (filme.status == 'A' ? "Ativo" : "Excluido")
            << endl;

        txt << "----------------------------------------\n";
    }

    txt << "\nTotal de registros armazenados: " << quantidade;

    arquivo.close();
    txt.close();

    cout << "\nRelatorio exportado com sucesso!\n";
    cout << "Arquivo gerado: " << ARQUIVO_TXT << endl;
}

// Realiza a exclusão lógica de um filme
void excluirFilme() {
    int codigo;

    cout << "\n========== EXCLUIR FILME ==========\n";

    codigo = lerInteiro("Digite o codigo do filme: ");

    fstream arquivo(ARQUIVO,
                    ios::binary | ios::in | ios::out);

    if (!arquivo) {
        cout << "Arquivo nao encontrado.\n";
        return;
    }

    Filme filme;
    bool encontrado = false;

    while (true) {
        streampos posicao = arquivo.tellg();

        if (!arquivo.read(reinterpret_cast<char*>(&filme),
                          sizeof(Filme))) {
            break;
        }

        if (filme.codigo == codigo && filme.status == 'A') {

            filme.status = 'E';

            arquivo.seekp(posicao);

            arquivo.write(reinterpret_cast<char*>(&filme),
                          sizeof(Filme));

            encontrado = true;
            break;
        }
    }

    arquivo.close();

    if (encontrado) {
        cout << "\nFilme excluido logicamente com sucesso!\n";
        cout << "Os dados continuam armazenados no arquivo.\n";
    } else {
        cout << "\nFilme nao encontrado ou ja excluido.\n";
    }
}

// Conta os registros ativos e armazenados
void contabilizarFilmes() {
    ifstream arquivo(ARQUIVO, ios::binary);

    Filme filme;

    int total = 0;
    int ativos = 0;
    int excluidos = 0;

    if (arquivo) {
        while (arquivo.read(reinterpret_cast<char*>(&filme),
                            sizeof(Filme))) {

            total++;

            if (filme.status == 'A') {
                ativos++;
            } else {
                excluidos++;
            }
        }

        arquivo.close();
    }

    cout << "\n========== CONTROLE DE REGISTROS ==========\n";
    cout << "Total de registros armazenados: " << total << endl;
    cout << "Filmes ativos: " << ativos << endl;
    cout << "Filmes excluidos: " << excluidos << endl;
}

// Menu principal
void menu() {
    int opcao;

    do {
        cout << "\n";
        cout << "========================================\n";
        cout << "             CINEMANAGER\n";
        cout << "       SISTEMA DE GERENCIAMENTO\n";
        cout << "========================================\n";

        contabilizarFilmes();

        cout << "\n--------------- MENU ----------------\n";
        cout << "1 - Cadastrar filme\n";
        cout << "2 - Consultar filme por codigo\n";
        cout << "3 - Exportar registros para TXT\n";
        cout << "4 - Excluir filme\n";
        cout << "5 - Contabilizar registros\n";
        cout << "0 - Sair do sistema\n";
        cout << "-------------------------------------\n";

        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {

            case 1:
                cadastrarFilme();
                break;

            case 2:
                consultarFilme();
                break;

            case 3:
                exportarTXT();
                break;

            case 4:
                excluirFilme();
                break;

            case 5:
                contabilizarFilmes();
                break;

            case 0:
                cout << "\nEncerrando o CineManager...\n";
                cout << "Obrigado por utilizar o sistema!\n";
                break;

            default:
                cout << "\nOpcao invalida! Tente novamente.\n";
        }

    } while (opcao != 0);
}

// Funcao principal
int main() {
    menu();

    return 0;
}
