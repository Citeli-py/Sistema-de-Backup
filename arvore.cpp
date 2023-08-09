#include <vector>
#include <iostream>
#include <filesystem>
#include "filehandler.hpp"

using namespace std;
namespace fs = filesystem;

vector<string> split_path(const string& path) {
    vector<string> directories;
    stringstream ss(path);
    string directory;

    while (getline(ss, directory, '\\')) {
        if (!directory.empty()) {
            directories.push_back(directory);
        }
    }

    return directories;
}

class No {
public:
    string nome;
    fs::path caminho_absoluto;
    fs::path caminho_relativo;
    int last_mod;
    bool is_directory;
    vector<No*> children;

    No(){} // Construtor padrão
     
    No(fs::path path, bool is_directory = false){
        this->nome = path.filename().string();
        this->caminho_absoluto = path;
        this->caminho_relativo = this->setRelativePath(path.string()); //ERRO
        this->is_directory = is_directory;
        //this->last_mod = this->lastmod();
    }

    No *find(const string& nome) const {
        for(auto child : children){
            if(child->nome == nome)
                return child;
        }

        return NULL;
    }

    No *find(No* no) const {
        for(auto child : children){
            if(child->nome == no->nome)
                return child;
        }

        return NULL;
    }

    private: string setRelativePath(string caminho_raiz) {
        int tam = caminho_raiz.size();
        return this->caminho_absoluto.string().substr(tam, caminho_absoluto.string().size());
    }

    /*int lastmod(){
        if (!this->is_directory) {
            return getlastmod(this->caminho.string());
        }
        return 0;
    }*/
};

class Arvore {

private:
    No *raiz;
    fs::path caminho_raiz;

public:

    Arvore(string caminho_raiz) {
        this->caminho_raiz = caminho_raiz;
        this->raiz = new No(this->caminho_raiz, true);  // Nó raiz representa o diretório raiz do dispositivo externo
        this->build_tree(this->caminho_raiz, this->raiz);
    }

    public: No* getRaiz(){ return this->raiz; }

    private: void build_tree(const fs::path& path, No *node) {
        for (const auto& entry : fs::directory_iterator(path)) {
            string item_name = entry.path().string();
            if (entry.is_directory()) {
                No *child_node = new No(item_name, true);
                build_tree(entry.path(), child_node);
                node->children.push_back(child_node);
            } 
            else {
                No *child_node = new No(item_name);
                node->children.push_back(child_node);
            }
        }
    }

    private: void print(No *no, int level = 0) {
        string indentation(level * 4, ' '); // Espaçamento para indentação

        // Imprime o nome do nó atual (arquivo ou diretório)
        cout << indentation << "|-- " << no->caminho_relativo << (no->is_directory ? "/" : "") << endl;
        //cout << no->last_mod<< endl;

        // Chama a função recursivamente para os nós filhos
        for (auto child : no->children) {
            print(child, level + 1);
        }
    }

    public: void print() {
        this->print(this->raiz);
        cout << endl;
    }

    public: void search(string caminho) {
        
        vector<string> dirs = split_path(caminho);

        No* prox = this->raiz;
        for(string dir : dirs) {
            prox = prox->find(dir);
            if(prox == NULL) {
                cout << "Esse caminho nao existe\n";
                return ;
            }
        }

        if (prox->nome == dirs[dirs.size()-1])
            cout << prox->nome <<" Existe!\n";
    }

    private: string RelativoToAbsoluto(string caminho_relativo){
        return this->caminho_raiz.string() + "\\" + caminho_relativo;
    }

    public: void add(string caminho) {
        vector<string> dirs = split_path(caminho); //Caminho relativo
        No* prox = this->raiz;

        for(int i=0; i< (dirs.size()-1); i++ ) {
            if(prox->find(dirs[i]) == NULL) {
                string path = prox->caminho_absoluto.string()+"\\"+dirs[i];
                createDir(path);
 
                prox->children.push_back(new No(path, true));
                prox = prox->children[0];
            }
            else
                prox = prox->find(dirs[i]);
        }
        string path = prox->caminho_absoluto.string()+"\\"+dirs[dirs.size()-1];
        prox->children.push_back(new No(path));
    }

    private: void transfer(No* no_origem, No* no_destino) {
        for(auto filho: no_origem->children){
            if(no_destino->find(filho) == NULL){ // Não existe arquivo/diretorio no destino
                if(!filho->is_directory){ // é arquivo
                    No* novo = new No(this->RelativoToAbsoluto(filho->caminho_relativo.string()));
                    no_destino->children.push_back(novo);
                    copy(filho->caminho_absoluto, no_destino->caminho_absoluto.string());
                }
                else{ // é Diretório
                    createDir(this->RelativoToAbsoluto(filho->caminho_relativo.string()));
                    No* novo = new No(this->RelativoToAbsoluto(filho->caminho_relativo.string()), true);
                    no_destino->children.push_back(novo);
                    this->transfer(novo, filho);
                }
            }
            else { // Existe arquivo/diretorio no destino
                if(filho->is_directory){ // é diretorio
                    this->transfer(no_destino->find(filho), filho);
                }
            }
            
        }
    }

    public: void transfer(Arvore arvore_origem) {
        No* raiz_origem = arvore_origem.getRaiz();
        No* raiz_destino = this->raiz;

        this->transfer(raiz_origem, raiz_destino);
    }
};

int main(){
    Arvore src = Arvore("C:\\Users\\Matheus\\Desktop\\Pastas\\programação\\C++\\Backup\\src");
    src.print();

    Arvore dst = Arvore("C:\\Users\\Matheus\\Desktop\\Pastas\\programação\\C++\\Backup\\dst");
    dst.print();

    dst.transfer(src);
    dst.print();

    return 0;
}
