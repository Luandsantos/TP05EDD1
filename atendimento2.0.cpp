#include <iostream>
using namespace std;

#define MAX 5

struct Fila {
    int ini;
    int fim;
    float nos[MAX];
};

Fila* init() {
    Fila *f = new Fila;
    f->ini = 0;
    f->fim = 0;
    return f;
}

int isEmpty(Fila *f) {
    return (f->ini == f->fim);
}

int incrementa(int i) {
    return (i == MAX ? 0 : ++i);
}

int enqueue(Fila *f, float v) {
    int podeEnfileirar = (incrementa(f->fim) != f->ini);
    if (podeEnfileirar) {
        f->nos[f->fim] = v;
        f->fim = incrementa(f->fim);
    }
    return podeEnfileirar;
}

float dequeue(Fila *f) {
    float ret;
    if (isEmpty(f)) {
        ret = -1;
    } else {
        ret = f->nos[f->ini];
        f->ini = incrementa(f->ini);
    }
    return ret;
}

int count(Fila *f) {
    int qtde = 0;
    int i = f->ini;
    while (i != f->fim) {
        qtde++;
        i = incrementa(i);
    }
    return qtde;
}

void print(Fila *f) {
    int i = f->ini;
    while (i != f->fim) {
        cout << f->nos[i] << " ";
        i = incrementa(i);
    }
    cout << endl;
}

void freeFila(Fila *f) {
    delete f;
}

struct Guiche {
    int id;
    Fila* senhasAtendidas;
    Guiche* prox;
};

Guiche* criarGuiche(int id) {
    Guiche* guiche = new Guiche;
    guiche->id = id;
    guiche->senhasAtendidas = init();
    guiche->prox = nullptr;
    return guiche;
}

Guiche* adicionarGuiche(Guiche* head, int id) {
    Guiche* novoGuiche = criarGuiche(id);
    if (head == nullptr) {
        return novoGuiche;
    }
    Guiche* atual = head;
    while (atual->prox != nullptr) {
        atual = atual->prox;
    }
    atual->prox = novoGuiche;
    return head;
}

Guiche* encontrarGuiche(Guiche* head, int id) {
    Guiche* atual = head;
    while (atual != nullptr) {
        if (atual->id == id) {
            return atual;
        }
        atual = atual->prox;
    }
    return nullptr;
}

void liberarGuiches(Guiche* head) {
    while (head != nullptr) {
        Guiche* temp = head;
        head = head->prox;
        freeFila(temp->senhasAtendidas);
        delete temp;
    }
}

int contarSenhasAtendidas(Guiche* head) {
    int total = 0;
    while (head != nullptr) {
        total += count(head->senhasAtendidas);
        head = head->prox;
    }
    return total;
}

void listarSenhasAtendidas(Guiche* guiche) {
    if (guiche != nullptr) {
        print(guiche->senhasAtendidas);
    } else {
        cout << "Guichê não encontrado." << endl;
    }
}

int main(int argc, char** argv) {
    Fila *senhasGeradas = init();
    float senha_base = 0;

    Guiche* guiches = nullptr;
    bool continuar = true;
    int input;

    while (continuar) {
        cout << "Quantidade de senhas não atendidas: ";
        print(senhasGeradas);
        cout << "Número de guichês abertos: ";
        int qtdeGuiches = 0;
        Guiche* temp = guiches;
        while (temp != nullptr) {
            qtdeGuiches++;
            temp = temp->prox;
        }
        cout << qtdeGuiches << endl;

        cout << "0. Sair" << endl;
        cout << "1. Gerar senha" << endl;
        cout << "2. Abrir guichê" << endl;
        cout << "3. Realizar atendimento" << endl;
        cout << "4. Listar senhas atendidas" << "\n" << endl;
        cout << "Digite o número da opção que deseja: ";
        cin >> input;

        switch (input) {
            case 0:
                if (isEmpty(senhasGeradas)) {
                    continuar = false;
                    freeFila(senhasGeradas);

                    int totalAtendidas = contarSenhasAtendidas(guiches);
                    cout << "Quantidade de senhas atendidas: " << totalAtendidas << endl;
                    cout << "Programa encerrado." << "\n" << endl;
                } else {
                    cout << "Ainda restam senhas a serem atendidas." << "\n" << endl;
                }
                break;

            case 1:
                senha_base++;
                enqueue(senhasGeradas, senha_base);
                cout << "Senha " << senha_base << " gerada." << "\n" << endl;
                break;

            case 2: {
                int novoId = qtdeGuiches + 1;
                guiches = adicionarGuiche(guiches, novoId);
                cout << "Guichê " << novoId << " aberto." << "\n" << endl;
                break;
            }

            case 3: {
                if (isEmpty(senhasGeradas)) {
                    cout << "Não há nenhuma senha para ser atendida agora." << "\n" << endl;
                } else {
                    int guicheId;
                    cout << "Digite o id do guichê: ";
                    cin >> guicheId;

                    Guiche* guiche = encontrarGuiche(guiches, guicheId);
                    if (guiche != nullptr) {
                        float senhaAtendida = dequeue(senhasGeradas);
                        enqueue(guiche->senhasAtendidas, senhaAtendida);
                        cout << "Senha " << senhaAtendida << " atendida no guichê " << guicheId << "." << "\n" << endl;
                    } else {
                        cout << "Guichê não encontrado." << "\n" <<  endl;
                    }
                }
                break;
            }

            case 4: {
                int guicheId;
                cout << "Digite o id do guichê: ";
                cin >> guicheId;

                Guiche* guiche = encontrarGuiche(guiches, guicheId);
                cout << "Senhas atendidas no guichê " << guicheId << ": ";
                listarSenhasAtendidas(guiche);
                break;
            }

            default:
                cout << "Opção inválida." << "\n" << endl;
                break;
        }
    }

    liberarGuiches(guiches);
    return 0;
}
