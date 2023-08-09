#include <iostream>
#include <stdlib.h>

class No {
    public:
    int item;
    No *esq;
    No *dir;

    No(){
        this->esq = NULL;
        this->dir = NULL;
    }

    No(int item){
        this->item = item;
        this->esq = NULL;
        this->dir = NULL;
    }   
};


class Arvore {

    private: No* raiz;

    public:

    Arvore(){ this->raiz = NULL; }

    void add(int item) {
        No *novo = new No(item);

        if(this->raiz == NULL) this->raiz = novo;

        else {
            No *atual = this->raiz;
            while(true){
                if(novo->item <= atual->item){
                    if (atual->esq == NULL){
                        atual->esq = novo;
                        return ;
                    }
                    atual = atual->esq;
                }
                else{
                    if(atual->dir == NULL){
                        atual->dir = novo;
                        return ;
                    }
                    atual = atual->dir;
                }
            } 
        }
    }

    private: void print(No* no, int level = 0) {
        if (no != NULL) {
            for (int i = 0; i < level; i++) {
                std::cout << "    "; // Adiciona quatro espaços para cada nível de profundidade
            }
            std::cout << "|-- " << no->item << std::endl;

            print(no->esq, level + 1);
            print(no->dir, level + 1);
        }
    }

    public: void print() {
        //std::cout << this->raiz->item << " ";
        this->print(this->raiz);
        std::cout << std::endl;
    }
};